//------------------------------------------------------------------------
// Copyright(c) 2023 redetach.
//------------------------------------------------------------------------

#pragma once

#include "pluginterfaces/base/funknown.h"
#include "pluginterfaces/vst/vsttypes.h"

namespace redetach {
//------------------------------------------------------------------------
static const Steinberg::FUID kFuzz2ProcessorUID (0xBDAB64A1, 0x49785F09, 0xBC93B21B, 0x14C2B793);
static const Steinberg::FUID kFuzz2ControllerUID (0x8AB28031, 0x0E445C27, 0x90563DD0, 0xE9D68263);

#define Fuzz2VST3Category "Fx"

//------------------------------------------------------------------------
} // namespace redetach
