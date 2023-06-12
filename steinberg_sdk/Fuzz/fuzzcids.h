//------------------------------------------------------------------------
// Copyright(c) 2023 Rolin Blake.
//------------------------------------------------------------------------

#pragma once

#include "pluginterfaces/base/funknown.h"
#include "pluginterfaces/vst/vsttypes.h"

namespace MyCompanyName {
//------------------------------------------------------------------------
static const Steinberg::FUID kFuzzProcessorUID (0xADE2ADE4, 0x19725C45, 0xA46E365D, 0xA7D68444);
static const Steinberg::FUID kFuzzControllerUID (0x07AE8D91, 0xE8F95E09, 0x8CA05495, 0xBEA880C0);

#define FuzzVST3Category "Fx"

//------------------------------------------------------------------------
} // namespace MyCompanyName
