//------------------------------------------------------------------------
// Copyright(c) 2023 redetach.
//------------------------------------------------------------------------

#pragma once

#include "pluginterfaces/base/funknown.h"
#include "pluginterfaces/vst/vsttypes.h"

namespace redetach {
//------------------------------------------------------------------------
enum {
	kDelayId = 100,
	kBypassId = 101
};

static const Steinberg::FUID kDelayDemoProcessorUID (0x57BEA99E, 0xEBED50E8, 0xAFCF2B69, 0x98FB0418);
static const Steinberg::FUID kDelayDemoControllerUID (0xC35F0EA5, 0xC1065078, 0xB43D6EF9, 0x5F233C43);

#define DelayDemoVST3Category "Fx"

//------------------------------------------------------------------------
} // namespace redetach
