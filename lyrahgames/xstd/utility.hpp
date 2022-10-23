#pragma once
#include <cassert>
#include <cstddef>
#include <cstdint>

namespace lyrahgames::xstd {

using float32 = float;
using float64 = double;

using int8 = int8_t;
using int16 = int16_t;
using int32 = int32_t;
using int64 = int64_t;

using uint8 = uint8_t;
using uint16 = uint16_t;
using uint32 = uint32_t;
using uint64 = uint64_t;

using zstring = char*;
using czstring = const char*;

/// With respect to the given offset,
/// compute the distance to the next offset
/// that fulfills the given alignment.
///
constexpr auto aligned_offset_padding(size_t offset, size_t alignment) noexcept
    -> size_t {
  return alignment - 1 - ((offset + alignment - 1) % alignment);
}

/// With respect to the given offset,
/// compute the next offset
/// that fulfills the given alignment.
///
constexpr auto aligned_offset(size_t offset, size_t alignment) noexcept
    -> size_t {
  return offset + aligned_offset_padding(offset, alignment);
}

}  // namespace lyrahgames::xstd
