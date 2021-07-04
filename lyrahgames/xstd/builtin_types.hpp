#pragma once
#include <cstdint>
//
#include <lyrahgames/xstd/contracts.hpp>

namespace lyrahgames::xstd {

using float32 = float;
using float64 = double;

using int8  = int8_t;
using int16 = int16_t;
using int32 = int32_t;
using int64 = int64_t;

using uint8  = uint8_t;
using uint16 = uint16_t;
using uint32 = uint32_t;
using uint64 = uint64_t;

using ufloat32 = non_negative<float32>;
using ufloat64 = non_negative<float64>;
using nfloat32 = normalized<float32>;
using nfloat64 = normalized<float64>;

}  // namespace lyrahgames::xstd
