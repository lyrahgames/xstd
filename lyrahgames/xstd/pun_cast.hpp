#pragma once
#include <concepts>
#include <cstring>

namespace lyrahgames::xstd {

template <std::default_initializable T, typename U>
requires(sizeof(T) == sizeof(U))  //
    inline T pun_cast(U x) noexcept {
  T result{};
  std::memcpy(&result, &x, sizeof(x));
  return result;
}

}  // namespace lyrahgames::xstd
