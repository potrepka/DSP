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
#include "Nodes/Channel/Spread.h"
#include "Nodes/Channel/StereoPanner.h"

#include "Nodes/Delays/Convolver.h"
#include "Nodes/Delays/VariableDelay.h"

#include "Nodes/Dynamics/Clipper.h"
#include "Nodes/Dynamics/CompressorGate.h"
#include "Nodes/Dynamics/DryWet.h"
#include "Nodes/Dynamics/Envelope.h"
#include "Nodes/Dynamics/Lag.h"
#include "Nodes/Dynamics/Shaper.h"

#include "Nodes/External/MidiInput.h"
#include "Nodes/External/MidiOutput.h"

#include "Nodes/Filters/Biquad.h"
#include "Nodes/Filters/Crossover.h"
#include "Nodes/Filters/OnePole.h"

#include "Nodes/Generators/FunctionOscillator.h"
#include "Nodes/Generators/MoorerOscillator.h"
#include "Nodes/Generators/Noise.h"
#include "Nodes/Generators/Phasor.h"
#include "Nodes/Generators/SamplePlayer.h"
#include "Nodes/Generators/TableOscillator.h"

#include "Nodes/Math/AbsoluteValue.h"
#include "Nodes/Math/BooleanMask.h"
#include "Nodes/Math/Comparison.h"
#include "Nodes/Math/Division.h"
#include "Nodes/Math/Floor.h"
#include "Nodes/Math/ForwardFFT.h"
#include "Nodes/Math/FrequencyToNote.h"
#include "Nodes/Math/Hyperbolic.h"
#include "Nodes/Math/Identity.h"
#include "Nodes/Math/InverseFFT.h"
#include "Nodes/Math/Logarithm.h"
#include "Nodes/Math/Modulo.h"
#include "Nodes/Math/Multiplication.h"
#include "Nodes/Math/Negative.h"
#include "Nodes/Math/NotGate.h"
#include "Nodes/Math/NoteToFrequency.h"
#include "Nodes/Math/Power.h"
#include "Nodes/Math/Reciprocal.h"
#include "Nodes/Math/Trigonometric.h"

#include "Nodes/Trigger/ClockTrigger.h"
#include "Nodes/Trigger/Differentiator.h"
#include "Nodes/Trigger/Integrator.h"
#include "Nodes/Trigger/OnOff.h"
#include "Nodes/Trigger/ResetTrigger.h"
#include "Nodes/Trigger/SampleAndHold.h"
#include "Nodes/Trigger/Sequencer.h"
#include "Nodes/Trigger/TriggerHold.h"

#include "Nodes/Variables/BufferDuration.h"
#include "Nodes/Variables/BufferRate.h"
#include "Nodes/Variables/SampleDuration.h"
#include "Nodes/Variables/SampleRate.h"
