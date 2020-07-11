#include "Engine.h"

dsp::Engine::Engine() : numInputChannels(0), numOutputChannels(0), sampleRate(0), bufferSize(0) {
    audio = std::make_shared<Audio>();
    midi = std::make_shared<Midi>();
}

dsp::Engine::~Engine() {
#if USE_RTAUDIO
    try {
        if (dac.isStreamOpen()) {
            dac.closeStream();
        }
    } catch (RtAudioError &error) {
#if DEBUG
        error.printMessage();
#endif
    }
#endif
}

std::vector<unsigned int> dsp::Engine::getInputDevices() {
    std::vector<unsigned int> inputDevices;
#if USE_RTAUDIO
    unsigned int deviceCount = getDeviceCount();

    for (unsigned int i = 0; i < deviceCount; i++) {
        try {
            RtAudio::DeviceInfo deviceInfo = dac.getDeviceInfo(i);
            if (deviceInfo.inputChannels != 0) {
                inputDevices.push_back(i);
            }
        } catch (RtAudioError &error) {
#if DEBUG
            error.printMessage();
#endif
        }
    }
#endif
    return inputDevices;
}

std::vector<unsigned int> dsp::Engine::getOutputDevices() {
    std::vector<unsigned int> outputDevices;
#if USE_RTAUDIO
    unsigned int deviceCount = getDeviceCount();

    for (unsigned int i = 0; i < deviceCount; i++) {
        try {
            RtAudio::DeviceInfo deviceInfo = dac.getDeviceInfo(i);
            if (deviceInfo.outputChannels != 0) {
                outputDevices.push_back(i);
            }
        } catch (RtAudioError &error) {
#if DEBUG
            error.printMessage();
#endif
        }
    }
#endif
    return outputDevices;
}

std::vector<unsigned int> dsp::Engine::getSampleRates(unsigned int inputDevice, unsigned int outputDevice) {
    std::vector<unsigned int> sampleRates;
#if USE_RTAUDIO
    std::vector<unsigned int> inputSampleRates = getInputSampleRates(inputDevice);
    std::vector<unsigned int> outputSampleRates = getOutputSampleRates(outputDevice);
    std::set_intersection(inputSampleRates.begin(),
                          inputSampleRates.end(),
                          outputSampleRates.begin(),
                          outputSampleRates.end(),
                          std::back_inserter(sampleRates));
#endif
    return sampleRates;
}

unsigned int dsp::Engine::getDefaultInputDevice() {
#if USE_RTAUDIO
    unsigned int deviceCount = getDeviceCount();
    for (unsigned int i = 0; i < deviceCount; i++) {
        try {
            RtAudio::DeviceInfo deviceInfo = dac.getDeviceInfo(i);
            if (deviceInfo.inputChannels != 0) {
                return i;
            }
        } catch (RtAudioError &error) {
#if DEBUG
            error.printMessage();
#endif
        }
    }
#endif
    return -1;
}

unsigned int dsp::Engine::getDefaultOutputDevice() {
#if USE_RTAUDIO
    unsigned int deviceCount = getDeviceCount();
    for (unsigned int i = 0; i < deviceCount; i++) {
        try {
            RtAudio::DeviceInfo deviceInfo = dac.getDeviceInfo(i);
            if (deviceInfo.outputChannels != 0) {
                return i;
            }
        } catch (RtAudioError &error) {
#if DEBUG
            error.printMessage();
#endif
        }
    }
#endif
    return -1;
}

unsigned int dsp::Engine::getDefaultSampleRate(unsigned int inputDevice, unsigned int outputDevice) {
#if USE_RTAUDIO
    std::vector<unsigned int> inputSampleRates = getInputSampleRates(inputDevice);
    std::vector<unsigned int> outputSampleRates = getOutputSampleRates(outputDevice);
    int i = 0, j = 0;
    while (i < inputSampleRates.size() && outputSampleRates.size() < j) {
        if (inputSampleRates[i] < outputSampleRates[j]) {
            i++;
        } else if (inputSampleRates[i] > outputSampleRates[j]) {
            j++;
        } else {
            return inputSampleRates[i];
        }
    }
#endif
    return 0;
}

