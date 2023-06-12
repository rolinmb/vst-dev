//------------------------------------------------------------------------
// Copyright(c) 2023 redetach.
//------------------------------------------------------------------------

#pragma once

#include "pluginterfaces/base/funknown.h"
#include "pluginterfaces/vst/vsttypes.h"

namespace redetach {
//------------------------------------------------------------------------
static const Steinberg::FUID kSineSynthProcessorUID (0xDD576B22, 0x9E1954F5, 0x840002F1, 0x3A9BA625);
static const Steinberg::FUID kSineSynthControllerUID (0x9CF3B2C1, 0x3D2D5D30, 0x9526A9A2, 0x174A9C1A);

#define SineSynthVST3Category "Instrument"

//------------------------------------------------------------------------
} // namespace redetach
