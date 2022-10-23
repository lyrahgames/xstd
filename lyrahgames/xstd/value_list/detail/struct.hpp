#pragma once
#include <lyrahgames/xstd/value_list/detail/functions.hpp>

namespace lyrahgames::xstd {

namespace detail::value_list {

template <auto... values>
struct base {
  using self = value_list<values...>;

  static constexpr size_t size = detail::value_list::size<self>::value;

  static constexpr bool empty = detail::value_list::empty<self>::value;

  using types = typename detail::value_list::types<self>::type;

  template <auto value>
  // static constexpr bool contains = (meta::strict_equal<value, values> ||
  // ...);
  static constexpr bool contains =
      detail::value_list::contains<self, value>::value;

  static constexpr bool is_set = detail::value_list::is_set<self>::value;

  template <size_t index>
  static constexpr decltype(auto) element =
      detail::value_list::element<self, index>::value;

  template <auto x>
  requires(contains<x>)  //
      static constexpr size_t index = detail::value_list::index<self, x>::value;

  template <auto value>
  using push_front = typename detail::value_list::push_front<self, value>::type;

  template <auto value>
  using push_back = typename detail::value_list::push_back<self, value>::type;

  template <size_t index, auto value>
  using insert = typename detail::value_list::insert<self, index, value>::type;

  template <size_t index>
  using remove = typename detail::value_list::remove<self, index>::type;

  template <auto value>
  requires(contains<value>)  //
      using remove_value =
          typename detail::value_list::remove_value<self, value>::type;

  template <auto value>
  using try_remove_value =
      typename detail::value_list::try_remove_value<self, value>::type;

  template <size_t index, auto value>
  using assignment =
      typename detail::value_list::assignment<self, index, value>::type;

  template <size_t first, size_t last>
  using range = typename detail::value_list::range<self, first, last>::type;

  template <size_t count>
  using trim_front = typename detail::value_list::trim_front<self, count>::type;

  template <size_t count>
  using trim_back = typename detail::value_list::trim_back<self, count>::type;

  template <typename list>
  using concat = typename detail::value_list::concatenation<self, list>::type;

  using reverse = typename detail::value_list::reverse<self>::type;

  template <size_t i, size_t j>
  using swap = typename detail::value_list::swap<self, i, j>::type;

  template <auto less>
  using sort = typename detail::value_list::sort<self, less>::type;

  // template <typename type, template <typename, typename> typename condition>
  // using insert_when =
  //     typename detail::value_list::insert_when<self, type, condition>::type;

  template <auto function>
  using transform =
      typename detail::value_list::transform<self, function>::type;

  template <auto function, auto init>
  static constexpr auto reduce =
      detail::value_list::reduce<self, function, init>::value;

  static constexpr auto for_each(auto&& f) {
    (f.template operator()<values>(), ...);
  }

  static constexpr bool for_each_until(auto&& f) {
    return detail::value_list::for_each_until<self>(
        std::forward<decltype(f)>(f));
  }
};

}  // namespace detail::value_list

/// The type specialization for an empty value list.
/// An empty value list does not provide access to the front or back.
/// Also, to pop from the front or back makes no sense.
template <>
struct value_list<> : detail::value_list::base<> {};

/// The type specialization for a non-empty value list.
template <auto V, auto... values>
struct value_list<V, values...> : detail::value_list::base<V, values...> {
  using base = detail::value_list::base<V, values...>;
  using base::size;

  using self = value_list<V, values...>;

  static constexpr decltype(auto) front =
      detail::value_list::front<self>::value;
  static constexpr decltype(auto) back = detail::value_list::back<self>::value;

  using pop_front = typename detail::value_list::pop_front<self>::type;
  using pop_back = typename detail::value_list::pop_back<self>::type;
};

}  // namespace lyrahgames::xstd
