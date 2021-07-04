#pragma once
#include <concepts>
#include <functional>
//
#include <lyrahgames/xstd/meta.hpp>

namespace lyrahgames::xstd {

namespace generic {

template <typename T, typename U>
concept copy_constructible = requires(const T& v) {
  { U(v) } -> identical<U>;
};

template <typename T, typename U>
concept copy_assignable = requires(U& u, const T& v) {
  { u = v } -> identical<U&>;
};

template <typename T, typename U>
concept move_constructible = requires(T& v) {
  { U(std::move(v)) } -> identical<U>;
};

template <typename T, typename U>
concept move_assignable = requires(U& u, T& v) {
  { u = std::move(v) } -> identical<U&>;
};

template <typename T, typename U>
concept forward_constructible = requires(T&& v) {
  { U(std::forward<T>(v)) } -> identical<U>;
};

template <typename T, typename U>
concept forward_assignable = requires(U& u, T&& t) {
  { u = std::forward<T>(t) } -> identical<U&>;
};

template <typename T, typename U>
concept forwardable = forward_constructible<T, U> && forward_assignable<T, U>;

template <typename T, typename U>
concept forward_reference = reducible<T, U> && forwardable<T, U>;

}  // namespace generic

/// Function overload constructs a value of type T out of the given forwardable
/// type K in-place if it is not a dircet forwardable reference of T.
template <generic::irreducible T, generic::forwardable<T> K>
constexpr decltype(auto) forward_construct(K&& k) noexcept {
  return T(std::forward<K>(k));
}

/// Function overload forwards direct forwarding reference of type T.
template <generic::irreducible T, generic::forward_reference<T> K>
constexpr decltype(auto) forward_construct(K&& k) noexcept {
  return std::forward<K>(k);
}

}  // namespace lyrahgames::xstd
