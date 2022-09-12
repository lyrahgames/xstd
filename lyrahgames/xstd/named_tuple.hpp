#pragma once
#include <tuple>
#include <utility>
//
#include <lyrahgames/xstd/forward.hpp>
#include <lyrahgames/xstd/static_zstring.hpp>
#include <lyrahgames/xstd/value_list.hpp>

namespace lyrahgames::xstd {

template <static_zstring... strings>
using static_zstring_list = value_list<strings...>;

namespace detail {
template <typename type>
struct is_static_zstring_list : std::false_type {};
template <static_zstring... strings>
struct is_static_zstring_list<static_zstring_list<strings...>>
    : std::true_type {};
}  // namespace detail

template <typename list>
constexpr bool is_static_zstring_list =
    detail::is_static_zstring_list<list>::value;

namespace instance {
template <typename T>
concept static_zstring_list = is_static_zstring_list<T>;
}

template <instance::static_zstring_list strings, typename... T>
requires(strings::size == sizeof...(T))  //
    struct named_tuple : std::tuple<T...> {
  using names = strings;
  using types = type_list<T...>;
  using base = std::tuple<T...>;
  using base::base;

  static constexpr size_t size = sizeof...(T);
  static constexpr bool empty = size == 0;

  // operator base() const noexcept { return *this; }
  // constexpr auto tuple() noexcept -> base { return *this; }

  template <static_zstring name>
  static constexpr bool contains = names::template contains<name>;
};

namespace detail {
template <typename type>
struct is_named_tuple : std::false_type {};
template <instance::static_zstring_list names, typename... types>
struct is_named_tuple<named_tuple<names, types...>> : std::true_type {};
}  // namespace detail

template <typename T>
constexpr bool is_named_tuple = detail::is_named_tuple<T>::value;

namespace instance {
template <typename T>
concept named_tuple = is_named_tuple<T>;
}  // namespace instance

namespace forwardable {
template <typename T>
concept named_tuple = instance::named_tuple<std::decay_t<T>>;
}

template <static_zstring... names, typename... types>
constexpr auto auto_named_tuple(types&&... t) noexcept {
  return named_tuple<static_zstring_list<names...>,
                     std::unwrap_ref_decay_t<types>...>(
      std::forward<types>(t)...);
}

template <static_zstring... names, typename... types>
constexpr auto named_tuple_forward(types&&... t) noexcept {
  return named_tuple<static_zstring_list<names...>, types&&...>(
      std::forward<types>(t)...);
}

template <size_t index>
constexpr decltype(auto) value(forwardable::named_tuple auto&& t) noexcept  //
    requires(index < std::decay_t<decltype(t)>::names::size) {
  return std::get<index>(forward<decltype(t)>(t));
}

template <static_zstring name>
constexpr decltype(auto) value(forwardable::named_tuple auto&& t) noexcept  //
    requires(std::decay_t<decltype(t)>::template contains<name>) {
  using tuple_type = std::decay_t<decltype(t)>;
  return std::get<tuple_type::names::template index<name>>(
      std::forward<decltype(t)>(t));
}

namespace detail {
template <static_zstring name, std::size_t... indices>
constexpr auto push_back(forwardable::named_tuple auto&& t,
                         auto&& x,
                         std::index_sequence<indices...>) {
  using tuple_type = std::decay_t<decltype(t)>;
  using namespace meta::value_list;
  return auto_named_tuple<element<typename tuple_type::names, indices>...,
                          name>(value<indices>(std::forward<decltype(t)>(t))...,
                                std::forward<decltype(x)>(x));
}
}  // namespace detail

template <static_zstring name>
constexpr auto push_back(forwardable::named_tuple auto&& t, auto&& value) {
  return detail::push_back<name>(
      std::forward<decltype(t)>(t), std::forward<decltype(value)>(value),
      std::make_index_sequence<std::decay_t<decltype(t)>::names::size>{});
}

namespace detail {
template <static_zstring name, std::size_t... indices>
constexpr auto push_back_forward(forwardable::named_tuple auto&& t,
                                 auto&& x,
                                 std::index_sequence<indices...>) {
  using tuple_type = std::decay_t<decltype(t)>;
  using namespace meta::value_list;
  return named_tuple_forward<element<typename tuple_type::names, indices>...,
                             name>(
      value<indices>(std::forward<decltype(t)>(t))...,
      std::forward<decltype(x)>(x));
}
}  // namespace detail

template <static_zstring name>
constexpr auto push_back_forward(forwardable::named_tuple auto&& t, auto&& x) {
  return detail::push_back_forward<name>(
      std::forward<decltype(t)>(t), std::forward<decltype(x)>(x),
      std::make_index_sequence<std::decay_t<decltype(t)>::names::size>{});
}

}  // namespace lyrahgames::xstd
