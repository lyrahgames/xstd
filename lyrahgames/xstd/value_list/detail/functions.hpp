#pragma once
#include <lyrahgames/xstd/type_list/common.hpp>
#include <lyrahgames/xstd/value_list/common.hpp>

namespace lyrahgames::xstd {

/// Forward declarations for type function details.
// This is needed for member type functions.
// Also, for every using declaration providing a non-member type function,
// an implementation given by struct specializations has to be declared.
//
// Every type function implementation is provided as its own template.
namespace detail::value_list {

using xstd::value_list;

// Two constexpr values are strictly equal
// if their types and values coincide.
//
template <auto x, auto y>
struct strict_equal : std::false_type {};
//
template <auto x>
struct strict_equal<x, x> : std::true_type {};

//
template <instance::value_list list>
struct size;
template <auto... values>
struct size<value_list<values...>> {
  static constexpr size_t value = sizeof...(values);
};

//
template <instance::value_list list>
struct empty {
  static constexpr bool value = size<list>::value == 0;
};

template <instance::value_list list, auto x>
struct contains : std::false_type {};
template <auto x, auto front, auto... tail>
struct contains<value_list<front, tail...>, x> {
  static constexpr bool value =
      strict_equal<x, front>::value || contains<value_list<tail...>, x>::value;
};

//
template <instance::value_list list>
struct types;
template <auto... values>
struct types<value_list<values...>> {
  using type = xstd::type_list<decltype(values)...>;
};

//
template <instance::value_list list>
struct is_set : std::false_type {};
template <>
struct is_set<value_list<>> : std::true_type {};
template <auto front, auto... tail>
struct is_set<value_list<front, tail...>> {
  using tail_list = value_list<tail...>;
  static constexpr bool value =
      !contains<tail_list, front>::value && is_set<tail_list>::value;
};

// Empty lists have no type access.
template <instance::value_list list, size_t index>
struct element {};
// The first value can be accessed by index zero.
template <auto front, auto... tail>
struct element<value_list<front, tail...>, 0> {
  static constexpr decltype(auto) value = front;
};
// For all other indices, we use a recursive call
// to elements with a reduced pack extension and index.
template <size_t index, auto front, auto... tail>
struct element<value_list<front, tail...>, index> {
  static constexpr decltype(auto) value =
      element<value_list<tail...>, index - 1>::value;
};

//
template <instance::value_list list, auto value>
struct index {};
template <auto x, auto front, auto... tail>
struct index<value_list<front, tail...>, x> {
  static constexpr size_t value = 1 + index<value_list<tail...>, x>::value;
};
template <auto x, auto front, auto... tail>
requires(strict_equal<x, front>::value)  //
    struct index<value_list<front, tail...>, x> {
  static constexpr size_t value = 0;
};

//
template <instance::value_list list>
struct front {};
template <auto x, auto... values>
struct front<value_list<x, values...>> {
  static constexpr decltype(auto) value = x;
};

//
template <instance::value_list list>
struct back {};
template <auto x>
struct back<value_list<x>> {
  static constexpr decltype(auto) value = x;
};
template <auto x, auto... values>
struct back<value_list<x, values...>> {
  static constexpr decltype(auto) value = back<value_list<values...>>::value;
};

//
template <instance::value_list list, auto value>
struct push_back {};
template <auto x, auto... values>
struct push_back<value_list<values...>, x> {
  using type = value_list<values..., x>;
};

//
template <instance::value_list list, auto value>
struct push_front {};
template <auto x, auto... values>
struct push_front<value_list<values...>, x> {
  using type = value_list<x, values...>;
};

//
template <instance::value_list x, instance::value_list y>
struct concatenation {};
template <auto... x, auto... y>
struct concatenation<value_list<x...>, value_list<y...>> {
  using type = value_list<x..., y...>;
};

//
template <instance::value_list list>
struct pop_front {};
template <auto x, auto... values>
struct pop_front<value_list<x, values...>> {
  using type = value_list<values...>;
};

//
template <instance::value_list list>
struct pop_back {};
template <auto x>
struct pop_back<value_list<x>> {
  using type = value_list<>;
};
template <auto x, auto... values>
struct pop_back<value_list<x, values...>> {
  using type =
      typename push_front<typename pop_back<value_list<values...>>::type,
                          x>::type;
};

//
template <instance::value_list list>
struct reverse {
  using type = list;
};
template <auto x, auto... values>
struct reverse<value_list<x, values...>> {
  using type = typename push_back<typename reverse<value_list<values...>>::type,
                                  x>::type;
};

//
template <instance::value_list list, size_t count>
requires(count <= list::size)  //
    struct trim_front {
  using type =
      typename trim_front<typename pop_front<list>::type, count - 1>::type;
};
template <instance::value_list list>
struct trim_front<list, 0> {
  using type = list;
};

//
template <instance::value_list list, size_t count>
requires(count <= list::size)  //
    struct trim_back {
  using type =
      typename trim_back<typename pop_back<list>::type, count - 1>::type;
};
template <instance::value_list list>
struct trim_back<list, 0> {
  using type = list;
};

//
template <instance::value_list list, size_t first, size_t last>
requires(first <= last) && (last <= list::size)  //
    struct range {
  using type =
      typename trim_front<typename trim_back<list, list::size - last>::type,
                          first>::type;
};

template <instance::value_list list, size_t i, size_t j>
struct swap;
template <instance::value_list list, size_t i, size_t j>
requires(i < j) && (j < list::size)  //
    struct swap<list, i, j> {
  using left = typename range<list, 0, i>::type;
  using middle = typename range<list, i + 1, j>::type;
  using right = typename range<list, j + 1, list::size>::type;
  static constexpr decltype(auto) xi = element<list, i>::value;
  static constexpr decltype(auto) xj = element<list, j>::value;
  using new_left = typename push_back<left, xj>::type;
  using new_right = typename push_front<right, xi>::type;
  using tmp = typename concatenation<new_left, middle>::type;
  using type = typename concatenation<tmp, new_right>::type;
};
template <instance::value_list list, size_t i, size_t j>
requires(j < i) && (i < list::size)  //
    struct swap<list, i, j> {
  using type = typename swap<list, j, i>::type;
};
template <instance::value_list list, size_t i>
struct swap<list, i, i> {
  using type = list;
};

//
template <instance::value_list list, size_t index, auto x>
requires(index <= list::size)  //
    struct insert {
  using left = typename range<list, 0, index>::type;
  using right = typename range<list, index, list::size>::type;
  using tmp = typename push_back<left, x>::type;
  using type = typename concatenation<tmp, right>::type;
};

//
template <instance::value_list list, size_t index>
requires(index < list::size)  //
    struct remove {
  using left = typename range<list, 0, index>::type;
  using right = typename range<list, index + 1, list::size>::type;
  using type = typename concatenation<left, right>::type;
};

//
template <instance::value_list list, auto x>
struct remove_value {
  using type = typename remove<list, index<list, x>::value>::type;
};

//
template <instance::value_list list, auto x>
struct try_remove_value {
  using type = list;
};
template <instance::value_list list, auto x>
requires(list::template contains<x>)  //
    struct try_remove_value<list, x> {
  using type = typename remove_value<list, x>::type;
};

//
template <instance::value_list list, size_t index, auto value>
requires(index < list::size)  //
    struct assignment {
  using left = typename range<list, 0, index>::type;
  using right = typename range<list, index + 1, list::size>::type;
  using new_right = typename push_front<right, value>::type;
  using type = typename concatenation<left, new_right>::type;
};

//
template <instance::value_list left, instance::value_list right, auto less>
struct merge;
template <auto less>
struct merge<value_list<>, value_list<>, less> {
  using type = value_list<>;
};
template <auto less, auto... values>
struct merge<value_list<values...>, value_list<>, less> {
  using type = value_list<values...>;
};
template <auto less, auto... values>
struct merge<value_list<>, value_list<values...>, less> {
  using type = value_list<values...>;
};
template <auto less,
          auto left,
          auto right,
          auto... left_tail,
          auto... right_tail>
requires(less.template operator()<left, right>())  //
    struct merge<value_list<left, left_tail...>,
                 value_list<right, right_tail...>,
                 less> {
  using type =
      typename push_front<typename merge<value_list<left_tail...>,
                                         value_list<right, right_tail...>,
                                         less>::type,
                          left>::type;
};
template <auto less,
          auto left,
          auto right,
          auto... left_tail,
          auto... right_tail>
requires(!less.template operator()<left, right>())  //
    struct merge<value_list<left, left_tail...>,
                 value_list<right, right_tail...>,
                 less> {
  using type =
      typename push_front<typename merge<value_list<left, left_tail...>,
                                         value_list<right_tail...>,
                                         less>::type,
                          right>::type;
};

//
template <instance::value_list list, auto less>
struct sort {
  static constexpr size_t half = list::size / 2;
  using type = typename merge<
      typename sort<typename range<list, 0, half>::type, less>::type,
      typename sort<typename range<list, half, list::size>::type, less>::type,
      less>::type;
};
template <instance::value_list list, auto less>
requires(list::size <= 1)  //
    struct sort<list, less> {
  using type = list;
};

//
// template <instance::value_list list,
//           typename t,
//           template <typename, typename>
//           typename less>
// struct insert_when;
// template <typename t, template <typename, typename> typename less>
// struct insert_when<value_list<>, t, less> {
//   using type = value_list<t>;
// };
// template <typename t,
//           typename front,
//           template <typename, typename>
//           typename less,
//           typename... types>
// struct insert_when<value_list<front, types...>, t, less> {
//   using type = value_list<t, front, types...>;
// };
// template <typename t,
//           typename front,
//           template <typename, typename>
//           typename less,
//           typename... types>
// requires(!less<t, front>::value)  //
//     struct insert_when<value_list<front, types...>, t, less> {
//   using type = typename push_front<
//       typename insert_when<value_list<types...>, t, less>::type,
//       front>::type;
// };

//
template <instance::value_list list, auto function>
struct transform {};
//
template <auto function, auto... values>
struct transform<value_list<values...>, function> {
  using type = value_list<function.template operator()<values>()...>;
};

//
template <instance::value_list list, auto function, auto init>
struct reduce {
  static constexpr auto value =
      function.template
      operator()<reduce<typename list::pop_back, function, init>::value,
                 list::back>();
};
//
template <auto function, auto init>
struct reduce<value_list<>, function, init> {
  static constexpr auto value = init;
};

//
//
template <instance::value_list list>
constexpr bool for_each_until(auto&& f) {
  if constexpr (list::empty)
    return false;
  else {
    if (f.template operator()<front<list>::value>()) return true;
    return for_each_until<typename pop_front<list>::type>(
        std::forward<decltype(f)>(f));
  }
}

//
//
template <size_t size, auto offset, auto increment>
struct iota {
  using type = typename iota<size - 1, increment(offset), increment>::type::
      template push_front<offset>;
};
//
template <auto offset, auto increment>
struct iota<0, offset, increment> {
  using type = value_list<>;
};

}  // namespace detail::value_list

}  // namespace lyrahgames::xstd