void dsp::Engine::setup(unsigned int inputDevice,
                        unsigned int outputDevice,
                        unsigned int sampleRate,
                        unsigned int bufferSize) {
#if USE_RTAUDIO
    try {
        if (dac.isStreamOpen()) {
            dac.closeStream();
        }
    } catch (RtAudioError &error) {
#if DEBUG
        error.printMessage();
#endif
    }

    RtAudio::StreamParameters inputParameters;
    RtAudio::StreamParameters outputParameters;

    inputParameters.deviceId = inputDevice;
    try {
        inputParameters.nChannels = dac.getDeviceInfo(inputDevice).inputChannels;
    } catch (RtAudioError &error) { inputParameters.nChannels = 0; }
    inputParameters.firstChannel = 0;

    outputParameters.deviceId = outputDevice;
    try {
        outputParameters.nChannels = dac.getDeviceInfo(outputDevice).outputChannels;
    } catch (RtAudioError &error) { outputParameters.nChannels = 0; }
    outputParameters.firstChannel = 0;

    RtAudioFormat format = (sizeof(DSP_FLOAT) == 8) ? RTAUDIO_FLOAT64 : RTAUDIO_FLOAT32;

    try {
        dac.openStream(outputParameters.nChannels > 0 ? &outputParameters : NULL,
                       inputParameters.nChannels > 0 ? &inputParameters : NULL,
                       format,
                       sampleRate,
                       &bufferSize,
                       &dsp::Engine::tick,
                       this);
    } catch (RtAudioError &error) {
#if DEBUG
        error.printMessage();
#endif
    }

    inputDeviceName = getDeviceName(inputDevice);
    outputDeviceName = getDeviceName(outputDevice);

    numInputChannels = inputParameters.nChannels;
    numOutputChannels = outputParameters.nChannels;
    this->sampleRate = sampleRate;
    this->bufferSize = bufferSize;
#endif

    if (numInputChannels > audio->getAudioInput()->getNumChannels()) {
        audio->getAudioInput()->setNumChannels(numInputChannels);
    }
    if (numOutputChannels > audio->getAudioOutput()->getNumChannels()) {
        audio->getAudioOutput()->setNumChannels(numOutputChannels);
    }
    audio->setSampleRate(sampleRate);
    audio->setBufferSize(bufferSize);
}

void dsp::Engine::start() {
#if USE_RTAUDIO
    try {
        dac.startStream();
    } catch (RtAudioError &error) {
#if DEBUG
        error.printMessage();
#endif
    }
#endif
}

std::string dsp::Engine::getDeviceName(unsigned int device) {
    std::string deviceName;
#if USE_RTAUDIO
    try {
        deviceName = dac.getDeviceInfo(device).name;
        deviceName = deviceName.substr(deviceName.find(":") + 2);
    } catch (RtAudioError &error) { deviceName = "None"; }
#endif
    return deviceName;
}

std::string dsp::Engine::getInputDeviceName() {
    return inputDeviceName;
}

std::string dsp::Engine::getOutputDeviceName() {
    return outputDeviceName;
}

unsigned int dsp::Engine::getNumInputChannels() {
    return numInputChannels;
}

unsigned int dsp::Engine::getNumOutputChannels() {
    return numOutputChannels;
}

unsigned int dsp::Engine::getSampleRate() {
    return sampleRate;
}

unsigned int dsp::Engine::getBufferSize() {
    return bufferSize;
}

