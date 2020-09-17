#pragma once

#if !defined(DSP_NO_ENGINE) && !defined(DSP_USE_JUCE)
#define DSP_USE_ENGINE
#endif

#ifdef DSP_USE_ENGINE
#include "Engine.h"
#else
#include "NodeProcessor.h"
#endif

#include "Nodes/Analyzers/Recorder.h"

#include "Nodes/Channel/ChannelMerger.h"
#include "Nodes/Channel/ChannelSplitter.h"
#include "Nodes/Channel/MidSide.h"
#include "Nodes/Channel/StereoPanner.h"

#include "Nodes/Delays/Convolver.h"
#include "Nodes/Delays/VariableDelay.h"

#include "Nodes/Dynamics/CompressorGate.h"
#include "Nodes/Dynamics/Distortion.h"
#include "Nodes/Dynamics/DryWet.h"
#include "Nodes/Dynamics/Envelope.h"
#include "Nodes/Dynamics/HardClip.h"

#include "Nodes/External/MidiInput.h"
#include "Nodes/External/MidiOutput.h"

#include "Nodes/Filters/Biquad.h"
#include "Nodes/Filters/Crossover.h"
#include "Nodes/Filters/OnePole.h"

#include "Nodes/Generators/FunctionOscillator.h"
#include "Nodes/Generators/MoorerOscillator.h"
#include "Nodes/Generators/Phasor.h"
#include "Nodes/Generators/PinkNoise.h"
#include "Nodes/Generators/SamplePlayer.h"
#include "Nodes/Generators/TableOscillator.h"
#include "Nodes/Generators/WhiteNoise.h"

#include "Nodes/Math/AbsoluteValue.h"
#include "Nodes/Math/Comparison.h"
#include "Nodes/Math/Exp2.h"
#include "Nodes/Math/Floor.h"
#include "Nodes/Math/ForwardFFT.h"
#include "Nodes/Math/InverseFFT.h"
#include "Nodes/Math/Log2.h"
#include "Nodes/Math/LogarithmicToRatio.h"
#include "Nodes/Math/Modulo.h"
#include "Nodes/Math/Multiply.h"
#include "Nodes/Math/MultiplyHertzSeconds.h"
#include "Nodes/Math/NotGate.h"
#include "Nodes/Math/OneOver.h"
#include "Nodes/Math/PassThrough.h"

#include "Nodes/Trigger/Counter.h"
#include "Nodes/Trigger/OnOff.h"
#include "Nodes/Trigger/ResetTrigger.h"
#include "Nodes/Trigger/SampleAndHold.h"
#include "Nodes/Trigger/Sequencer.h"
#include "Nodes/Trigger/BeatTrigger.h"

#include "Nodes/Variables/BufferDuration.h"
#include "Nodes/Variables/BufferRate.h"
#include "Nodes/Variables/SampleDuration.h"
#include "Nodes/Variables/SampleRate.h"
