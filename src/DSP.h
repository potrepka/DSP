#pragma once

#include "Engine.h"

// Buffer
#include "BufferRecorder.h"
#include "SamplePlayer.h"
#include "SampleRecorder.h"
#include "TableOscillator.h"

// Dynamics
#include "AHR.h"
#include "CompressorGate.h"
#include "GainComputer.h"
#include "GainEnvelope.h"
#include "GainScale.h"
#include "HardClip.h"
#include "PowerDistortion.h"

// Filters
#include "Biquad.h"
#include "Convolver.h"
#include "OnePole.h"
#include "VariableDelay.h"

// Math
#include "AbsoluteValue.h"
#include "AndGate.h"
#include "EqualTo.h"
#include "Floor.h"
#include "GreaterThan.h"
#include "LessThan.h"
#include "Maximum.h"
#include "Minimum.h"
#include "Modulo.h"
#include "Multiply.h"
#include "MultiplyHertzSeconds.h"
#include "Negative.h"
#include "NotGate.h"
#include "OneOver.h"
#include "OrGate.h"
#include "XorGate.h"

// Noise
#include "PinkNoise.h"
#include "WhiteNoise.h"

// Phase
#include "Phasor.h"
#include "Trigger.h"

// Transforms
#include "Base2Exp.h"
#include "Base2Log.h"
#include "BipolarToUnipolar.h"
#include "CartesianToPolar.h"
#include "FFT.h"
#include "HertzToSeconds.h"
#include "IFFT.h"
#include "PolarToCartesian.h"
#include "SecondsToHertz.h"
#include "ToBinary.h"
#include "ToInteger.h"
#include "UnipolarToBipolar.h"

// Utility
#include "Audio.h"
#include "ChannelMix.h"
#include "DryWetMix.h"
#include "OnOff.h"
#include "PassThrough.h"
#include "SampleAndHold.h"
#include "Sequencer.h"
#include "StereoPanner.h"

// Variables
#include "BufferDuration.h"
#include "BufferRate.h"
#include "SampleDuration.h"
#include "SampleRate.h"