std::shared_ptr<dsp::Unit::OutputParameter> dsp::Engine::getAudioInput() {
    return audio->getAudioInput();
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::Engine::getAudioOutput() {
    return audio->getAudioOutput();
}

unsigned int dsp::Engine::getNumUnits() {
    return audio->getNumUnits();
}

std::shared_ptr<dsp::Unit> dsp::Engine::getUnit(unsigned int index) {
    return audio->getUnit(index);
}

void dsp::Engine::pushUnit(std::shared_ptr<Unit> unit) {
    audio->pushUnit(unit);
}

void dsp::Engine::insertUnit(unsigned int index, std::shared_ptr<Unit> unit) {
    audio->insertUnit(index, unit);
}

void dsp::Engine::removeUnit(std::shared_ptr<Unit> unit) {
    audio->removeUnit(unit);
}

void dsp::Engine::removeUnit(unsigned int index) {
    audio->removeUnit(index);
}

void dsp::Engine::sortUnits() {
    audio->sortUnits();
}

unsigned int dsp::Engine::getNumMidiInputs() {
    return midi->getNumMidiInputs();
}

unsigned int dsp::Engine::getNumMidiOutputs() {
    return midi->getNumMidiOutputs();
}

std::shared_ptr<dsp::Midi::MidiInput> dsp::Engine::getMidiInput(unsigned int index) {
    return midi->getMidiInput(index);
}

std::shared_ptr<dsp::Midi::MidiOutput> dsp::Engine::getMidiOutput(unsigned int index) {
    return midi->getMidiOutput(index);
}

void dsp::Engine::pushMidiInput(unsigned int port) {
    midi->pushMidiInput(port);
}

void dsp::Engine::pushMidiOutput(unsigned int port) {
    midi->pushMidiOutput(port);
}

void dsp::Engine::removeMidiInput(unsigned int index) {
    midi->removeMidiInput(index);
}

void dsp::Engine::removeMidiOutput(unsigned int index) {
    midi->removeMidiOutput(index);
}

#if USE_RTAUDIO
int dsp::Engine::tick(void *outputBuffer,
                      void *inputBuffer,
                      unsigned int nBufferFrames,
                      double streamTime,
                      RtAudioStreamStatus status,
                      void *pointer) {
    Engine *engine = (Engine *)pointer;
    process((DSP_FLOAT *)inputBuffer,
            (DSP_FLOAT *)outputBuffer,
            nBufferFrames,
            engine->getNumInputChannels(),
            engine->getNumOutputChannels(),
            engine);
    return 0;
}
#endif

void dsp::Engine::process(DSP_FLOAT *inputBuffer,
                          DSP_FLOAT *outputBuffer,
                          unsigned int numFrames,
                          unsigned int numInputChannels,
                          unsigned int numOutputChannels,
                          Engine *engine) {
    engine->audio->zeroBuffers();
    engine->audio->readInterleaved(inputBuffer, numInputChannels, numFrames);
    engine->midi->processInputs();
    engine->audio->run();
    engine->midi->processOutputs();
    engine->audio->copyBuffers();
    engine->audio->writeInterleaved(outputBuffer, numOutputChannels, numFrames);
}

unsigned int dsp::Engine::getDeviceCount() {
#if USE_RTAUDIO
    unsigned int deviceCount;
    try {
        deviceCount = dac.getDeviceCount();
    } catch (RtAudioError &error) {
        deviceCount = 0;
#if DEBUG
        error.printMessage();
#endif
    }
    return deviceCount;
#else
    return 0;
#endif
}

std::vector<unsigned int> dsp::Engine::getInputSampleRates(unsigned int inputDevice) {
    std::vector<unsigned int> inputSampleRates;
#if USE_RTAUDIO
    try {
        inputSampleRates = dac.getDeviceInfo(inputDevice).sampleRates;
    } catch (RtAudioError &error) {}
#endif
    return inputSampleRates;
}
std::vector<unsigned int> dsp::Engine::getOutputSampleRates(unsigned int outputDevice) {
    std::vector<unsigned int> outputSampleRates;
#if USE_RTAUDIO
    try {
        outputSampleRates = dac.getDeviceInfo(outputDevice).sampleRates;
    } catch (RtAudioError &error) {}
#endif
    return outputSampleRates;
}
