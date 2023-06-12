//------------------------------------------------------------------------
// Copyright(c) 2023 redetach.
//------------------------------------------------------------------------

#include "gain_processor.h"
#include "gain_cids.h"
#include "base/source/fstreamer.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include "public.sdk/source/vst/vstaudioprocessoralgo.h"

using namespace Steinberg;

namespace redetach {
//------------------------------------------------------------------------
// GainDemoProcessor
//------------------------------------------------------------------------
GainDemoProcessor::GainDemoProcessor ()
{
	//--- set the wanted controller for our processor
	setControllerClass (kGainDemoControllerUID);
}

//------------------------------------------------------------------------
GainDemoProcessor::~GainDemoProcessor ()
{}

//------------------------------------------------------------------------
tresult PLUGIN_API GainDemoProcessor::initialize (FUnknown* context)
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
tresult PLUGIN_API GainDemoProcessor::terminate ()
{
	// Here the Plug-in will be de-instantiated, last possibility to remove some memory!
	
	//---do not forget to call parent ------
	return AudioEffect::terminate ();
}

//------------------------------------------------------------------------
tresult PLUGIN_API GainDemoProcessor::setActive (TBool state)
{
	//--- called when the Plug-in is enable/disable (On/Off) -----
	return AudioEffect::setActive (state);
}

//------------------------------------------------------------------------
tresult PLUGIN_API GainDemoProcessor::process (Vst::ProcessData& data)
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
				if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultFalse)
				{
					switch (paramQueue->getParameterId())
					{
					case GainParams::kGainId:
						mGain = value;
						break;
					}

				}
			}
		}
	}
	
	//--- Here you have to implement your processing
	if (data.numInputs == 0 || data.numSamples == 0) 
	{
		return kResultOk;
	}

	int32 numChannels = data.inputs[0].numChannels;

	// helper functions from vstaudioprocessoralgo.h
	uint32 sampleFramesSize = getSampleFramesSizeInBytes(processSetup, data.numSamples);
	void** in = getChannelBuffersPointer(processSetup, data.inputs[0]);
	void** out = getChannelBuffersPointer(processSetup, data.outputs[0]);

	// crude silence flags check
	//data.outputs[0].silenceFlags = 0;
	// thorough check of silence flags, needs vstaudioprocessoralgo.h helpers
	if (data.inputs[0].silenceFlags != 0)
	{
		data.outputs[0].silenceFlags = data.inputs[0].silenceFlags;
		for (int32 i = 0; i < numChannels; i++)
		{
			if (in[i] != out[i])
			{
				memset(out[i], 0, sampleFramesSize);
			}
		}
		return kResultOk;
	}

	// processing block
	float gain = mGain * 5.0;
	for (int32 ch = 0; ch < numChannels; ch++)
	{
		int32 samples = data.numSamples;
		Vst::Sample32* ptrIn = (Vst::Sample32*)in[ch];
		Vst::Sample32* ptrOut = (Vst::Sample32*)out[ch];
		Vst::Sample32 tmp;
		while (--samples >= 0)
		{
			tmp = (*ptrIn++) * gain; // apply gain
			(*ptrOut++) = tmp;
		}
	}

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API GainDemoProcessor::setupProcessing (Vst::ProcessSetup& newSetup)
{
	//--- called before any processing ----
	return AudioEffect::setupProcessing (newSetup);
}

//------------------------------------------------------------------------
tresult PLUGIN_API GainDemoProcessor::canProcessSampleSize (int32 symbolicSampleSize)
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
tresult PLUGIN_API GainDemoProcessor::setState (IBStream* state)
{
	if (!state)
		return kResultFalse;

	// called when we load a preset, the model has to be reloaded
	IBStreamer streamer (state, kLittleEndian);
	float savedParam1 = 0.f;
	if (streamer.readFloat(savedParam1) == false)
	{
		return kResultFalse;
	}
	mGain = savedParam1;
	
	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API GainDemoProcessor::getState (IBStream* state)
{
	float toSaveParam1 = mGain;
	// here we need to save the model
	IBStreamer streamer (state, kLittleEndian);
	streamer.writeFloat(toSaveParam1);

	return kResultOk;
}

//------------------------------------------------------------------------
} // namespace redetach
