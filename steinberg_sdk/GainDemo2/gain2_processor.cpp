//------------------------------------------------------------------------
// Copyright(c) 2023 redetach.
//------------------------------------------------------------------------

#include "gain2_processor.h"
#include "gain2_cids.h"
#include "params.h"
#include "base/source/fstreamer.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include "public.sdk/source/vst/vstaudioprocessoralgo.h"
#include "base/source/fstreamer.h"

using namespace Steinberg;

namespace redetach {
//------------------------------------------------------------------------
// GainDemo2Processor
//------------------------------------------------------------------------
GainDemo2Processor::GainDemo2Processor ()
{
	//--- set the wanted controller for our processor
	setControllerClass (kGainDemo2ControllerUID);
}

//------------------------------------------------------------------------
GainDemo2Processor::~GainDemo2Processor ()
{}

//------------------------------------------------------------------------
tresult PLUGIN_API GainDemo2Processor::initialize (FUnknown* context)
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
tresult PLUGIN_API GainDemo2Processor::terminate ()
{
	// Here the Plug-in will be de-instantiated, last possibility to remove some memory!
	
	//---do not forget to call parent ------
	return AudioEffect::terminate ();
}

//------------------------------------------------------------------------
tresult PLUGIN_API GainDemo2Processor::setActive (TBool state)
{
	//--- called when the Plug-in is enable/disable (On/Off) -----
	return AudioEffect::setActive (state);
}

//------------------------------------------------------------------------
tresult PLUGIN_API GainDemo2Processor::process (Vst::ProcessData& data)
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
					case kGainId:
						fGain = (float) value;
						break;
					}
				}
			}
		}
	}
	
	//--- Here you have to implement your processing
	if (data.numInputs == 0 || data.numSamples == 0)
		return kResultOk;

	int32 numChannels = data.inputs[0].numChannels;
	int32 sampleCount = data.numSamples;
	uint32 sampleFramesSize = getSampleFramesSizeInBytes(processSetup, sampleCount);
	void** in = getChannelBuffersPointer(processSetup, data.inputs[0]);
	void** out = getChannelBuffersPointer(processSetup, data.inputs[0]);

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

	for (int32 ch = 0; ch < numChannels; ch++)
	{
		Vst::Sample32* ptrIn = (Vst::Sample32*)in[ch];
		Vst::Sample32* ptrOut = (Vst::Sample32*)out[ch];
		Vst::Sample32 tmp;
		for (int32 i = 0; i < sampleCount; i++)
		{
			tmp = *ptrIn * fGain;
			*ptrOut = tmp;
			ptrIn++;
			ptrOut++;
		}
	}

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API GainDemo2Processor::setupProcessing (Vst::ProcessSetup& newSetup)
{
	//--- called before any processing ----
	return AudioEffect::setupProcessing (newSetup);
}

//------------------------------------------------------------------------
tresult PLUGIN_API GainDemo2Processor::canProcessSampleSize (int32 symbolicSampleSize)
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
tresult PLUGIN_API GainDemo2Processor::setState (IBStream* state)
{
	if (!state)
		return kResultFalse;

	// called when we load a preset, the model has to be reloaded
	IBStreamer streamer (state, kLittleEndian);
	float savedParam1 = 0.f;
	if (streamer.readFloat(savedParam1) == false)
		return kResultFalse;

	fGain = savedParam1;
	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API GainDemo2Processor::getState (IBStream* state)
{
	// here we need to save the model
	float paramToSave1 = fGain;
	IBStreamer streamer (state, kLittleEndian);
	streamer.writeFloat(paramToSave1);
	return kResultOk;
}

//------------------------------------------------------------------------
} // namespace redetach
