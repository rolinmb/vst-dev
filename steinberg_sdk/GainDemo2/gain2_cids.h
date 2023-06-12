//------------------------------------------------------------------------
// Copyright(c) 2023 redetach.
//------------------------------------------------------------------------

#pragma once

#include "pluginterfaces/base/funknown.h"
#include "pluginterfaces/vst/vsttypes.h"

namespace redetach {
//------------------------------------------------------------------------
static const Steinberg::FUID kGainDemo2ProcessorUID (0xAF7C114A, 0xCC2452C7, 0x870B724D, 0x2A28ACB0);
static const Steinberg::FUID kGainDemo2ControllerUID (0x7696AA6E, 0xF1715447, 0xAD66E5CF, 0x8FE06833);

#define GainDemo2VST3Category "Fx"

//------------------------------------------------------------------------
} // namespace redetach
