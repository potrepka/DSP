#include "Engine.h"

dsp::Engine::Engine() : numInputChannels(0), numOutputChannels(0), sampleRate(0), bufferSize(0) {
    audio = new Audio();
}

dsp::Engine::~Engine() {
#if USE_RTAUDIO
    try {
        if (dac.isStreamRunning()) {
            dac.stopStream();
        }
        if (dac.isStreamOpen()) {
            dac.closeStream();
        }
    } catch (RtAudioError &error) { error.printMessage(); }
#endif

    delete audio;
}

std::vector<unsigned int> dsp::Engine::getInputDevices() {
    std::vector<unsigned int> inputDevices;
#if USE_RTAUDIO
    unsigned int deviceCount;

    try {
        deviceCount = dac.getDeviceCount();
    } catch (RtAudioError &error) {
        deviceCount = 0;
        error.printMessage();
    }

    for (unsigned int i = 0; i < deviceCount; i++) {
        try {
            RtAudio::DeviceInfo deviceInfo = dac.getDeviceInfo(i);
            if (deviceInfo.inputChannels != 0) {
                inputDevices.push_back(i);
            }
        } catch (RtAudioError &error) { error.printMessage(); }
    }
#endif
    return inputDevices;
}

std::vector<unsigned int> dsp::Engine::getOutputDevices() {
    std::vector<unsigned int> outputDevices;
#if USE_RTAUDIO
    unsigned int deviceCount;

    try {
        deviceCount = dac.getDeviceCount();
    } catch (RtAudioError &error) {
        deviceCount = 0;
        error.printMessage();
    }

    for (unsigned int i = 0; i < deviceCount; i++) {
        try {
            RtAudio::DeviceInfo deviceInfo = dac.getDeviceInfo(i);
            if (deviceInfo.outputChannels != 0) {
                outputDevices.push_back(i);
            }
        } catch (RtAudioError &error) { error.printMessage(); }
    }
#endif
    return outputDevices;
}

std::vector<unsigned int> dsp::Engine::getAvailableSampleRates(unsigned int inputDevice, unsigned int outputDevice) {
    std::vector<unsigned int> sampleRates;
#if USE_RTAUDIO
    std::vector<unsigned int> inputSampleRates;
    std::vector<unsigned int> outputSampleRates;

    try {
        inputSampleRates = dac.getDeviceInfo(inputDevice).sampleRates;
    } catch (RtAudioError &error) {}

    try {
        outputSampleRates = dac.getDeviceInfo(outputDevice).sampleRates;
    } catch (RtAudioError &error) {}

    std::set_intersection(inputSampleRates.begin(),
                          inputSampleRates.end(),
                          outputSampleRates.begin(),
                          outputSampleRates.end(),
                          std::back_inserter(sampleRates));
#endif
    return sampleRates;
}

void dsp::Engine::setup(unsigned int inputDevice,
                        unsigned int outputDevice,
                        unsigned int sampleRate,
                        unsigned int bufferSize) {
#if USE_RTAUDIO
    try {
        if (dac.isStreamRunning()) {
            dac.stopStream();
        }
        if (dac.isStreamOpen()) {
            dac.closeStream();
        }
    } catch (RtAudioError &error) { error.printMessage(); }

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
    } catch (RtAudioError &error) { error.printMessage(); }

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
    } catch (RtAudioError &error) { error.printMessage(); }
#endif
}

void dsp::Engine::stop() {
#if USE_RTAUDIO
    try {
        dac.stopStream();
    } catch (RtAudioError &error) { error.printMessage(); }
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

std::size_t dsp::Engine::getNumUnits() {
    return audio->getNumUnits();
}

std::shared_ptr<dsp::Unit> dsp::Engine::getUnit(std::size_t index) {
    return audio->getUnit(index);
}

void dsp::Engine::pushUnit(std::shared_ptr<Unit> unit) {
    audio->pushUnit(unit);
}

void dsp::Engine::insertUnit(std::size_t index, std::shared_ptr<Unit> unit) {
    audio->insertUnit(index, unit);
}

void dsp::Engine::removeUnit(std::shared_ptr<Unit> unit) {
    audio->removeUnit(unit);
}

void dsp::Engine::removeUnit(std::size_t index) {
    audio->removeUnit(index);
}

void dsp::Engine::sortUnits() {
    audio->sortUnits();
}

#if USE_RTAUDIO
int dsp::Engine::tick(void *outputBuffer,
                      void *inputBuffer,
                      unsigned int nBufferFrames,
                      double streamTime,
                      RtAudioStreamStatus status,
                      void *pointer) {
    Engine *engine = (Engine *)pointer;
    process((double *)inputBuffer,
            (double *)outputBuffer,
            nBufferFrames,
            engine->getNumInputChannels(),
            engine->getNumOutputChannels(),
            engine);
    return 0;
}
#endif

void dsp::Engine::process(double *inputBuffer,
                          double *outputBuffer,
                          unsigned int numFrames,
                          unsigned int numInputChannels,
                          unsigned int numOutputChannels,
                          Engine *engine) {
    engine->audio->zeroBuffers();
    engine->audio->readInterleaved(inputBuffer, numInputChannels, numFrames);
    engine->audio->run();
    engine->audio->copyBuffers();
    engine->audio->writeInterleaved(outputBuffer, numOutputChannels, numFrames);
}
