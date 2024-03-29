#pragma once
// This header will provide the standard headers
// for mathematical operations as well as some additional functionality.

// Get all the basics.
//
#include <lyrahgames/xstd/utility.hpp>

// We need bit manipulation, C-style math functions, and math constants.
//
#include <bit>
#include <cmath>
#include <numbers>

namespace lyrahgames::xstd {

using namespace std::numbers;

inline uint32_t log2(const uint32_t x) {
  uint32_t y;
  asm("\tbsr %1, %0\n" : "=r"(y) : "r"(x));
  return y;
}

inline uint64_t log2(uint64_t x) noexcept {
  uint64_t y;
  asm("\tbsr %1, %0\n" : "=r"(y) : "r"(x));
  return y;
}

/// Return smallest positive integral power of two
/// that is bigger or equal to the given positive number.
/// If the given number is zero the function returns zero.
/// The function fails if the answer would be 2^32.
/// With NDEBUG defined, it would also return zero.
///
inline uint32_t ceil_pow2(uint32_t x) {
  assert(x <= (uint32_t{1} << 31));
  return uint64_t{1} << (log2(x - 1) + 1);
}

/// Return smallest positive integral power of two
/// that is bigger or equal to the given positive number.
/// The function fails if the given number is zero or if the answer would be
/// 2^64. With NDEBUG defined, undefined behavior is the result in such cases.
///
inline uint64_t ceil_pow2(uint64_t x) {
  assert((0 < x) && (x <= (uint64_t{1} << 63)));
  return uint64_t{1} << (log2(x - 1) + 1);
}

/// Return the N-th non-negative power of the given value.
/// The exponent is a compile-time constant which will result
/// in the construction of an efficient multiplication expression.
/// Especially useful for the use in automatic differentiation.
///
template <size_t N>
constexpr auto pow(auto x) noexcept {
  if constexpr (N == 0)
    return 1;
  else if constexpr (N == 1)
    return x;
  else if constexpr (N % 2) {
    const auto t = pow<(N - 1) / 2>(x);
    return t * t * x;
  } else {
    const auto t = pow<N / 2>(x);
    return t * t;
  }
}

}  // namespace lyrahgames::xstd
