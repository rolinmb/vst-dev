//------------------------------------------------------------------------
// Copyright(c) 2023 Rolin Blake.
//------------------------------------------------------------------------
#include <windows.h>

#include "fuzzprocessor.h"
#include "fuzzcids.h"
#include "params.h"
#include "base/source/fstreamer.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"

using namespace Steinberg;

namespace MyCompanyName {
//------------------------------------------------------------------------
// FuzzProcessor
//------------------------------------------------------------------------
FuzzProcessor::FuzzProcessor ()
{
	//--- set the wanted controller for our processor
	setControllerClass (kFuzzControllerUID);
}

//------------------------------------------------------------------------
FuzzProcessor::~FuzzProcessor ()
{}

//------------------------------------------------------------------------
tresult PLUGIN_API FuzzProcessor::initialize (FUnknown* context)
{
	// Here the Plug-in will be instantiated
	
	//---always initialize the parent-------
	tresult result = AudioEffect::initialize (context);
	// if everything Ok, continue
	if (result != kResultOk)
	{
		return result;
	}

	//--- create Audio IO ------
	addAudioInput (STR16 ("Stereo In"), Steinberg::Vst::SpeakerArr::kStereo);
	addAudioOutput (STR16 ("Stereo Out"), Steinberg::Vst::SpeakerArr::kStereo);

	/* If you don't need an event bus, you can remove the next line */
	addEventInput (STR16 ("Event In"), 1);

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API FuzzProcessor::terminate ()
{
	// Here the Plug-in will be de-instantiated, last possibility to remove some memory!
	
	//---do not forget to call parent ------
	return AudioEffect::terminate ();
}

//------------------------------------------------------------------------
tresult PLUGIN_API FuzzProcessor::setActive (TBool state)
{
	//--- called when the Plug-in is enable/disable (On/Off) -----
	return AudioEffect::setActive (state);
}

//------------------------------------------------------------------------
tresult PLUGIN_API FuzzProcessor::process (Vst::ProcessData& data)
{
	//--- First : Read inputs parameter changes-----------

    if (data.inputParameterChanges)
    {
        int32 numParamsChanged = data.inputParameterChanges->getParameterCount ();
        for (int32 index = 0; index < numParamsChanged; index++)
        {
            if (auto* paramQueue = data.inputParameterChanges->getParameterData (index))
            {
                Vst::ParamValue value;
                int32 sampleOffset;
                int32 numPoints = paramQueue->getPointCount ();
				if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue)
				{
					switch (paramQueue->getParameterId())
					{
					case kFuzzId:
						fFuzz = (float) value;
						break;
					}
				}
			}
		}
	}
	
	//--- Here you have to implement your processing
	if (data.numInputs == 0 || data.numOutputs == 0) 
	{
		return kResultOk;
	}
	// Get number of channels and pointers to I/O buffers/streams
	int32 numChannels = data.inputs[0].numChannels;
	Vst::Sample32** in = data.inputs[0].channelBuffers32;
	Vst::Sample32** out = data.outputs[0].channelBuffers32;
	for (int32 ch = 0; ch < numChannels; ch++) 
	{
		Vst::Sample32* pIn = in[ch];   //Pointer to input channel from buffer pointer
		Vst::Sample32* pOut = out[ch]; //Pointer to output channel from buffer pointer
		Vst::Sample32 tmp;             //Temp variable to hold result before clipping and save to val at pOut addr. (*pOut = tmp)
		for (int32 i = 0; i < data.numSamples; i++) 
		{
			tmp = *pIn + *pIn * 20.0 * fFuzz; //Apply fuzz, save result in tmp
			if (tmp > 0.8) {tmp = 0.8;}       //Clipping amp. on peaks
			if (tmp < -0.8) {tmp = -0.8;}     //Clipping amp. on troughs
			*pOut = tmp;
			pIn++;
			pOut++;
		}
	}

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API FuzzProcessor::setupProcessing (Vst::ProcessSetup& newSetup)
{
	//--- called before any processing ----
	return AudioEffect::setupProcessing (newSetup);
}

//------------------------------------------------------------------------
tresult PLUGIN_API FuzzProcessor::canProcessSampleSize (int32 symbolicSampleSize)
{
	// by default kSample32 is supported
	if (symbolicSampleSize == Vst::kSample32)
		return kResultTrue;

	// disable the following comment if your processing support kSample64
	/* if (symbolicSampleSize == Vst::kSample64)
		return kResultTrue; */

	return kResultFalse;
}

//------------------------------------------------------------------------
tresult PLUGIN_API FuzzProcessor::setState (IBStream* state)
{
	// called when we load a preset, the model has to be reloaded
	IBStreamer streamer (state, kLittleEndian);
	
	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API FuzzProcessor::getState (IBStream* state)
{
	// here we need to save the model
	IBStreamer streamer (state, kLittleEndian);

	return kResultOk;
}

//------------------------------------------------------------------------
} // namespace MyCompanyName
