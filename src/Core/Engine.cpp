#include "Engine.h"

dsp::Engine::Engine()
        : audioBuffer(0, 0)
        , nodeProcessor(std::make_shared<NodeProcessor>(0, 0, 0, 0))
        , midiProcessor(std::make_shared<MidiProcessor>())
        , numInputChannels(0)
        , numOutputChannels(0)
        , numSamples(0)
        , sampleRate(0) {}

dsp::Engine::~Engine() {
    lock();

#ifdef DSP_USE_RTAUDIO
    try {
        if (dac.isStreamOpen()) {
            dac.abortStream();
        }
    } catch (RtAudioError &error) {
#if defined(DEBUG) || defined(_DEBUG)
        error.printMessage();
#endif
    }

    try {
        if (dac.isStreamOpen()) {
            dac.closeStream();
        }
    } catch (RtAudioError &error) {
#if defined(DEBUG) || defined(_DEBUG)
        error.printMessage();
#endif
    }
#endif

    unlock();
}

std::vector<unsigned int> dsp::Engine::getInputDevices() {
    std::vector<unsigned int> inputDevices;
#ifdef DSP_USE_RTAUDIO
    unsigned int deviceCount = getDeviceCount();

    for (unsigned int i = 0; i < deviceCount; ++i) {
        try {
            RtAudio::DeviceInfo deviceInfo = dac.getDeviceInfo(i);
            if (deviceInfo.inputChannels != 0) {
                inputDevices.push_back(i);
            }
        } catch (RtAudioError &error) {
#if defined(DEBUG) || defined(_DEBUG)
            error.printMessage();
#endif
        }
    }
#endif
    return inputDevices;
}

std::vector<unsigned int> dsp::Engine::getOutputDevices() {
    std::vector<unsigned int> outputDevices;
#ifdef DSP_USE_RTAUDIO
    unsigned int deviceCount = getDeviceCount();

    for (unsigned int i = 0; i < deviceCount; ++i) {
        try {
            RtAudio::DeviceInfo deviceInfo = dac.getDeviceInfo(i);
            if (deviceInfo.outputChannels != 0) {
                outputDevices.push_back(i);
            }
        } catch (RtAudioError &error) {
#if defined(DEBUG) || defined(_DEBUG)
            error.printMessage();
#endif
        }
    }
#endif
    return outputDevices;
}

std::vector<unsigned int> dsp::Engine::getSampleRates(unsigned int inputDevice, unsigned int outputDevice) {
    std::vector<unsigned int> sampleRates;
#ifdef DSP_USE_RTAUDIO
    std::vector<unsigned int> inputSampleRates = getInputSampleRates(inputDevice);
    std::vector<unsigned int> outputSampleRates = getOutputSampleRates(outputDevice);
    if (inputSampleRates.size() == 0) {
        return outputSampleRates;
    }
    if (outputSampleRates.size() == 0) {
        return inputSampleRates;
    }
    std::set_intersection(inputSampleRates.begin(),
                          inputSampleRates.end(),
                          outputSampleRates.begin(),
                          outputSampleRates.end(),
                          std::back_inserter(sampleRates));
#endif
    return sampleRates;
}

unsigned int dsp::Engine::getDefaultInputDevice() {
#ifdef DSP_USE_RTAUDIO
    unsigned int deviceCount = getDeviceCount();
    for (unsigned int i = 0; i < deviceCount; ++i) {
        try {
            RtAudio::DeviceInfo deviceInfo = dac.getDeviceInfo(i);
            if (deviceInfo.inputChannels != 0) {
                return i;
            }
        } catch (RtAudioError &error) {
#if defined(DEBUG) || defined(_DEBUG)
            error.printMessage();
#endif
        }
    }
#endif
    return -1;
}

unsigned int dsp::Engine::getDefaultOutputDevice() {
#ifdef DSP_USE_RTAUDIO
    unsigned int deviceCount = getDeviceCount();
    for (unsigned int i = 0; i < deviceCount; ++i) {
        try {
            RtAudio::DeviceInfo deviceInfo = dac.getDeviceInfo(i);
            if (deviceInfo.outputChannels != 0) {
                return i;
            }
        } catch (RtAudioError &error) {
#if defined(DEBUG) || defined(_DEBUG)
            error.printMessage();
#endif
        }
    }
#endif
    return -1;
}

unsigned int dsp::Engine::getDefaultSampleRate(unsigned int inputDevice, unsigned int outputDevice) {
#ifdef DSP_USE_RTAUDIO
    std::vector<unsigned int> inputSampleRates = getInputSampleRates(inputDevice);
    std::vector<unsigned int> outputSampleRates = getOutputSampleRates(outputDevice);
    if (inputSampleRates.size() == 0 && outputSampleRates.size() == 0) {
        return 0;
    }
    if (inputSampleRates.size() == 0) {
        return outputSampleRates[0];
    }
    if (outputSampleRates.size() == 0) {
        return inputSampleRates[0];
    }
    size_t i = 0, j = 0;
    while (i < inputSampleRates.size() && j < outputSampleRates.size()) {
        if (inputSampleRates[i] < outputSampleRates[j]) {
            ++i;
        } else if (inputSampleRates[i] > outputSampleRates[j]) {
            ++j;
        } else {
            return inputSampleRates[i];
        }
    }
#endif
    return 0;
}

