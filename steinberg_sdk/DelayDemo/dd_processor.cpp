//------------------------------------------------------------------------
// Copyright(c) 2023 redetach.
//------------------------------------------------------------------------

#include "dd_processor.h"
#include "dd_cids.h"
#include "pluginterfaces/base/ustring.h"
#include "pluginterfaces/base/ibstream.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include <algorithm>
#include <cstdlib>
#include "base/source/fstreamer.h"

using namespace Steinberg;

namespace redetach {
//------------------------------------------------------------------------
// DelayDemoProcessor
//------------------------------------------------------------------------
DelayDemoProcessor::DelayDemoProcessor ()
: mDelay (1)
, mBuffer (nullptr)
, mBufferPos (0)
, mBypass (false)
{
	setControllerClass (kDelayDemoControllerUID);
}

//------------------------------------------------------------------------
tresult PLUGIN_API DelayDemoProcessor::initialize (FUnknown* context)
{
	tresult result = AudioEffect::initialize (context);
	if (result == kResultOk)
	{
		addAudioInput(STR16("AudioInput"), Vst::SpeakerArr::kStereo);
		addAudioOutput(STR16("AudioOutput"), Vst::SpeakerArr::kStereo);
	}

	return result;
}

//------------------------------------------------------------------------
tresult PLUGIN_API DelayDemoProcessor::setBusArrangements (Vst::SpeakerArrangement* inputs, int32 numIns, Vst::SpeakerArrangement* outputs, int32 numOuts)
{
	if (numIns == 1 && numOuts == 1 && inputs[0] == outputs[0])
		return AudioEffect::setBusArrangements (inputs, numIns, outputs, numOuts);

	return kResultFalse;
}


//------------------------------------------------------------------------
tresult PLUGIN_API DelayDemoProcessor::setActive (TBool state)
{
	Vst::SpeakerArrangement arr;
	if (getBusArrangement(Vst::kOutput, 0, arr) != kResultTrue)
		return kResultFalse;

	int32 numChannels = Vst::SpeakerArr::getChannelCount(arr);
	if (numChannels == 0)
		return kResultFalse;

	if (state)
	{
		mBuffer = (float**)std::malloc(numChannels*sizeof(float*));
		size_t size = (size_t)(processSetup.sampleRate*sizeof(float)+0.5);
		for (int32 ch = 0; ch < numChannels; ch++)
		{
			mBuffer[ch] = (float*)std::malloc(size);
			if (mBuffer[ch])
			{
				memset(mBuffer[ch], 0, size);
			}
		}
		mBufferPos = 0;
	}
	else
	{
		if (mBuffer)
		{
			for (int32 ch = 0; ch < numChannels; ch++)
			{
				std::free(mBuffer[ch]);
			}
			std::free(mBuffer);
			mBuffer = nullptr;
		}
	}

	return AudioEffect::setActive (state);
}

//------------------------------------------------------------------------
tresult PLUGIN_API DelayDemoProcessor::process (Vst::ProcessData& data)
{
	//--- First : Read inputs parameter changes-----------

    if (data.inputParameterChanges)
    {
        int32 numParamsChanged = data.inputParameterChanges->getParameterCount ();
        for (int32 index = 0; index < numParamsChanged; index++)
        {
			Vst::IParamValueQueue* paramQueue = data.inputParameterChanges->getParameterData(index);
            if (paramQueue)
            {
                Vst::ParamValue value;
                int32 sampleOffset;
                int32 numPoints = paramQueue->getPointCount ();
                switch (paramQueue->getParameterId ())
                {
				case kDelayId:
					if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue)
					{
						mDelay = value;
					}
					break;
				case kBypassId:
					if (paramQueue->getPoint(numPoints = 1, sampleOffset, value) == kResultTrue)
					{
						mBypass = (value > 0.5f);
					}
					break;
				}
			}
		}
	}

	if (data.numSamples > 0)
	{
		Vst::SpeakerArrangement arr;
		getBusArrangement(Vst::kOutput, 0 ,arr);
		int32 numChannels = Vst::SpeakerArr::getChannelCount(arr);
		int32 delayInSamples = std::max<int32>(1, (int32)(mDelay * processSetup.sampleRate));
		for (int32 ch = 0; ch < numChannels; ch++)
		{
			float* inputChannel = data.inputs[0].channelBuffers32[ch];
			float* outputChannel = data.outputs[0].channelBuffers32[ch];
			int32 tempBufferPos = mBufferPos;
			for (int32 s = 0; s < data.numSamples; s++)
			{
				float tempSample = inputChannel[s];
				outputChannel[s] = mBuffer[ch][tempBufferPos];
				mBuffer[ch][tempBufferPos] = tempSample;
				tempBufferPos++;
				if (tempBufferPos >= delayInSamples)
					tempBufferPos = 0;
			}
		}
		mBufferPos += data.numSamples;
		while (delayInSamples && mBufferPos >= delayInSamples)
			mBufferPos -= delayInSamples;
	}

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API DelayDemoProcessor::setState (IBStream* state)
{
	if (!state)
		return kResultFalse;

	// called when we load a preset, the model has to be reloaded
	IBStreamer streamer (state, kLittleEndian);
	float savedDelay = 0.f;
	if (streamer.readFloat(savedDelay) == false)
		return kResultFalse;

	int32 savedBypass = 0;
	if (streamer.readInt32(savedBypass) == false) { /* could be an older version, continue */ }

	mDelay = savedDelay;
	mBypass = savedBypass > 0;

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API DelayDemoProcessor::getState (IBStream* state)
{
	// here we need to save the model
	IBStreamer streamer (state, kLittleEndian);
	streamer.writeFloat(mDelay);
	streamer.writeInt32(mBypass ? 1 : 0);

	return kResultOk;
}

//------------------------------------------------------------------------
} // namespace redetach
