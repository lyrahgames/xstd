#pragma once
#include <compare>
#include <concepts>
#include <functional>
#include <type_traits>
#include <typeinfo>

namespace lyrahgames::xstd {

namespace meta {

template <typename T>
constexpr auto print_type() = delete;

template <typename T>
constexpr auto print_type(T) = delete;

template <typename T>
using reduction = std::decay_t<T>;

}  // namespace meta

namespace generic {

///
///
template <typename T, typename U>
concept identical = std::same_as<T, U>;

template <typename T, typename U>
concept reducible = identical<std::decay_t<T>, U>;

template <typename T>
concept irreducible = reducible<T, T>;

template <typename T>
concept callable = requires(T v) {
  std::function{v};
};

}  // namespace generic

namespace generic {

namespace detail {

template <template <typename...> typename T, typename U>
struct instantiated : std::false_type {};

template <template <typename...> typename T, typename... U>
struct instantiated<T, T<U...>> : std::true_type {};

}  // namespace detail

template <typename T, template <typename...> typename U>
concept instantiation = detail::instantiated<U, std::decay_t<T>>::value;

}  // namespace generic

namespace meta {

template <typename T, typename U>
constexpr auto equal = std::same_as<T, U>;

namespace detail {
template <auto x, auto y>
struct strict_equal : std::false_type {};
template <auto x>
struct strict_equal<x, x> : std::true_type {};
}  // namespace detail

template <auto x, auto y>
constexpr bool strict_equal = detail::strict_equal<x, y>::value;

}  // namespace meta

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
