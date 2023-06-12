//------------------------------------------------------------------------
// Copyright(c) 2023 redetach.
//------------------------------------------------------------------------

#pragma once

#include "pluginterfaces/base/funknown.h"
#include "pluginterfaces/vst/vsttypes.h"


namespace redetach {
enum GainParams : Steinberg::Vst::ParamID
{
	kGainId = 0,
};
//------------------------------------------------------------------------
static const Steinberg::FUID kGainDemoProcessorUID (0xABD9C4F9, 0xB54456A3, 0xB541B5DB, 0xFB5AE44A);
static const Steinberg::FUID kGainDemoControllerUID (0x355A18EB, 0x3F6D5658, 0x80C3F11E, 0x1D74F2D0);

#define GainDemoVST3Category "Fx"
#define default_gain 1.

//------------------------------------------------------------------------
} // namespace redetach
