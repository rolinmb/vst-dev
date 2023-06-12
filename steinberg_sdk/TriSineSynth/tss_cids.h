//------------------------------------------------------------------------
// Copyright(c) 2023 redetach.
//------------------------------------------------------------------------

#pragma once

#include "pluginterfaces/base/funknown.h"
#include "pluginterfaces/vst/vsttypes.h"

namespace redetach {
//------------------------------------------------------------------------
static const Steinberg::FUID kTriSineSynthProcessorUID (0x1EC96A08, 0x63C55A93, 0x8B506914, 0xE8BFDD27);
static const Steinberg::FUID kTriSineSynthControllerUID (0x56E27BDF, 0xB65F5645, 0xAD14C6EF, 0x9F4B54E0);

#define TriSineSynthVST3Category "Instrument"

//------------------------------------------------------------------------
} // namespace redetach
