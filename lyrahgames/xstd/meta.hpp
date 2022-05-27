#pragma once
#include <concepts>
#include <functional>

namespace lyrahgames::xstd {

namespace meta {

template <typename T>
constexpr auto print_type() = delete;

template <typename T>
constexpr auto print_type(T) = delete;

}  // namespace meta

namespace generic {

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

// Access to elements of pack extension by using indices.
template <size_t index, typename... types>
struct variadic_projection;

template <size_t index>
struct variadic_projection<index> {};

template <typename t, typename... types>
struct variadic_projection<0, t, types...> {
  using type = t;
};

template <size_t index, typename t, typename... types>
struct variadic_projection<index, t, types...> {
  using type = typename variadic_projection<index - 1, types...>::type;
};

template <typename T>
struct size;

template <typename T>
struct empty;

template <typename T, typename U>
struct contains;

template <typename T, auto x>
struct element;

template <typename T>
struct front;

template <typename T>
struct back;

}  // namespace detail

template <typename T>
constexpr auto size = detail::size<T>::value;

template <typename T>
constexpr auto empty = detail::empty<T>::value;

template <typename T, typename U>
constexpr auto contains = detail::contains<T, U>::value;

template <typename T, auto x>
using element = typename detail::element<T, x>::type;

template <typename T>
using front = typename detail::front<T>::type;

template <typename T>
using back = typename detail::back<T>::type;

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
