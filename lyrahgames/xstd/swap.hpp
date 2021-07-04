#pragma once
#include <utility>

namespace lyrahgames::xstd {

namespace generic {

template <typename T>
concept member_swappable = requires(T& a, T& b) {
  a.swap(b);
};

}  // namespace generic

template <typename T>
constexpr void swap(T& a, T& b) noexcept(noexcept(std::swap(a, b))) {
  std::swap(a, b);
}

template <generic::member_swappable T>
constexpr void swap(T& a, T& b) noexcept(noexcept(a.swap(b))) {
  a.swap(b);
}

}  // namespace lyrahgames::xstd