void dsp::Engine::setup(unsigned int inputDevice,
                        unsigned int outputDevice,
                        unsigned int numSamples,
                        unsigned int sampleRate) {
    lock();

#ifdef DSP_USE_RTAUDIO
    try {
        if (dac.isStreamOpen()) {
            dac.abortStream();
        }
    } catch (RtAudioError &error) {
#if defined(DEBUG) || defined(_DEBUG)
        error.printMessage();
#endif
    }

    try {
        if (dac.isStreamOpen()) {
            dac.closeStream();
        }
    } catch (RtAudioError &error) {
#if defined(DEBUG) || defined(_DEBUG)
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

    RtAudioFormat format = (sizeof(Sample) == 8) ? RTAUDIO_FLOAT64 : RTAUDIO_FLOAT32;

    RtAudio::StreamOptions options;
    options.flags = RTAUDIO_SCHEDULE_REALTIME;
    options.numberOfBuffers = 1;
    options.priority = 10;

    try {
        dac.openStream(outputParameters.nChannels > 0 ? &outputParameters : NULL,
                       inputParameters.nChannels > 0 ? &inputParameters : NULL,
                       format,
                       sampleRate,
                       &numSamples,
                       &dsp::Engine::tick,
                       this,
                       &options);
    } catch (RtAudioError &error) {
#if defined(DEBUG) || defined(_DEBUG)
        error.printMessage();
#endif
    }

    inputDeviceName = getDeviceName(inputDevice);
    outputDeviceName = getDeviceName(outputDevice);

    numInputChannels = inputParameters.nChannels;
    numOutputChannels = outputParameters.nChannels;
    this->numSamples = numSamples;
    this->sampleRate = sampleRate;
#endif

    if (numInputChannels > nodeProcessor->getNumInputChannels()) {
        nodeProcessor->setNumInputChannels(numInputChannels);
    }
    if (numOutputChannels > nodeProcessor->getNumOutputChannels()) {
        nodeProcessor->setNumOutputChannels(numOutputChannels);
    }
    nodeProcessor->setNumSamples(this->numSamples);
    nodeProcessor->setSampleRate(this->sampleRate);

    audioBuffer.setSize(std::max(this->numInputChannels, this->numOutputChannels), this->numSamples);

    midiProcessor->setNumSamples(this->numSamples);
    midiProcessor->setSampleRate(this->sampleRate);

    unlock();
}

void dsp::Engine::start() {
    lock();
#ifdef DSP_USE_RTAUDIO
    try {
        dac.startStream();
    } catch (RtAudioError &error) {
#if defined(DEBUG) || defined(_DEBUG)
        error.printMessage();
#endif
    }
#endif
    unlock();
}

std::string dsp::Engine::getDeviceName(unsigned int device) {
    std::string deviceName;
#ifdef DSP_USE_RTAUDIO
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

unsigned int dsp::Engine::getNumSamples() const {
    return numSamples;
}

unsigned int dsp::Engine::getSampleRate() const {
    return sampleRate;
}

dsp::AudioBuffer<dsp::Sample> &dsp::Engine::getAudioBuffer() {
    return audioBuffer;
}

void dsp::Engine::processAudioBufferNoLock(Sample *inputBuffer, Sample *outputBuffer) {
    for (unsigned int channel = 0; channel < numInputChannels; ++channel) {
        Sample *audioInputChannel = audioBuffer.getWritePointer(channel);
        for (unsigned int k = 0, sample = channel; k < numSamples; ++k, sample += numInputChannels) {
            audioInputChannel[k] = inputBuffer[sample];
        }
    }

    nodeProcessor->process(audioBuffer, midiProcessor->getMidiBuffer());

    for (unsigned int channel = 0; channel < numOutputChannels; ++channel) {
        const Sample *audioOutputChannel = audioBuffer.getReadPointer(channel);
        for (unsigned int k = 0, sample = channel; k < numSamples; ++k, sample += numOutputChannels) {
            outputBuffer[sample] = audioOutputChannel[k];
        }
    }
}

std::shared_ptr<dsp::NodeProcessor> dsp::Engine::getNodeProcessor() const {
    return nodeProcessor;
}

std::shared_ptr<dsp::MidiProcessor> dsp::Engine::getMidiProcessor() const {
    return midiProcessor;
}

#ifdef DSP_USE_RTAUDIO
int dsp::Engine::tick(void *outputBuffer,
                      void *inputBuffer,
                      unsigned int nBufferFrames,
                      double streamTime,
                      RtAudioStreamStatus status,
                      void *pointer) {
    Engine *engine = reinterpret_cast<Engine *>(pointer);
    engine->lock();
    process(reinterpret_cast<Sample *>(inputBuffer), reinterpret_cast<Sample *>(outputBuffer), engine);
    engine->unlock();
    return 0;
}
#endif

void dsp::Engine::process(Sample *inputBuffer, Sample *outputBuffer, Engine *engine) {
    engine->getMidiProcessor()->processInputs();
    engine->processAudioBufferNoLock(inputBuffer, outputBuffer);
    engine->getMidiProcessor()->processOutputs();
}

unsigned int dsp::Engine::getDeviceCount() {
#ifdef DSP_USE_RTAUDIO
    unsigned int deviceCount;
    try {
        deviceCount = dac.getDeviceCount();
    } catch (RtAudioError &error) {
        deviceCount = 0;
#if defined(DEBUG) || defined(_DEBUG)
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
#ifdef DSP_USE_RTAUDIO
    try {
        inputSampleRates = dac.getDeviceInfo(inputDevice).sampleRates;
    } catch (RtAudioError &error) {}
#endif
    return inputSampleRates;
}
std::vector<unsigned int> dsp::Engine::getOutputSampleRates(unsigned int outputDevice) {
    std::vector<unsigned int> outputSampleRates;
#ifdef DSP_USE_RTAUDIO
    try {
        outputSampleRates = dac.getDeviceInfo(outputDevice).sampleRates;
    } catch (RtAudioError &error) {}
#endif
    return outputSampleRates;
}
