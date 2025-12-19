/**
 * Comprehensive Emscripten/Embind bindings for DSP library
 * Exposes all 78+ classes, constants, enums, and functions
 */

#include <emscripten/bind.h>
#include <emscripten/val.h>

#include "../../../../src/DSP.h"

using namespace emscripten;
using namespace dsp;

EMSCRIPTEN_BINDINGS(native_audio) {
  // ========== Mathematical Constants ==========

  constant("PI", PI);
  constant("ONE_OVER_PI", ONE_OVER_PI);
  constant("TAU", TAU);
  constant("ONE_OVER_TAU", ONE_OVER_TAU);
  constant("PI_OVER_TWO", PI_OVER_TWO);
  constant("SQRT_OF_TWO", SQRT_OF_TWO);
  constant("LOG_OF_TWO", LOG_OF_TWO);
  constant("ONE_OVER_LOG_OF_TWO", ONE_OVER_LOG_OF_TWO);
  constant("PHI", PHI);
  constant("E", E);

  // ========== Global Enums ==========

  // Type
  enum_<Type>("Type")
      .value("RATIO", Type::RATIO)
      .value("INTEGER", Type::INTEGER)
      .value("BOOLEAN", Type::BOOLEAN)
      .value("HERTZ", Type::HERTZ)
      .value("SECONDS", Type::SECONDS);

  // Space
  enum_<Space>("Space")
      .value("TIME", Space::TIME)
      .value("FREQUENCY", Space::FREQUENCY);

  // Shape
  constant("SHAPE_LINEAR", Shape::LINEAR);
  constant("SHAPE_EXPONENTIAL", Shape::EXPONENTIAL);

  // Interpolation
  constant("INTERPOLATION_NONE", Interpolation::NONE);
  constant("INTERPOLATION_LINEAR", Interpolation::LINEAR);
  constant("INTERPOLATION_HERMITE", Interpolation::HERMITE);

  // Input::Mode
  enum_<Input::Mode>("InputMode")
      .value("SUM", Input::Mode::SUM)
      .value("MINIMUM", Input::Mode::MINIMUM)
      .value("MAXIMUM", Input::Mode::MAXIMUM);

  // ========== Node-Specific Enums ==========

  // Recorder::Mode
  enum_<Recorder::Mode>("RecorderMode")
      .value("FIXED_SINGLE", Recorder::Mode::FIXED_SINGLE)
      .value("FIXED_DOUBLE", Recorder::Mode::FIXED_DOUBLE)
      .value("VARIABLE", Recorder::Mode::VARIABLE);

  // Spread::Mode
  constant("SPREAD_MODE_UNIPOLAR", Spread::Mode::UNIPOLAR);
  constant("SPREAD_MODE_BIPOLAR", Spread::Mode::BIPOLAR);

  // Clipper::Mode
  constant("CLIPPER_MODE_CLIP", Clipper::Mode::CLIP);
  constant("CLIPPER_MODE_WRAP", Clipper::Mode::WRAP);
  constant("CLIPPER_MODE_FOLD", Clipper::Mode::FOLD);

  // Shaper::Mode
  constant("SHAPER_MODE_HYPERBOLIC", Shaper::Mode::HYPERBOLIC);
  constant("SHAPER_MODE_RATIONAL", Shaper::Mode::RATIONAL);

  // Biquad::Mode
  constant("BIQUAD_MODE_LOW_PASS", Biquad::Mode::LOW_PASS);
  constant("BIQUAD_MODE_HIGH_PASS", Biquad::Mode::HIGH_PASS);
  constant("BIQUAD_MODE_BAND_PASS", Biquad::Mode::BAND_PASS);
  constant("BIQUAD_MODE_BAND_STOP", Biquad::Mode::BAND_STOP);
  constant("BIQUAD_MODE_LOW_SHELF", Biquad::Mode::LOW_SHELF);
  constant("BIQUAD_MODE_HIGH_SHELF", Biquad::Mode::HIGH_SHELF);
  constant("BIQUAD_MODE_PEAK", Biquad::Mode::PEAK);
  constant("BIQUAD_MODE_ALL_PASS", Biquad::Mode::ALL_PASS);

  // OnePole::Mode
  constant("ONEPOLE_MODE_LOW_PASS", OnePole::Mode::LOW_PASS);
  constant("ONEPOLE_MODE_HIGH_PASS", OnePole::Mode::HIGH_PASS);

  // MoorerOscillator::Mode
  constant("MOORER_OSCILLATOR_MODE_ONE_SIDED",
           MoorerOscillator::Mode::ONE_SIDED);
  constant("MOORER_OSCILLATOR_MODE_TWO_SIDED",
           MoorerOscillator::Mode::TWO_SIDED);

  // Noise::Mode
  constant("NOISE_MODE_WHITE", Noise::Mode::WHITE);
  constant("NOISE_MODE_PINK", Noise::Mode::PINK);

  // Phasor::Mode
  constant("PHASOR_MODE_WRAPPED", Phasor::Mode::WRAPPED);
  constant("PHASOR_MODE_UNBOUNDED", Phasor::Mode::UNBOUNDED);

  // Hyperbolic::Mode
  constant("HYPERBOLIC_MODE_SINE", Hyperbolic::Mode::SINE);
  constant("HYPERBOLIC_MODE_COSINE", Hyperbolic::Mode::COSINE);
  constant("HYPERBOLIC_MODE_TANGENT", Hyperbolic::Mode::TANGENT);

  // Trigonometric::Mode
  constant("TRIGONOMETRIC_MODE_SINE", Trigonometric::Mode::SINE);
  constant("TRIGONOMETRIC_MODE_COSINE", Trigonometric::Mode::COSINE);
  constant("TRIGONOMETRIC_MODE_TANGENT", Trigonometric::Mode::TANGENT);

  // ========== Vectors ==========

  register_vector<uint8_t>("UInt8Vector");
  register_vector<unsigned int>("UIntVector");
  register_vector<Sample>("SampleVector");
  register_vector<audiofft::Sample>("AudioFFTSampleVector");
  register_vector<std::shared_ptr<Buffer>>("BufferVector");
  register_vector<std::shared_ptr<Input>>("InputVector");
  register_vector<std::shared_ptr<Output>>("OutputVector");
  register_vector<std::shared_ptr<Node>>("NodeVector");
  register_vector<std::shared_ptr<MidiProcessor::Input>>("MidiInputVector");
  register_vector<std::shared_ptr<MidiProcessor::Output>>("MidiOutputVector");

  // ========== Multimaps ==========

  // register_multimap<double, MidiMessage>("MidiMessageMultimap");

  // ========== Global Functions ==========

  function("byteToUnipolar", &byteToUnipolar);
  function("unipolarToByte", &unipolarToByte);
  function("shortToBipolar", &shortToBipolar);
  function("bipolarToShort", &bipolarToShort);
  function("clip", &clip);
  function("wrap", &wrap);
  function("linear", &linear, allow_raw_pointers());
  function("hermite", &hermite, allow_raw_pointers());

  // ========== Core Classes ==========

  // Data class
  class_<Data>("Data")
      .smart_ptr<std::shared_ptr<Data>>("Data")
      .constructor(&std::make_shared<Data, size_t, size_t>)
      .function("getNumChannels", &Data::getNumChannels)
      .function("getNumSamples", &Data::getNumSamples)
      .function("setSize", &Data::setSize)
      .function("clear", &Data::clear)
      .function("getReadPointer",
                optional_override([](const Data& data, size_t channel) {
                  const Sample* pointer = data.getReadPointer(channel);
                  size_t length = data.getNumSamples();
                  return emscripten::typed_memory_view(
                      length, const_cast<Sample*>(pointer));
                }))
      .function("getWritePointer",
                optional_override([](Data& data, size_t channel) {
                  Sample* pointer = data.getWritePointer(channel);
                  size_t length = data.getNumSamples();
                  return emscripten::typed_memory_view(length, pointer);
                }))
      .function("getMagnitude", &Data::getMagnitude)
      .function("getRMSLevel", &Data::getRMSLevel)
      .function("getArrayOfReadPointers",
                optional_override([](const Data& data) {
                  emscripten::val array = emscripten::val::array();
                  size_t numChannels = data.getNumChannels();
                  size_t numSamples = data.getNumSamples();
                  const Sample* const* pointers = data.getArrayOfReadPointers();
                  for (size_t i = 0; i < numChannels; ++i) {
                    const Sample* pointer = pointers[i];
                    auto view = emscripten::typed_memory_view(
                        numSamples, const_cast<Sample*>(pointer));
                    array.call<void>("push", view);
                  }
                  return array;
                }))
      .function("getArrayOfWritePointers", optional_override([](Data& data) {
                  emscripten::val array = emscripten::val::array();
                  size_t numChannels = data.getNumChannels();
                  size_t numSamples = data.getNumSamples();
                  Sample** pointers = data.getArrayOfWritePointers();
                  for (size_t i = 0; i < numChannels; ++i) {
                    Sample* pointer = pointers[i];
                    auto view =
                        emscripten::typed_memory_view(numSamples, pointer);
                    array.call<void>("push", view);
                  }
                  return array;
                }));

  // Wrapper class
  class_<Wrapper>("Wrapper")
      .smart_ptr<std::shared_ptr<Wrapper>>("Wrapper")
      .constructor(&std::make_shared<Wrapper>)
      .constructor(&std::make_shared<Wrapper, Data&>)
      .function("getNumChannels", &Wrapper::getNumChannels)
      .function("getNumSamples", &Wrapper::getNumSamples)
      .function("getChannelPointer",
                optional_override([](const Wrapper& wrapper, size_t channel) {
                  Sample* pointer = wrapper.getChannelPointer(channel);
                  size_t length = wrapper.getNumSamples();
                  return emscripten::typed_memory_view(length, pointer);
                }))
      .function("getSingleChannel", &Wrapper::getSingleChannel)
      .function("getSampleRange", &Wrapper::getSampleRange)
      .function("clear", &Wrapper::clear)
      .function("fill", &Wrapper::fill)
      .function("apply", &Wrapper::apply)
      .function(
          "replaceWithApplicationOf",
          select_overload<Wrapper&(std::function<Sample(Sample)>, Wrapper)>(
              &Wrapper::replaceWithApplicationOf))
      .function("replaceWithApplicationOfTwoArgs",
                select_overload<Wrapper&(std::function<Sample(Sample, Sample)>,
                                         Wrapper, Wrapper)>(
                    &Wrapper::replaceWithApplicationOf))
      .function("replaceWithApplicationOfThreeArgs",
                select_overload<Wrapper&(
                    std::function<Sample(Sample, Sample, Sample)>, Wrapper,
                    Wrapper, Wrapper)>(&Wrapper::replaceWithApplicationOf))
      .function("copyFrom", &Wrapper::copyFrom)
      .function("add", select_overload<Wrapper&(Sample)>(&Wrapper::add))
      .function("addWrapper", select_overload<Wrapper&(Wrapper)>(&Wrapper::add))
      .function("multiplyBy",
                select_overload<Wrapper&(Sample)>(&Wrapper::multiplyBy))
      .function("multiplyByWrapper",
                select_overload<Wrapper&(Wrapper)>(&Wrapper::multiplyBy))
      .function("addProductOf", select_overload<Wrapper&(Wrapper, Sample)>(
                                    &Wrapper::addProductOf))
      .function(
          "addProductOfWrappers",
          select_overload<Wrapper&(Wrapper, Wrapper)>(&Wrapper::addProductOf))
      .function("replaceWithNegativeOf", &Wrapper::replaceWithNegativeOf)
      .function("replaceWithAbsoluteValueOf",
                &Wrapper::replaceWithAbsoluteValueOf)
      .function("replaceWithSumOf", select_overload<Wrapper&(Wrapper, Sample)>(
                                        &Wrapper::replaceWithSumOf))
      .function("replaceWithSumOfWrappers",
                select_overload<Wrapper&(Wrapper, Wrapper)>(
                    &Wrapper::replaceWithSumOf))
      .function("replaceWithProductOf",
                select_overload<Wrapper&(Wrapper, Sample)>(
                    &Wrapper::replaceWithProductOf))
      .function("replaceWithProductOfWrappers",
                select_overload<Wrapper&(Wrapper, Wrapper)>(
                    &Wrapper::replaceWithProductOf))
      .function("replaceWithMinOf", select_overload<Wrapper&(Wrapper, Sample)>(
                                        &Wrapper::replaceWithMinOf))
      .function("replaceWithMinOfWrappers",
                select_overload<Wrapper&(Wrapper, Wrapper)>(
                    &Wrapper::replaceWithMinOf))
      .function("replaceWithMaxOf", select_overload<Wrapper&(Wrapper, Sample)>(
                                        &Wrapper::replaceWithMaxOf))
      .function("replaceWithMaxOfWrappers",
                select_overload<Wrapper&(Wrapper, Wrapper)>(
                    &Wrapper::replaceWithMaxOf))
      .function("getSample", &Wrapper::getSample)
      .function("setSample", &Wrapper::setSample);

  // Buffer class
  class_<Buffer, base<Lockable>>("Buffer")
      .smart_ptr<std::shared_ptr<Buffer>>("Buffer")
      .constructor(&std::make_shared<Buffer, Type, Space, Sample, Sample,
                                     size_t, size_t>)
      .function("getType", &Buffer::getType)
      .function("setType", &Buffer::setType)
      .function("getSpace", &Buffer::getSpace)
      .function("setSpace", &Buffer::setSpace)
      .function("getRange", &Buffer::getRange)
      .function("setRange", &Buffer::setRange)
      .function("getDefaultValue", &Buffer::getDefaultValue)
      .function("setDefaultValue", &Buffer::setDefaultValue)
      .function("getNumChannels", &Buffer::getNumChannels)
      .function("setNumChannels", &Buffer::setNumChannels)
      .function("getNumSamples", &Buffer::getNumSamples)
      .function("setNumSamples", &Buffer::setNumSamples)
      .function("setSize", &Buffer::setSize)
      .function("getChannelValues", &Buffer::getChannelValues)
      .function("setChannelValues", &Buffer::setChannelValues)
      .function("getChannelValue", &Buffer::getChannelValue)
      .function("setSingleChannelValue", &Buffer::setSingleChannelValue)
      .function("setAllChannelValues", &Buffer::setAllChannelValues)
      .function("getPeak", &Buffer::getPeak)
      .function("getRMS", &Buffer::getRMS)
      .function("getData", &Buffer::getData)
      .function("getWrapper", &Buffer::getWrapper);

  // Input class (extends Buffer)
  class_<Input, base<Buffer>>("Input")
      .smart_ptr<std::shared_ptr<Input>>("Input")
      .smart_ptr_constructor<std::shared_ptr<Input>>(
          "Input",
          &std::make_shared<Input, Type, Space, Sample, Sample, size_t, size_t>)
      .function("getMode", &Input::getMode)
      .function("setMode", &Input::setMode)
      .function("getConnections", &Input::getConnections)
      .function("connect", &Input::connect)
      .function("disconnect", &Input::disconnect)
      .function("disconnectAll", &Input::disconnectAll);

  // Output class (extends Buffer)
  class_<Output, base<Buffer>>("Output")
      .smart_ptr<std::shared_ptr<Output>>("Output")
      .constructor(&std::make_shared<Output, Type, Space, Sample, Sample,
                                     size_t, size_t>)
      .function("getConnections", &Output::getConnections)
      .function("connect", &Output::connect)
      .function("disconnect", &Output::disconnect)
      .function("disconnectAll", &Output::disconnectAll);

  // Engine class
  class_<Engine, base<Lockable>>("Engine")
      .smart_ptr<std::shared_ptr<Engine>>("Engine")
      .smart_ptr_constructor<std::shared_ptr<Engine>>("Engine",
                                                      &std::make_shared<Engine>)
      .function("getInputDevices", &Engine::getInputDevices)
      .function("getOutputDevices", &Engine::getOutputDevices)
      .function("getSampleRates", &Engine::getSampleRates)
      .function("getDefaultInputDevice", &Engine::getDefaultInputDevice)
      .function("getDefaultOutputDevice", &Engine::getDefaultOutputDevice)
      .function("getDefaultSampleRate", &Engine::getDefaultSampleRate)
      .function("setup", &Engine::setup)
      .function("start", &Engine::start)
      .function("getDeviceName", &Engine::getDeviceName)
      .function("getInputDeviceName", &Engine::getInputDeviceName)
      .function("getOutputDeviceName", &Engine::getOutputDeviceName)
      .function("getNumInputChannels", &Engine::getNumInputChannels)
      .function("getNumOutputChannels", &Engine::getNumOutputChannels)
      .function("getNumSamples", &Engine::getNumSamples)
      .function("getSampleRate", &Engine::getSampleRate)
      .function("getAudioBuffer", &Engine::getAudioBuffer)
      .function("getNodeProcessor", &Engine::getNodeProcessor)
      .function("getMidiProcessor", &Engine::getMidiProcessor);

  // Lockable base class
  class_<Lockable>("Lockable")
      .smart_ptr<std::shared_ptr<Lockable>>("Lockable")
      .constructor(&std::make_shared<Lockable>)
      .function("lock", &Lockable::lock)
      .function("unlock", &Lockable::unlock);

  // Node base class
  class_<Node>("Node")
      .smart_ptr<std::shared_ptr<Node>>("Node")
      .smart_ptr_constructor<std::shared_ptr<Node>>("Node",
                                                    &std::make_shared<Node>)
      .function("isActive", &Node::isActive)
      .function("setActive", &Node::setActive)
      .function("getNumChannels", &Node::getNumChannels)
      .function("setNumChannels", &Node::setNumChannels)
      .function("getNumInputChannels", &Node::getNumInputChannels)
      .function("setNumInputChannels", &Node::setNumInputChannels)
      .function("getNumOutputChannels", &Node::getNumOutputChannels)
      .function("setNumOutputChannels", &Node::setNumOutputChannels)
      .function("getNumSamples", &Node::getNumSamples)
      .function("setNumSamples", &Node::setNumSamples)
      .function("getSampleRate", &Node::getSampleRate)
      .function("setSampleRate", &Node::setSampleRate)
      .function("getOneOverNumSamples", &Node::getOneOverNumSamples)
      .function("getOneOverSampleRate", &Node::getOneOverSampleRate)
      .function("getInputs", &Node::getInputs)
      .function("getOutputs", &Node::getOutputs)
      .function("getChildren", &Node::getChildren)
      .function("addChild", &Node::addChild)
      .function("removeChild", &Node::removeChild)
      .function("sortChildren", &Node::sortChildren)
      .function("disconnectAll", &Node::disconnectAll)
      .function("process", &Node::process);

  // Consumer base class (extends Node)
  class_<Consumer, base<Node>>("Consumer")
      .smart_ptr<std::shared_ptr<Consumer>>("Consumer")
      .constructor(&std::make_shared<Consumer, Type, Space>)
      .function("getInput", &Consumer::getInput);

  // Producer base class (extends Node)
  class_<Producer, base<Node>>("Producer")
      .smart_ptr<std::shared_ptr<Producer>>("Producer")
      .constructor(&std::make_shared<Producer, Type, Space>)
      .function("getOutput", &Producer::getOutput);

  // Transformer base class (extends Node)
  class_<Transformer, base<Node>>("Transformer")
      .smart_ptr<std::shared_ptr<Transformer>>("Transformer")
      .constructor(&std::make_shared<Transformer, Type, Space>)
      .constructor(&std::make_shared<Transformer, Type, Type, Space>)
      .smart_ptr_constructor<std::shared_ptr<Transformer>>(
          "Transformer",
          &std::make_shared<Transformer, Type, Type, Space, Space>)
      .function("getInput", &Transformer::getInput)
      .function("getOutput", &Transformer::getOutput);

  // NodeProcessor
  class_<NodeProcessor>("NodeProcessor")
      .smart_ptr<std::shared_ptr<NodeProcessor>>("NodeProcessor")
      .smart_ptr_constructor<std::shared_ptr<NodeProcessor>>(
          "NodeProcessor",
          &std::make_shared<NodeProcessor, size_t, size_t, size_t, double>)
      .function("isActive", &NodeProcessor::isActive)
      .function("setActive", &NodeProcessor::setActive)
      .function("getAudioInput", &NodeProcessor::getAudioInput)
      .function("getAudioOutput", &NodeProcessor::getAudioOutput)
      .function("getAudioInputClipping", &NodeProcessor::getAudioInputClipping)
      .function("getAudioOutputClipping",
                &NodeProcessor::getAudioOutputClipping)
      .function("getNumInputChannels", &NodeProcessor::getNumInputChannels)
      .function("setNumInputChannels", &NodeProcessor::setNumInputChannels)
      .function("getNumOutputChannels", &NodeProcessor::getNumOutputChannels)
      .function("setNumOutputChannels", &NodeProcessor::setNumOutputChannels)
      .function("getNumSamples", &NodeProcessor::getNumSamples)
      .function("setNumSamples", &NodeProcessor::setNumSamples)
      .function("getSampleRate", &NodeProcessor::getSampleRate)
      .function("setSampleRate", &NodeProcessor::setSampleRate)
      .function("setInputSize", &NodeProcessor::setInputSize)
      .function("setOutputSize", &NodeProcessor::setOutputSize)
      .function("getDefaultNode", &NodeProcessor::getDefaultNode)
      .function("getNodes", &NodeProcessor::getNodes)
      .function("getInputMessages", &NodeProcessor::getInputMessages)
      .function("getOutputMessages", &NodeProcessor::getOutputMessages)
      .function("process",
                optional_override([](NodeProcessor& nodeProcessor, Data& data,
                                     MidiBuffer& midiBuffer) {
                  nodeProcessor.process(data, midiBuffer);
                }));

  // ========== Midi Classes ==========

  // MidiBuffer
  class_<MidiBuffer>("MidiBuffer")
      .smart_ptr<std::shared_ptr<MidiBuffer>>("MidiBuffer")
      .constructor(&std::make_shared<MidiBuffer>)
      .function("begin", &MidiBuffer::begin)
      .function("end", &MidiBuffer::end)
      .function("addEvent", &MidiBuffer::addEvent)
      .function("addEvents", &MidiBuffer::addEvents)
      .function("clear", &MidiBuffer::clear);

  // MidiMessage
  class_<MidiMessage>("MidiMessage")
      .constructor<uint8_t>()
      .constructor<uint8_t, uint8_t>()
      .constructor<uint8_t, uint8_t, uint8_t>()
      .class_function("fromArray",
                      optional_override([](const std::vector<uint8_t>& data) {
                        return MidiMessage(data.data(), data.size());
                      }))
      .class_function("noteOff", &MidiMessage::noteOff)
      .class_function("noteOn", &MidiMessage::noteOn)
      .class_function("aftertouchChange", &MidiMessage::aftertouchChange)
      .class_function("controllerEvent", &MidiMessage::controllerEvent)
      .class_function("programChange", &MidiMessage::programChange)
      .class_function("channelPressureChange",
                      &MidiMessage::channelPressureChange)
      .class_function("pitchWheel", &MidiMessage::pitchWheel)
      .class_function("allNotesOff", &MidiMessage::allNotesOff)
      .class_function("songPositionPointer", &MidiMessage::songPositionPointer)
      .class_function("midiClock", &MidiMessage::midiClock)
      .class_function("midiStart", &MidiMessage::midiStart)
      .class_function("midiContinue", &MidiMessage::midiContinue)
      .class_function("midiStop", &MidiMessage::midiStop)
      .function("isNote", &MidiMessage::isNote)
      .function("isNoteOff", &MidiMessage::isNoteOff)
      .function("isNoteOn", &MidiMessage::isNoteOn)
      .function("isAftertouch", &MidiMessage::isAftertouch)
      .function("isController", &MidiMessage::isController)
      .function("isProgramChange", &MidiMessage::isProgramChange)
      .function("isChannelPressure", &MidiMessage::isChannelPressure)
      .function("isPitchWheel", &MidiMessage::isPitchWheel)
      .function("isAllNotesOff", &MidiMessage::isAllNotesOff)
      .function("isSongPositionPointer", &MidiMessage::isSongPositionPointer)
      .function("isMidiClock", &MidiMessage::isMidiClock)
      .function("isMidiStart", &MidiMessage::isMidiStart)
      .function("isMidiContinue", &MidiMessage::isMidiContinue)
      .function("isMidiStop", &MidiMessage::isMidiStop)
      .function("getChannel", &MidiMessage::getChannel)
      .function("getNoteNumber", &MidiMessage::getNoteNumber)
      .function("getVelocity", &MidiMessage::getVelocity)
      .function("getAfterTouchValue", &MidiMessage::getAfterTouchValue)
      .function("getControllerNumber", &MidiMessage::getControllerNumber)
      .function("getControllerValue", &MidiMessage::getControllerValue)
      .function("getProgramChangeNumber", &MidiMessage::getProgramChangeNumber)
      .function("getChannelPressureValue",
                &MidiMessage::getChannelPressureValue)
      .function("getPitchWheelValue", &MidiMessage::getPitchWheelValue)
      .function("getSongPositionPointerMidiBeat",
                &MidiMessage::getSongPositionPointerMidiBeat)
      .function("getBytes", &MidiMessage::getBytes)
      .function("getRawData", optional_override([](const MidiMessage& msg) {
                  const uint8_t* pointer = msg.getRawData();
                  size_t size = msg.getRawDataSize();
                  return emscripten::typed_memory_view(
                      size, const_cast<uint8_t*>(pointer));
                }))
      .function("getRawDataSize", &MidiMessage::getRawDataSize);

  // MidiProcessor
  class_<MidiProcessor, base<Lockable>>("MidiProcessor")
      .smart_ptr<std::shared_ptr<MidiProcessor>>("MidiProcessor")
      .constructor(&std::make_shared<MidiProcessor>)
      .class_function("getNumMidiInputPorts",
                      &MidiProcessor::getNumMidiInputPorts)
      .class_function("getNumMidiOutputPorts",
                      &MidiProcessor::getNumMidiOutputPorts)
      .class_function("getMidiInputName", &MidiProcessor::getMidiInputName)
      .class_function("getMidiOutputName", &MidiProcessor::getMidiOutputName)
      .function("getNumSamples", &MidiProcessor::getNumSamples)
      .function("setNumSamples", &MidiProcessor::setNumSamples)
      .function("getSampleRate", &MidiProcessor::getSampleRate)
      .function("setSampleRate", &MidiProcessor::setSampleRate)
      .function("getMidiBuffer", &MidiProcessor::getMidiBuffer)
      .function("getInputs", &MidiProcessor::getInputs)
      .function("getOutputs", &MidiProcessor::getOutputs)
      .function("processInputs", &MidiProcessor::processInputs)
      .function("processOutputs", &MidiProcessor::processOutputs);

  // MidiProcessor::Input
  class_<MidiProcessor::Input, base<Lockable>>("MidiProcessorInput")
      .smart_ptr<std::shared_ptr<MidiProcessor::Input>>("MidiProcessor::Input")
      .smart_ptr_constructor<std::shared_ptr<MidiProcessor::Input>>(
          "MidiProcessor::Input",
          &std::make_shared<MidiProcessor::Input, unsigned int>)
      .function("callback",
                optional_override([](MidiProcessor::Input& input, double delta,
                                     const std::vector<uint8_t>& bytes) {
                  std::vector<uint8_t> tempBytes = bytes;
                  MidiProcessor::Input::callback(delta, &tempBytes, &input);
                }))
      .function("getDeviceName", &MidiProcessor::Input::getDeviceName)
      .function("setPort", &MidiProcessor::Input::setPort);
  // .function("getMessages", &MidiProcessor::Input::getMessages);

  // MidiProcessor::Output
  class_<MidiProcessor::Output, base<Lockable>>("MidiProcessorOutput")
      .smart_ptr<std::shared_ptr<MidiProcessor::Output>>(
          "MidiProcessor::Output")
      .smart_ptr_constructor<std::shared_ptr<MidiProcessor::Output>>(
          "MidiProcessor::Output",
          &std::make_shared<MidiProcessor::Output, unsigned int>)
      .function("getDeviceName", &MidiProcessor::Output::getDeviceName)
      .function("setPort", &MidiProcessor::Output::setPort)
      .function("sendMessageWithDelay",
                &MidiProcessor::Output::sendMessageWithDelay);

  // ========== Analyzer Nodes ==========

  // Recorder
  class_<Recorder, base<Consumer>>("Recorder")
      .smart_ptr<std::shared_ptr<Recorder>>("Recorder")
      .constructor(&std::make_shared<Recorder, Type, Space, Sample>)
      .function("getMode", &Recorder::getMode)
      .function("setMode", &Recorder::setMode)
      .function("getRecordingTime", &Recorder::getRecordingTime)
      .function("setRecordingTime", &Recorder::setRecordingTime)
      .function("getRecordingBuffer", &Recorder::getRecordingBuffer)
      .function("getGate", &Recorder::getGate)
      .function("getReset", &Recorder::getReset);

  // ========== Channel Nodes ==========

  // ChannelMerger
  class_<ChannelMerger, base<Producer>>("ChannelMerger")
      .smart_ptr<std::shared_ptr<ChannelMerger>>("ChannelMerger")
      .constructor(&std::make_shared<ChannelMerger, Type, Space>)
      .function("getInput", &ChannelMerger::getInput);

  // ChannelSplitter
  class_<ChannelSplitter, base<Consumer>>("ChannelSplitter")
      .smart_ptr<std::shared_ptr<ChannelSplitter>>("ChannelSplitter")
      .constructor(&std::make_shared<ChannelSplitter, Type, Space>)
      .function("getOutput", &ChannelSplitter::getOutput);

  // MidSide
  class_<MidSide, base<Consumer>>("MidSide")
      .smart_ptr<std::shared_ptr<MidSide>>("MidSide")
      .constructor(&std::make_shared<MidSide, Type, Space>)
      .function("getMixAmount", &MidSide::getMixAmount)
      .function("getMid", &MidSide::getMid)
      .function("getSide", &MidSide::getSide);

  // Spread
  class_<Spread, base<Transformer>>("Spread")
      .smart_ptr<std::shared_ptr<Spread>>("Spread")
      .constructor(&std::make_shared<Spread, Type, Space>)
      .function("getSpread", &Spread::getSpread)
      .function("getMode", &Spread::getMode);

  // StereoPanner
  class_<StereoPanner, base<Consumer>>("StereoPanner")
      .smart_ptr<std::shared_ptr<StereoPanner>>("StereoPanner")
      .constructor(&std::make_shared<StereoPanner, Type, Space>)
      .function("getDirection", &StereoPanner::getDirection)
      .function("getLeft", &StereoPanner::getLeft)
      .function("getRight", &StereoPanner::getRight);

  // ========== Delay Nodes ==========

  // Convolver
  class_<Convolver, base<Transformer>>("Convolver")
      .smart_ptr<std::shared_ptr<Convolver>>("Convolver")
      .constructor(&std::make_shared<Convolver>)
      .function("getHeadSize", &Convolver::getHeadSize)
      .function("setHeadSize", &Convolver::setHeadSize)
      .function("getTailSize", &Convolver::getTailSize)
      .function("setTailSize", &Convolver::setTailSize)
      .function("getBuffer", &Convolver::getBuffer)
      .function("setBuffer", &Convolver::setBuffer)
      .function("initConvolvers", &Convolver::initConvolvers);

  // VariableDelay
  class_<VariableDelay, base<Transformer>>("VariableDelay")
      .smart_ptr<std::shared_ptr<VariableDelay>>("VariableDelay")
      .constructor(&std::make_shared<VariableDelay, Type>)
      .function("getMaxDelayTime", &VariableDelay::getMaxDelayTime)
      .function("setMaxDelayTime", &VariableDelay::setMaxDelayTime)
      .function("getDelayTime", &VariableDelay::getDelayTime)
      .function("getDecayTime", &VariableDelay::getDecayTime)
      .function("getReset", &VariableDelay::getReset)
      .function("getFeedbackSource", &VariableDelay::getFeedbackSource)
      .function("getFeedbackSink", &VariableDelay::getFeedbackSink)
      .function("getFeedbackProcessor", &VariableDelay::getFeedbackProcessor);

  // ========== Dynamics Nodes ==========

  // Clipper
  class_<Clipper, base<Transformer>>("Clipper")
      .smart_ptr<std::shared_ptr<Clipper>>("Clipper")
      .constructor(&std::make_shared<Clipper, Type, Space>)
      .function("getMin", &Clipper::getMin)
      .function("getMax", &Clipper::getMax)
      .function("getMode", &Clipper::getMode);

  // CompressorGate
  class_<CompressorGate, base<Transformer>>("CompressorGate")
      .smart_ptr<std::shared_ptr<CompressorGate>>("CompressorGate")
      .constructor(&std::make_shared<CompressorGate>)
      .function("getControl", &CompressorGate::getControl)
      .function("getThreshold", &CompressorGate::getThreshold)
      .function("getSoftness", &CompressorGate::getSoftness)
      .function("getCompressionRatio", &CompressorGate::getCompressionRatio)
      .function("getGateRatio", &CompressorGate::getGateRatio)
      .function("getAttack", &CompressorGate::getAttack)
      .function("getRelease", &CompressorGate::getRelease)
      .function("getGain", &CompressorGate::getGain)
      .function("getGainResponse", select_overload<Sample(size_t, Sample)>(
                                       &CompressorGate::getGainResponse));

  // DryWet
  class_<DryWet, base<Producer>>("DryWet")
      .smart_ptr<std::shared_ptr<DryWet>>("DryWet")
      .constructor(&std::make_shared<DryWet, Type, Space>)
      .function("getDry", &DryWet::getDry)
      .function("getWet", &DryWet::getWet)
      .function("getMixAmount", &DryWet::getMixAmount)
      .function("getA", &DryWet::getA)
      .function("getB", &DryWet::getB);

  // Envelope
  class_<Envelope, base<Producer>>("Envelope")
      .smart_ptr<std::shared_ptr<Envelope>>("Envelope")
      .constructor(&std::make_shared<Envelope>)
      .function("getAttack", &Envelope::getAttack)
      .function("getRelease", &Envelope::getRelease)
      .function("getAttackShape", &Envelope::getAttackShape)
      .function("getReleaseShape", &Envelope::getReleaseShape)
      .function("getGate", &Envelope::getGate)
      .function("getReset", &Envelope::getReset)
      .function("getCurrentTime", &Envelope::getCurrentTime);

  // Lag
  class_<Lag, base<Transformer>>("Lag")
      .smart_ptr<std::shared_ptr<Lag>>("Lag")
      .smart_ptr_constructor<std::shared_ptr<Lag>>("Lag",
                                                   &std::make_shared<Lag, Type>)
      .function("getLagTime", &Lag::getLagTime);

  // Shaper
  class_<Shaper, base<Transformer>>("Shaper")
      .smart_ptr<std::shared_ptr<Shaper>>("Shaper")
      .constructor(&std::make_shared<Shaper, Space>)
      .function("getDrive", &Shaper::getDrive)
      .function("getMode", &Shaper::getMode)
      .function("getOutputSignal", select_overload<Sample(size_t, Sample)>(
                                       &Shaper::getOutputSignal));

  // ========== External Nodes ==========

  // MidiInput
  class_<MidiInput, base<Producer>>("MidiInput")
      .smart_ptr<std::shared_ptr<MidiInput>>("MidiInput")
      .smart_ptr_constructor<std::shared_ptr<MidiInput>>(
          "MidiInput",
          &std::make_shared<MidiInput, std::shared_ptr<MidiBuffer>, Type>)
      .function("getInputMessages", &MidiInput::getInputMessages)
      .function("getProcessFunction", &MidiInput::getProcessFunction)
      .function("setProcessFunction", &MidiInput::setProcessFunction)
      .function("processNote", &MidiInput::processNote)
      .function("processNoteOn", &MidiInput::processNoteOn)
      .function("processNoteOff", &MidiInput::processNoteOff)
      .function("processNotePressure", &MidiInput::processNotePressure)
      .function("processControl", &MidiInput::processControl)
      .function("processControlValue", &MidiInput::processControlValue)
      .function("processProgram", &MidiInput::processProgram)
      .function("processChannelPressure", &MidiInput::processChannelPressure)
      .function("processPitchBend", &MidiInput::processPitchBend)
      .function("processAllNotesOff", &MidiInput::processAllNotesOff)
      .function("processSongPositionInQuarterNotes",
                &MidiInput::processSongPositionInQuarterNotes)
      .function("processClock", &MidiInput::processClock)
      .function("processStart", &MidiInput::processStart)
      .function("processContinue", &MidiInput::processContinue)
      .function("processStop", &MidiInput::processStop);

  // MidiOutput
  class_<MidiOutput, base<Consumer>>("MidiOutput")
      .smart_ptr<std::shared_ptr<MidiOutput>>("MidiOutput")
      .smart_ptr_constructor<std::shared_ptr<MidiOutput>>(
          "MidiOutput",
          &std::make_shared<MidiOutput, std::shared_ptr<MidiBuffer>, Type>)
      .function("getOutputMessages", &MidiOutput::getOutputMessages)
      .function("getProcessFunction", &MidiInput::getProcessFunction)
      .function("setProcessFunction", &MidiInput::setProcessFunction)
      .function("processNote", &MidiInput::processNote)
      .function("processNoteOn", &MidiInput::processNoteOn)
      .function("processNoteOff", &MidiInput::processNoteOff)
      .function("processNotePressure", &MidiInput::processNotePressure)
      .function("processControl", &MidiInput::processControl)
      .function("processControlValue", &MidiInput::processControlValue)
      .function("processProgram", &MidiInput::processProgram)
      .function("processChannelPressure", &MidiInput::processChannelPressure)
      .function("processPitchBend", &MidiInput::processPitchBend)
      .function("processAllNotesOff", &MidiInput::processAllNotesOff)
      .function("processSongPositionInQuarterNotes",
                &MidiInput::processSongPositionInQuarterNotes)
      .function("processClock", &MidiInput::processClock)
      .function("processStart", &MidiInput::processStart)
      .function("processContinue", &MidiInput::processContinue)
      .function("processStop", &MidiInput::processStop);

  // ========== Filter Nodes ==========

  // Biquad
  class_<Biquad, base<Transformer>>("Biquad")
      .smart_ptr<std::shared_ptr<Biquad>>("Biquad")
      .smart_ptr_constructor<std::shared_ptr<Biquad>>("Biquad",
                                                      &std::make_shared<Biquad>)
      .function("getFrequency", &Biquad::getFrequency)
      .function("getResonance", &Biquad::getResonance)
      .function("getAmplitude", &Biquad::getAmplitude)
      .function("getMode", &Biquad::getMode)
      .function("getFrequencyResponse", &Biquad::getFrequencyResponse);

  // Crossover
  class_<Crossover, base<Node>>("Crossover")
      .smart_ptr<std::shared_ptr<Crossover>>("Crossover")
      .constructor(&std::make_shared<Crossover>)
      .function("getInput", &Crossover::getInput)
      .function("getFrequency", &Crossover::getFrequency)
      .function("getLow", &Crossover::getLow)
      .function("getHigh", &Crossover::getHigh);

  // OnePole
  class_<OnePole, base<Transformer>>("OnePole")
      .smart_ptr<std::shared_ptr<OnePole>>("OnePole")
      .constructor(&std::make_shared<OnePole, Type>)
      .function("getFrequency", &OnePole::getFrequency)
      .function("getMode", &OnePole::getMode);

  // ========== Generator Nodes ==========

  // FunctionOscillator
  class_<FunctionOscillator, base<Producer>>("FunctionOscillator")
      .smart_ptr<std::shared_ptr<FunctionOscillator>>("FunctionOscillator")
      .constructor(&std::make_shared<FunctionOscillator, Type>)
      .function("getFunction", &FunctionOscillator::getFunction)
      .function("setFunction", &FunctionOscillator::setFunction)
      .function("getPhase", &FunctionOscillator::getPhase);

  // MoorerOscillator
  class_<MoorerOscillator, base<Producer>>("MoorerOscillator")
      .smart_ptr<std::shared_ptr<MoorerOscillator>>("MoorerOscillator")
      .constructor(&std::make_shared<MoorerOscillator>)
      .function("getPhase", &MoorerOscillator::getPhase)
      .function("getIntensity", &MoorerOscillator::getIntensity)
      .function("getModulationIndex", &MoorerOscillator::getModulationIndex)
      .function("getHarmonics", &MoorerOscillator::getHarmonics)
      .function("getMode", &MoorerOscillator::getMode);

  // Noise
  class_<Noise, base<Producer>>("Noise")
      .smart_ptr<std::shared_ptr<Noise>>("Noise")
      .smart_ptr_constructor<std::shared_ptr<Noise>>("Noise",
                                                     &std::make_shared<Noise>)
      .function("getMode", &Noise::getMode);

  // Phasor
  class_<Phasor, base<Producer>>("Phasor")
      .smart_ptr<std::shared_ptr<Phasor>>("Phasor")
      .smart_ptr_constructor<std::shared_ptr<Phasor>>("Phasor",
                                                      &std::make_shared<Phasor>)
      .function("getFrequency", &Phasor::getFrequency)
      .function("getMode", &Phasor::getMode)
      .function("getReset", &Phasor::getReset);

  // SamplePlayer
  class_<SamplePlayer, base<Producer>>("SamplePlayer")
      .smart_ptr<std::shared_ptr<SamplePlayer>>("SamplePlayer")
      .constructor(&std::make_shared<SamplePlayer, Type>)
      .function("getSamples", &SamplePlayer::getSamples)
      .function("getSpeed", &SamplePlayer::getSpeed)
      .function("getStartTime", &SamplePlayer::getStartTime)
      .function("getSampleIndex", &SamplePlayer::getSampleIndex)
      .function("getInterpolation", &SamplePlayer::getInterpolation)
      .function("getGate", &SamplePlayer::getGate)
      .function("getReset", &SamplePlayer::getReset)
      .function("getCurrentTime", &SamplePlayer::getCurrentTime);

  // TableOscillator
  class_<TableOscillator, base<Producer>>("TableOscillator")
      .smart_ptr<std::shared_ptr<TableOscillator>>("TableOscillator")
      .constructor(&std::make_shared<TableOscillator, Type>)
      .function("getTables", &TableOscillator::getTables)
      .function("getPhase", &TableOscillator::getPhase)
      .function("getPosition", &TableOscillator::getPosition)
      .function("getPhaseInterpolation",
                &TableOscillator::getPhaseInterpolation)
      .function("getPositionInterpolation",
                &TableOscillator::getPositionInterpolation);

  // ========== Math Nodes ==========

  // AbsoluteValue
  class_<AbsoluteValue, base<Transformer>>("AbsoluteValue")
      .smart_ptr<std::shared_ptr<AbsoluteValue>>("AbsoluteValue")
      .constructor(&std::make_shared<AbsoluteValue, Type, Space>);

  // BooleanMask
  class_<BooleanMask, base<Transformer>>("BooleanMask")
      .smart_ptr<std::shared_ptr<BooleanMask>>("BooleanMask")
      .constructor(&std::make_shared<BooleanMask, Type, Space>)
      .function("getMask", &BooleanMask::getMask);

  // Comparison
  class_<Comparison, base<Transformer>>("Comparison")
      .smart_ptr<std::shared_ptr<Comparison>>("Comparison")
      .constructor(&std::make_shared<Comparison, Type, Space>)
      .function("getThreshold", &Comparison::getThreshold);

  // Division
  class_<Division, base<Transformer>>("Division")
      .smart_ptr<std::shared_ptr<Division>>("Division")
      .constructor(&std::make_shared<Division, Type, Space>)
      .function("getDivisor", &Division::getDivisor);

  // Floor
  class_<Floor, base<Transformer>>("Floor")
      .smart_ptr<std::shared_ptr<Floor>>("Floor")
      .constructor(&std::make_shared<Floor, Type, Space>)
      .function("getDivisor", &Floor::getDivisor);

  // ForwardFFT
  class_<ForwardFFT, base<Consumer>>("ForwardFFT")
      .smart_ptr<std::shared_ptr<ForwardFFT>>("ForwardFFT")
      .constructor(&std::make_shared<ForwardFFT>)
      .function("getMagnitude", &ForwardFFT::getMagnitude)
      .function("getPhase", &ForwardFFT::getPhase);

  // FrequencyToNote
  class_<FrequencyToNote, base<Transformer>>("FrequencyToNote")
      .smart_ptr<std::shared_ptr<FrequencyToNote>>("FrequencyToNote")
      .constructor(&std::make_shared<FrequencyToNote, Space>)
      .function("getTuningFrequency", &FrequencyToNote::getTuningFrequency);

  // Hyperbolic
  class_<Hyperbolic, base<Transformer>>("Hyperbolic")
      .smart_ptr<std::shared_ptr<Hyperbolic>>("Hyperbolic")
      .constructor(&std::make_shared<Hyperbolic, Space>)
      .function("getMode", &Hyperbolic::getMode);

  // Identity
  class_<Identity, base<Transformer>>("Identity")
      .smart_ptr<std::shared_ptr<Identity>>("Identity")
      .constructor(&std::make_shared<Identity, Type, Space>)
      .constructor(&std::make_shared<Identity, Type, Type, Space>)
      .constructor(&std::make_shared<Identity, Type, Type, Space, Space>);

  // InverseFFT
  class_<InverseFFT, base<Producer>>("InverseFFT")
      .smart_ptr<std::shared_ptr<InverseFFT>>("InverseFFT")
      .constructor(&std::make_shared<InverseFFT>)
      .function("getMagnitude", &InverseFFT::getMagnitude)
      .function("getPhase", &InverseFFT::getPhase);

  // Logarithm
  class_<Logarithm, base<Transformer>>("Logarithm")
      .smart_ptr<std::shared_ptr<Logarithm>>("Logarithm")
      .constructor(&std::make_shared<Logarithm, Space>)
      .function("getBase", &Logarithm::getBase);

  // Modulo
  class_<Modulo, base<Transformer>>("Modulo")
      .smart_ptr<std::shared_ptr<Modulo>>("Modulo")
      .constructor(&std::make_shared<Modulo, Type, Space>)
      .function("getDivisor", &Modulo::getDivisor);

  // Multiplication
  class_<Multiplication, base<Transformer>>("Multiplication")
      .smart_ptr<std::shared_ptr<Multiplication>>("Multiplication")
      .constructor(&std::make_shared<Multiplication, Type, Space>)
      .function("getFactor", &Multiplication::getFactor);

  // Negative
  class_<Negative, base<Transformer>>("Negative")
      .smart_ptr<std::shared_ptr<Negative>>("Negative")
      .constructor(&std::make_shared<Negative, Type, Space>);

  // NoteToFrequency
  class_<NoteToFrequency, base<Transformer>>("NoteToFrequency")
      .smart_ptr<std::shared_ptr<NoteToFrequency>>("NoteToFrequency")
      .constructor(&std::make_shared<NoteToFrequency, Space>)
      .function("getTuningFrequency", &NoteToFrequency::getTuningFrequency);

  // NotGate
  class_<NotGate, base<Transformer>>("NotGate")
      .smart_ptr<std::shared_ptr<NotGate>>("NotGate")
      .constructor(&std::make_shared<NotGate, Space>);

  // Power
  class_<Power, base<Transformer>>("Power")
      .smart_ptr<std::shared_ptr<Power>>("Power")
      .constructor(&std::make_shared<Power, Space>)
      .function("getExponent", &Power::getExponent);

  // Reciprocal
  class_<Reciprocal, base<Transformer>>("Reciprocal")
      .smart_ptr<std::shared_ptr<Reciprocal>>("Reciprocal")
      .constructor(&std::make_shared<Reciprocal, Type, Space>);

  // Trigonometric
  class_<Trigonometric, base<Transformer>>("Trigonometric")
      .smart_ptr<std::shared_ptr<Trigonometric>>("Trigonometric")
      .constructor(&std::make_shared<Trigonometric, Space>)
      .function("getMode", &Trigonometric::getMode);

  // ========== Trigger Nodes ==========

  // ClockTrigger
  class_<ClockTrigger, base<Producer>>("ClockTrigger")
      .smart_ptr<std::shared_ptr<ClockTrigger>>("ClockTrigger")
      .constructor(&std::make_shared<ClockTrigger>)
      .function("getInterval", &ClockTrigger::getInterval)
      .function("getDelayTime", &ClockTrigger::getDelayTime)
      .function("getReset", &ClockTrigger::getReset)
      .function("getCurrentTime", &ClockTrigger::getCurrentTime);

  // Differentiator
  class_<Differentiator, base<Transformer>>("Differentiator")
      .smart_ptr<std::shared_ptr<Differentiator>>("Differentiator")
      .constructor(&std::make_shared<Differentiator, Type>)
      .function("getGate", &Differentiator::getGate)
      .function("getReset", &Differentiator::getReset);

  // Integrator
  class_<Integrator, base<Transformer>>("Integrator")
      .smart_ptr<std::shared_ptr<Integrator>>("Integrator")
      .constructor(&std::make_shared<Integrator, Type>)
      .function("getGate", &Integrator::getGate)
      .function("getReset", &Integrator::getReset);

  // OnOff
  class_<OnOff, base<Producer>>("OnOff")
      .smart_ptr<std::shared_ptr<OnOff>>("OnOff")
      .smart_ptr_constructor<std::shared_ptr<OnOff>>("OnOff",
                                                     &std::make_shared<OnOff>)
      .function("getOnTrigger", &OnOff::getOnTrigger)
      .function("getOffTrigger", &OnOff::getOffTrigger);

  // ResetTrigger
  class_<ResetTrigger, base<Producer>>("ResetTrigger")
      .smart_ptr<std::shared_ptr<ResetTrigger>>("ResetTrigger")
      .constructor(&std::make_shared<ResetTrigger>)
      .function("reset", select_overload<void()>(&ResetTrigger::reset))
      .function("resetChannel",
                select_overload<void(size_t)>(&ResetTrigger::reset));

  // SampleAndHold
  class_<SampleAndHold, base<Transformer>>("SampleAndHold")
      .smart_ptr<std::shared_ptr<SampleAndHold>>("SampleAndHold")
      .constructor(&std::make_shared<SampleAndHold, Type>)
      .function("getGate", &SampleAndHold::getGate)
      .function("getReset", &SampleAndHold::getReset);

  // Sequencer
  class_<Sequencer, base<Producer>>("Sequencer")
      .smart_ptr<std::shared_ptr<Sequencer>>("Sequencer")
      .constructor(&std::make_shared<Sequencer, Type, Space>)
      .function("getSequences", &Sequencer::getSequences)
      .function("getSequenceIndex", &Sequencer::getSequenceIndex)
      .function("getPositionIndex", &Sequencer::getPositionIndex);

  // TriggerHold
  class_<TriggerHold, base<Transformer>>("TriggerHold")
      .smart_ptr<std::shared_ptr<TriggerHold>>("TriggerHold")
      .constructor(&std::make_shared<TriggerHold>)
      .function("getHoldTime", &TriggerHold::getHoldTime)
      .function("getCurrentTime", &TriggerHold::getCurrentTime);

  // ========== Variable Nodes ==========

  // BufferDuration
  class_<BufferDuration, base<Producer>>("BufferDuration")
      .smart_ptr<std::shared_ptr<BufferDuration>>("BufferDuration")
      .constructor(&std::make_shared<BufferDuration>);

  // BufferRate
  class_<BufferRate, base<Producer>>("BufferRate")
      .smart_ptr<std::shared_ptr<BufferRate>>("BufferRate")
      .constructor(&std::make_shared<BufferRate>);

  // SampleDuration
  class_<SampleDuration, base<Producer>>("SampleDuration")
      .smart_ptr<std::shared_ptr<SampleDuration>>("SampleDuration")
      .constructor(&std::make_shared<SampleDuration>);

  // SampleRate
  class_<SampleRate, base<Producer>>("SampleRate")
      .smart_ptr<std::shared_ptr<SampleRate>>("SampleRate")
      .constructor(&std::make_shared<SampleRate>);

  // ========== Utility Classes ==========

  // NormalizedFFT
  class_<NormalizedFFT>("NormalizedFFT")
      .smart_ptr<std::shared_ptr<NormalizedFFT>>("NormalizedFFT")
      .constructor(&std::make_shared<NormalizedFFT>)
      .function("setup", &NormalizedFFT::setup)
      .function("getSize", &NormalizedFFT::getSize)
      .function("getComplexSize", &NormalizedFFT::getComplexSize)
      .function("toRealImaginary", &NormalizedFFT::toRealImaginary,
                allow_raw_pointers())
      .function("fromRealImaginary", &NormalizedFFT::fromRealImaginary,
                allow_raw_pointers())
      .function("toMagnitudePhase", &NormalizedFFT::toMagnitudePhase,
                allow_raw_pointers())
      .function("fromMagnitudePhase", &NormalizedFFT::fromMagnitudePhase,
                allow_raw_pointers());
}
