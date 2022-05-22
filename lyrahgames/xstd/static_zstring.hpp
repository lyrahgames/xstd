#pragma once
#include <lyrahgames/xstd/builtin_types.hpp>

namespace lyrahgames::xstd {

// Static zero-terminated strings can be used as template arguments.
// C++20 support has to be enabled.

// We will not use a variant such as basic_static_zstring to provide wchar_t
// support. This would makes complicated and difficult to read without much
// benefit.

// The length has to be known.
template <size_t N>
struct static_zstring {
  static_assert(N > 0, "String must at least store a zero byte.");

  using iterator = zstring;
  using const_iterator = czstring;

  constexpr static_zstring() noexcept = default;

  // Enable implicit construction of static strings from C strings.
  constexpr static_zstring(const char (&str)[N]) noexcept {
    for (size_t i = 0; i < N; ++i)
      data_[i] = str[i];
  }

  // Enable implicit conversion to C strings.
  constexpr operator zstring() noexcept { return data_; }
  constexpr operator czstring() const noexcept { return data_; }

  constexpr auto operator[](size_t index) noexcept -> char& {
    return data_[index];
  }

  constexpr auto operator[](size_t index) const noexcept -> char {
    return data_[index];
  }

  // The compiler needs to be able to compare the content of strings.
  friend constexpr auto operator<=>(const static_zstring&,
                                    const static_zstring&) noexcept = default;

  static constexpr auto size() noexcept { return N - 1; }
  static constexpr bool empty() noexcept { return size() == 0; }

  constexpr auto data() noexcept -> zstring { return data_; }
  constexpr auto data() const noexcept -> czstring { return data_; }

  constexpr auto begin() noexcept -> iterator { return &data_[0]; }
  constexpr auto begin() const noexcept -> const_iterator { return &data_[0]; }
  constexpr auto end() noexcept -> iterator { return &data_[N]; }
  constexpr auto end() const noexcept -> const_iterator { return &data_[N]; }

  char data_[N]{};
};

// Provide support for custom literal '_sz'.
template <static_zstring str>
constexpr auto operator""_sz() noexcept {
  return str;
}

// Append characters to static string should be compile-time enabled.
template <size_t N>
constexpr auto operator+(const static_zstring<N>& str, char c) noexcept {
  static_zstring<N + 1> result{};
  for (size_t i = 0; i < N - 1; ++i)
    result[i] = str[i];
  result[N - 1] = c;
  return result;
}

// Appending two static strings should also be compile-time enabled.
template <size_t N, size_t M>
constexpr auto operator+(const static_zstring<N>& str1,
                         const static_zstring<M>& str2) noexcept {
  static_zstring<N + M - 1> result{};
  size_t i = 0;
  for (; i < N - 1; ++i)
    result[i] = str1[i];
  for (; i < N + M - 1; ++i)
    result[i] = str2[i - N + 1];
  return result;
}

template <size_t offset, size_t size, size_t N>
constexpr auto substring(static_zstring<N> str) noexcept  //
    requires(offset + size < N) {
  static_zstring<size + 1> result{};
  for (size_t i = 0; i < size; ++i)
    result[i] = str[offset + i];
  return result;
}

template <size_t size, size_t N>
constexpr auto prefix(static_zstring<N> str) noexcept  //
    requires(size < N) {
  return substring<0, size>(str);
}

template <size_t size, size_t N>
constexpr auto suffix(static_zstring<N> str) noexcept  //
    requires(size < N) {
  return substring<N - 1 - size, size>(str);
}

template <size_t N, size_t M>
constexpr auto prefix_match_index(static_zstring<N> str1,
                                  static_zstring<M> str2) noexcept -> size_t {
  constexpr auto bound = (N < M) ? N : M;
  for (size_t i = 0; i < bound - 1; ++i)
    if (str1[i] != str2[i]) return i;
  return bound;
}

}  // namespace lyrahgames::xstd
