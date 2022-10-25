#pragma once
#include <lyrahgames/xstd/utility.hpp>

namespace lyrahgames::xstd {

namespace generic {

template <typename T>
concept callable = requires(T v) {
  std::function{v};
};

}  // namespace generic

namespace meta {

template <typename T>
struct function;

template <typename R, typename... Args>
struct function<std::function<R(Args...)>> {
  using result = R;
  using arguments = std::tuple<Args...>;
  template <size_t n>
  using argument = std::tuple_element_t<n, std::tuple<Args...>>;

  static constexpr size_t argument_count = sizeof...(Args);

  explicit function(std::function<R(Args...)>) {}
};

template <generic::callable T>
function(T) -> function<decltype(std::function{std::declval<T>()})>;

// We have to introduce a second using declaration.
// Otherwise, strange compiler errors seem to happen.
// We do not know why this happens.

template <generic::callable T>
using qualified_result = typename decltype(function{std::declval<T>()})::result;

template <generic::callable T>
using result = std::decay_t<qualified_result<T>>;

template <generic::callable T>
constexpr auto argument_count = decltype(function{
    std::declval<T>()})::argument_count;

template <generic::callable T>
using arguments = typename decltype(function{std::declval<T>()})::arguments;

template <generic::callable T, size_t N = 0>
requires(N < argument_count<T>)  //
    using qualified_argument = std::decay_t<typename decltype(function{
        std::declval<T>()})::template argument<N>>;

template <generic::callable T, size_t N = 0>
requires(N < argument_count<T>)  //
    using argument = std::decay_t<qualified_argument<T, N>>;

}  // namespace meta

}  // namespace lyrahgames::xstd
