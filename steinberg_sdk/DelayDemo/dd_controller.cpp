//------------------------------------------------------------------------
// Copyright(c) 2023 redetach.
//------------------------------------------------------------------------

#include "dd_controller.h"
#include "dd_cids.h"
#include "pluginterfaces/base/ustring.h"
#include "pluginterfaces/base/ibstream.h"
#include "vstgui/plugin-bindings/vst3editor.h"
#include "base/source/fstreamer.h"

using namespace Steinberg;

namespace redetach {

//------------------------------------------------------------------------
// DelayDemoController Implementation
//------------------------------------------------------------------------
tresult PLUGIN_API DelayDemoController::initialize (FUnknown* context)
{
	// Here the Plug-in will be instantiated

	//---do not forget to call parent ------
	tresult result = EditControllerEx1::initialize (context);
	if (result == kResultTrue)
	{ // register parameters
		parameters.addParameter(STR16("Bypass"), nullptr, 1, 0, Vst::ParameterInfo::kCanAutomate|Vst::ParameterInfo::kIsBypass, kBypassId);
		parameters.addParameter(STR16("Delay"), STR16("sec"), 0, 1, Vst::ParameterInfo::kCanAutomate, kDelayId);
	}

	return kResultTrue;
}
//------------------------------------------------------------------------
tresult PLUGIN_API DelayDemoController::setComponentState (IBStream* state)
{
	// Here you get the state of the component (Processor part)
	if (!state)
		return kResultFalse;

	IBStreamer streamer(state, kLittleEndian);
	float savedDelay = 0.f;
	if (streamer.readFloat(savedDelay) == false)
		return kResultFalse;
	
	setParamNormalized(kDelayId, savedDelay);
	int32 bypassState = 0;
	if (streamer.readInt32(bypassState) == false)
	{/* could be an old version, continue */ }

	setParamNormalized(kBypassId, bypassState ? 1 : 0);

	return kResultOk;
}
//------------------------------------------------------------------------
} // namespace redetach
