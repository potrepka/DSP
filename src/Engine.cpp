#include "Engine.h"

dsp::Engine::Engine()
        : numInputChannels(0)
        , numOutputChannels(0) {
    audio = std::make_shared<Audio>();
    midi = std::make_shared<Midi>();
}

dsp::Engine::~Engine() {
    lock();

#if USE_RTAUDIO
    try {
        if (dac.isStreamOpen()) {
            dac.abortStream();
        }
    } catch (RtAudioError &error) {
#if DEBUG
        error.printMessage();
#endif
    }

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

    unlock();
}

void dsp::Engine::lockAudio() {
    audio->lock();
}

void dsp::Engine::unlockAudio() {
    audio->unlock();
}

void dsp::Engine::lockMidi() {
    midi->lock();
}

void dsp::Engine::unlockMidi() {
    midi->unlock();
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
    lock();

#if USE_RTAUDIO
    try {
        if (dac.isStreamOpen()) {
            dac.abortStream();
        }
    } catch (RtAudioError &error) {
#if DEBUG
        error.printMessage();
#endif
    }

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
    setSampleRateNoLock(sampleRate);
    setBufferSizeNoLock(bufferSize);
#endif

    if (numInputChannels > audio->getAudioInput()->getNumChannels()) {
        audio->getAudioInput()->setNumChannels(numInputChannels);
        audio->getAudioInputClipping()->setNumChannels(numInputChannels);
    }
    if (numOutputChannels > audio->getAudioOutput()->getNumChannels()) {
        audio->getAudioOutput()->setNumChannels(numOutputChannels);
        audio->getAudioOutputClipping()->setNumChannels(numOutputChannels);
    }
    audio->setSampleRate(sampleRate);
    audio->setBufferSize(bufferSize);

    unlock();
}

void dsp::Engine::start() {
    lock();
#if USE_RTAUDIO
    try {
        dac.startStream();
    } catch (RtAudioError &error) {
#if DEBUG
        error.printMessage();
#endif
    }
#endif
    unlock();
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

std::string dsp::Engine::getInputDeviceName() const {
    return inputDeviceName;
}

std::string dsp::Engine::getOutputDeviceName() const {
    return outputDeviceName;
}

unsigned int dsp::Engine::getNumInputChannels() const {
    return numInputChannels;
}

unsigned int dsp::Engine::getNumOutputChannels() const {
    return numOutputChannels;
}

std::shared_ptr<dsp::OutputParameter> dsp::Engine::getAudioInput() const {
    return audio->getAudioInput();
}

std::shared_ptr<dsp::InputParameter> dsp::Engine::getAudioOutput() const {
    return audio->getAudioOutput();
}

std::shared_ptr<dsp::OutputParameter> dsp::Engine::getAudioInputClipping() const {
    return audio->getAudioInputClipping();
}

std::shared_ptr<dsp::OutputParameter> dsp::Engine::getAudioOutputClipping() const {
    return audio->getAudioOutputClipping();
}

unsigned int dsp::Engine::getNumUnits() const {
    return audio->getNumUnits();
}

std::shared_ptr<dsp::Unit> dsp::Engine::getUnit(unsigned int index) const {
    return audio->getUnit(index);
}

void dsp::Engine::pushUnit(std::shared_ptr<Unit> unit, bool sort) {
    audio->pushUnit(unit, sort);
}

void dsp::Engine::pushUnits(std::vector<std::shared_ptr<Unit>> units, bool sort) {
    audio->pushUnits(units, sort);
}

void dsp::Engine::replaceUnit(std::shared_ptr<Unit> unit, std::shared_ptr<Unit> replacement) {
    audio->replaceUnit(unit, replacement);
}

void dsp::Engine::removeUnit(std::shared_ptr<Unit> unit) {
    audio->removeUnit(unit);
}

void dsp::Engine::removeUnits(std::vector<std::shared_ptr<Unit>> units) {
    audio->removeUnits(units);
}

void dsp::Engine::sortUnits() {
    audio->sortUnits();
}

unsigned int dsp::Engine::getNumMidiInputs() const {
    return midi->getNumMidiInputs();
}

unsigned int dsp::Engine::getNumMidiOutputs() const {
    return midi->getNumMidiOutputs();
}

std::shared_ptr<dsp::Midi::MidiInput> dsp::Engine::getMidiInput(unsigned int index) const {
    return midi->getMidiInput(index);
}

std::shared_ptr<dsp::Midi::MidiOutput> dsp::Engine::getMidiOutput(unsigned int index) const {
    return midi->getMidiOutput(index);
}

std::shared_ptr<dsp::Midi::MidiInput> dsp::Engine::pushMidiInput(unsigned int port) {
    return midi->pushMidiInput(port);
}

std::shared_ptr<dsp::Midi::MidiOutput> dsp::Engine::pushMidiOutput(unsigned int port) {
    return midi->pushMidiOutput(port);
}

void dsp::Engine::removeMidiInput(std::shared_ptr<Midi::MidiInput> input) {
    midi->removeMidiInput(input);
}

void dsp::Engine::removeMidiOutput(std::shared_ptr<Midi::MidiOutput> output) {
    midi->removeMidiOutput(output);
}

#if USE_RTAUDIO
int dsp::Engine::tick(void *outputBuffer,
                      void *inputBuffer,
                      unsigned int nBufferFrames,
                      double streamTime,
                      RtAudioStreamStatus status,
                      void *pointer) {
    Engine *engine = (Engine *)pointer;
    engine->lock();
    process((DSP_FLOAT *)inputBuffer,
            (DSP_FLOAT *)outputBuffer,
            nBufferFrames,
            engine->getNumInputChannels(),
            engine->getNumOutputChannels(),
            engine);
    engine->unlock();
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
