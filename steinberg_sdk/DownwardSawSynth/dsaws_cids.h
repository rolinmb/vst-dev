//------------------------------------------------------------------------
// Copyright(c) 2023 redetach.
//------------------------------------------------------------------------

#pragma once

#include "pluginterfaces/base/funknown.h"
#include "pluginterfaces/vst/vsttypes.h"

namespace redetach {
//------------------------------------------------------------------------
static const Steinberg::FUID kDownwardSawSynthProcessorUID (0xC412C906, 0x0D505B82, 0xA64DD771, 0x304A1198);
static const Steinberg::FUID kDownwardSawSynthControllerUID (0x1D4D13B2, 0x7CB95441, 0xA547AB70, 0xA79AA75D);

#define DownwardSawSynthVST3Category "Instrument"

//------------------------------------------------------------------------
} // namespace redetach
