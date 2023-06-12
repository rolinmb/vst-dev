//------------------------------------------------------------------------
// Copyright(c) 2023 redetach.
//------------------------------------------------------------------------

#pragma once

#include "pluginterfaces/base/funknown.h"
#include "pluginterfaces/vst/vsttypes.h"

namespace redetach {
//------------------------------------------------------------------------
static const Steinberg::FUID kTriangleSynthProcessorUID (0x5AE5DF38, 0xAD655FAD, 0x9B76E4D4, 0x76840B3F);
static const Steinberg::FUID kTriangleSynthControllerUID (0xAAAC98F3, 0x58DB5C4A, 0x882E3F4C, 0x298388FE);

#define TriangleSynthVST3Category "Fx"

//------------------------------------------------------------------------
} // namespace redetach
