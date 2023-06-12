//------------------------------------------------------------------------
// Copyright(c) 2023 redetach.
//------------------------------------------------------------------------

#pragma once

#include "pluginterfaces/base/funknown.h"
#include "pluginterfaces/vst/vsttypes.h"

namespace redetach {
//------------------------------------------------------------------------
static const Steinberg::FUID kSquareSynthProcessorUID (0xBC1FD909, 0x68A75763, 0xBE21CC23, 0x14B1E2DE);
static const Steinberg::FUID kSquareSynthControllerUID (0xB122729E, 0xBEC95E2D, 0xAED5C183, 0x9BAF3AFB);

#define SquareSynthVST3Category "Instrument"

//------------------------------------------------------------------------
} // namespace redetach
