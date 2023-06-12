//------------------------------------------------------------------------
// Copyright(c) 2023 redetach.
//------------------------------------------------------------------------

#pragma once

#include "pluginterfaces/base/funknown.h"
#include "pluginterfaces/vst/vsttypes.h"

namespace redetach {
//------------------------------------------------------------------------
static const Steinberg::FUID kUpwardSawSynthProcessorUID (0xC7C1FA06, 0xBB8B5B1A, 0x9C770BA8, 0xF0B33974);
static const Steinberg::FUID kUpwardSawSynthControllerUID (0xE138C22D, 0x4A025340, 0x830348D4, 0xCB44F071);

#define UpwardSawSynthVST3Category "Instrument"

//------------------------------------------------------------------------
} // namespace redetach
