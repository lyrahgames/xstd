#pragma once
#include <lyrahgames/xstd/builtin_types.hpp>

namespace lyrahgames::xstd {

// Static zero-terminated strings can be used as template arguments.
// C++20 support has to be enabled.

// The length has to be known.
template <size_t N>
struct static_zstring {
  constexpr static_zstring() noexcept = default;

  // Enable implicit construction of static strings from C strings.
  constexpr static_zstring(const char (&str)[N]) noexcept {
    for (size_t i = 0; i < N; ++i)
      data[i] = str[i];
  }

  // Enable implicit conversion to C strings.
  constexpr operator zstring() noexcept { return data; }
  constexpr operator czstring() const noexcept { return data; }

  // The compiler needs to be able to compare the content of strings.
  friend constexpr auto operator<=>(const static_zstring&,
                                    const static_zstring&) noexcept = default;

  static constexpr auto size() { return N; }

  char data[N]{};
};

// Append characters to static string should be compile-time enabled.
template <size_t N>
constexpr auto operator+(static_zstring<N> x, char c) noexcept {
  static_zstring<N + 1> result{};
  for (size_t i = 0; i < N - 1; ++i)
    result.data[i] = x.data[i];
  result.data[N - 1] = c;
  return result;
}

// Appending two static strings should also be compile-time enabled.
template <size_t N, size_t M>
constexpr auto operator+(static_zstring<N> x, static_zstring<M> y) noexcept {
  static_zstring<N + M - 1> result{};
  size_t i = 0;
  for (; i < N - 1; ++i)
    result.data[i] = x.data[i];
  for (; i < N + M - 1; ++i)
    result.data[i] = y.data[i - N + 1];
  return result;
}

}  // namespace lyrahgames::xstd
