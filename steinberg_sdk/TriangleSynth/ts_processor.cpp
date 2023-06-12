//------------------------------------------------------------------------
// Copyright(c) 2023 redetach.
//------------------------------------------------------------------------

#include "ts_processor.h"
#include "ts_cids.h"
#include "public.sdk/source/vst/hosting/eventlist.h"
#include "base/source/fstreamer.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"

using namespace Steinberg;

namespace redetach {
//------------------------------------------------------------------------
// TriangleSynthProcessor
//------------------------------------------------------------------------
TriangleSynthProcessor::TriangleSynthProcessor ()
{
	//--- set the wanted controller for our processor
	setControllerClass (kTriangleSynthControllerUID);
}

//------------------------------------------------------------------------
TriangleSynthProcessor::~TriangleSynthProcessor ()
{}

//------------------------------------------------------------------------
tresult PLUGIN_API TriangleSynthProcessor::initialize (FUnknown* context)
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
tresult PLUGIN_API TriangleSynthProcessor::terminate ()
{
	// Here the Plug-in will be de-instantiated, last possibility to remove some memory!
	
	//---do not forget to call parent ------
	return AudioEffect::terminate ();
}

//------------------------------------------------------------------------
tresult PLUGIN_API TriangleSynthProcessor::setActive (TBool state)
{
	//--- called when the Plug-in is enable/disable (On/Off) -----
	return AudioEffect::setActive (state);
}

//------------------------------------------------------------------------
tresult PLUGIN_API TriangleSynthProcessor::process (Vst::ProcessData& data)
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
				paramQueue->getPoint(numPoints - 1, sampleOffset, value);
                switch (paramQueue->getParameterId ())
                {
				case kOsc1:
					fOsc1 = (float)value;
					break;
				}
			}
		}
	}
	
	//--- Here you have to implement your processing
	Vst::IEventList* events = data.inputEvents;
	if (events != NULL)
	{
		int32 numEvent = events->getEventCount();
		for (int32 i = 0; i < numEvent; i++)
		{
			Vst::Event event;
			if (events->getEvent(i, event) == kResultOk)
			{
				switch (event.type)
				{
				case Vst::Event::kNoteOnEvent:
					fFrequency = 440.0f * powf(2.0f, (float)(event.noteOn.pitch - 69) / 12.f);
					fDeltaAngle = PI2 * fFrequency / data.processContext->sampleRate;
					fVolume = 0.3f;
					fOsc1Phase = 0.f;
					break;
				case Vst::Event::kNoteOffEvent:
					fVolume = 0.f;
					break;
				}
			}
		}
	}

	Vst::Sample32* outL = data.outputs[0].channelBuffers32[0];
	Vst::Sample32* outR = data.outputs[0].channelBuffers32[1];
	for (int32 i = 0; i < data.numSamples; i++)
	{
		float Osc1val = 2.f * (fOsc1Phase / PI2) - 1.f;
		if (Osc1val < 0.0) 
		{
			Osc1val = -Osc1val;
		}
		Osc1val = 2.f * (Osc1val - 0.5f);
		outL[i] = fOsc1 * Osc1val;
		outL[i] *= fVolume;
		outR[i] = outL[i];
		fOsc1Phase += fDeltaAngle;
	}

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API TriangleSynthProcessor::setupProcessing (Vst::ProcessSetup& newSetup)
{
	//--- called before any processing ----
	return AudioEffect::setupProcessing (newSetup);
}

//------------------------------------------------------------------------
tresult PLUGIN_API TriangleSynthProcessor::canProcessSampleSize (int32 symbolicSampleSize)
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
tresult PLUGIN_API TriangleSynthProcessor::setState (IBStream* state)
{
	// called when we load a preset, the model has to be reloaded
	IBStreamer streamer (state, kLittleEndian);
	
	float fval;
	if (streamer.readFloat(fval) == false)
	{
		return kResultFalse;
	}

	fOsc1 = fval;

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API TriangleSynthProcessor::getState (IBStream* state)
{
	// here we need to save the model
	IBStreamer streamer (state, kLittleEndian);

	streamer.writeFloat(fOsc1);

	return kResultOk;
}

//------------------------------------------------------------------------
} // namespace redetach
