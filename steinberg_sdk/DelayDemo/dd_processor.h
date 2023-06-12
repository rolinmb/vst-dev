//------------------------------------------------------------------------
// Copyright(c) 2023 redetach.
//------------------------------------------------------------------------

#pragma once

#include "public.sdk/source/vst/vstaudioeffect.h"

namespace redetach {

//------------------------------------------------------------------------
//  DelayDemoProcessor
//------------------------------------------------------------------------
class DelayDemoProcessor : public Steinberg::Vst::AudioEffect
{
public:
	DelayDemoProcessor ();

	/** Called at first after constructor */
	Steinberg::tresult PLUGIN_API initialize (Steinberg::FUnknown* context) SMTG_OVERRIDE;
	Steinberg::tresult PLUGIN_API setBusArrangements (Steinberg::Vst::SpeakerArrangement* inputs, int32 numIns, Steinberg::Vst::SpeakerArrangement* outputs, int32 numOuts) SMTG_OVERRIDE;

	Steinberg::tresult PLUGIN_API setActive (Steinberg::TBool state) SMTG_OVERRIDE;
	Steinberg::tresult PLUGIN_API process (Steinberg::Vst::ProcessData& data) SMTG_OVERRIDE;
		
	/** For persistence */
	Steinberg::tresult PLUGIN_API setState (Steinberg::IBStream* state) SMTG_OVERRIDE;
	Steinberg::tresult PLUGIN_API getState (Steinberg::IBStream* state) SMTG_OVERRIDE;

	// Create function
	static Steinberg::FUnknown* createInstance(void*)
	{
		return (Steinberg::Vst::IAudioProcessor*)new DelayDemoProcessor;
	}

	protected:
		Steinberg::Vst::ParamValue mDelay;
		float** mBuffer;
		int32 mBufferPos;
		bool mBypass;
}; // end class def
//------------------------------------------------------------------------
} // namespace redetach
