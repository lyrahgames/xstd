#pragma once
#include <lyrahgames/xstd/type_list.hpp>

namespace lyrahgames::xstd {

/// The 'value_list' template should only be used
/// in the context of template meta programming.
/// It enables the access and modification of
/// constexpr values inside variadic templates.
template <auto... values>
struct value_list;

/// For tight constraints in template requirements,
/// we want to be able to decide whether a type is
/// an instance of the 'value_list' template.
namespace detail {
// The implementation of this idea is taken out inside the 'details'
// namespace by a typical template struct predicate specialization.
template <typename t>
struct is_value_list : std::false_type {};
template <auto... values>
struct is_value_list<value_list<values...>> : std::true_type {};
}  // namespace detail

/// To simplify the interface, we use templated variables as alias.
template <typename T>
constexpr bool is_value_list = detail::is_value_list<T>::value;

//
namespace detail {
template <typename list, template <auto> typename constraint>
struct is_constrained_value_list : std::false_type {};
template <template <auto> typename constraint, auto... values>
struct is_constrained_value_list<value_list<values...>, constraint> {
  static constexpr bool value = (constraint<values>::value && ...);
};
}  // namespace detail

///
template <typename list, template <auto> typename constraint>
constexpr bool is_constrained_value_list =
    detail::is_constrained_value_list<list, constraint>::value;

/// To simplify the usage as concept,
/// we also provide a concept alias inside the 'instance' namespace.
namespace instance {
template <typename T>
concept value_list = is_value_list<T>;
template <typename T, template <auto> typename constraint>
concept constrained_value_list = is_constrained_value_list<T, constraint>;
}  // namespace instance

/// Helper/Non-Member Type Functions for Value Lists
/// They are all dependent on the member type functions
/// and show how these would be used.
//
// Starting from here, we have to provide implementations
// for helper/non-member type functions for value lists.
// The namespace 'meta' is used globally inside this library.
// As a consequence, different templates may need to use
// the same names for their type functions.
// In C++, using declarations cannot be overloaded or specialized.
// Concerning template parameters, only functions are allowed
// to overload them by using concepts or values.
// The problem is that each function is only able
// to return a value and not a type.
// This means that in either case
// the interface cannot be uniform and consistent.
// Instead, we provide another namespace 'value_list'
// inside the 'meta' namespace.
namespace meta::value_list {

/// Check whether a given value list is empty.
template <instance::value_list list>
constexpr auto empty = list::empty;

/// Returns the count of values inside the value list.
template <instance::value_list list>
constexpr auto size = list::size;

/// Returns a list of the types of values inside the given value list.
template <instance::value_list list>
using types = typename list::types;

/// Check whether the given value list contains the given value.
template <instance::value_list list, auto value>
constexpr bool contains = list::template contains<value>;

///
template <instance::value_list list>
constexpr bool is_set = list::is_set;

/// Access values inside the value list by their position.
template <instance::value_list list, size_t index>
requires(index < size<list>)  //
    constexpr decltype(auto) element = list::template element<index>;

/// Get back the first index of the given value contained in the value list.
template <instance::value_list list, auto value>
requires(contains<list, value>)  //
    constexpr auto index = list::template index<value>;

/// Access the first value inside the value list.
template <instance::value_list list>
requires(!empty<list>)  //
    constexpr decltype(auto) front = list::front;

/// Access the last value inside the value list.
template <instance::value_list list>
requires(!empty<list>)  //
    constexpr decltype(auto) back = list::back;

/// Returns a new value list of all values positioned in [first,last).
template <instance::value_list list, size_t first, size_t last>
requires(first <= last) && (last <= size<list>)  //
    using range = typename list::template range<first, last>;

/// Appends the given value at the front of the value list.
template <instance::value_list list, auto value>
using push_front = typename list::template push_front<value>;

/// Appends the given value at the end of the value list.
template <instance::value_list list, auto value>
using push_back = typename list::template push_back<value>;

/// Insert the given value at the given position inside the type list.
template <instance::value_list list, size_t index, auto value>
requires(index <= size<list>)  //
    using insert = typename list::template insert<index, value>;

/// Insert the given type into the type list at the first position
/// where the condition predicate evaluates to true.
// template <instance::value_list list,
//           auto value,
//           template <auto, auto>
//           typename condition>
// using insert_when = typename list::template insert_when<value, condition>;

/// Remove a value inside the value list at the given position.
template <instance::value_list list, size_t index>
requires(index < size<list>)  //
    using remove = typename list::template remove<index>;

///
template <instance::value_list list, auto value>
requires(contains<list, value>)  //
    using remove_value = typename list::template remove_value<value>;

///
template <instance::value_list list, auto value>
using try_remove_value = typename list::template try_remove_value<value>;

/// Assign a new value given by its index.
template <instance::value_list list, size_t index, auto value>
requires(index < size<list>)  //
    using assignment = typename list::template assignment<index, value>;

/// Concatenate two given value lists.
template <instance::value_list x, instance::value_list y>
using concatenation = typename x::template concat<y>;

/// Get a value list with reversed order of all contained values.
template <instance::value_list list>
using reverse = typename list::reverse;

/// Sort all contained types by the given order relation predicate.
// template <instance::value_list list,
//           template <typename, typename>
//           typename less>
// using sort = typename list::template sort<less>;

/// Swap two contained values by their given indices.
template <instance::value_list list, size_t i, size_t j>
requires(i < size<list>) && (j < size<list>)  //
    using swap = typename list::template swap<i, j>;

/// Transform algorithm for a value list
/// which transforms every contained type according to a given predicate.
// We only provide this interface style
// because there is no technique in C++
// to provide variadic template predicates
// in a lambda expression style.
// template <instance::value_list list, template <auto> typename f>
// using transformation = typename list::template transformation<f>;

/// Generate code based on given lambda expression for each contained value.
template <instance::value_list list>
constexpr void for_each(auto&& f) {
  list::for_each(std::forward<decltype(f)>(f));
}

///
template <instance::value_list list>
constexpr bool for_each_until(auto&& f) {
  return list::for_each_until(std::forward<decltype(f)>(f));
}

}  // namespace meta::value_list

/// Forward declarations for type function details.
// This is needed for member type functions.
// Also, for every using declaration providing a non-member type function,
// an implementation given by struct specializations has to be declared.
//
// Every type function implementation is provided as its own template.
namespace detail::value_list {

using xstd::value_list;

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
      meta::strict_equal<x, front> || contains<value_list<tail...>, x>::value;
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
requires(meta::strict_equal<x, front>)  //
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
// template <instance::value_list left,
//           instance::value_list right,
//           template <typename, typename>
//           typename less>
// struct merge;
// template <template <typename, typename> typename less>
// struct merge<value_list<>, value_list<>, less> {
//   using type = value_list<>;
// };
// template <template <typename, typename> typename less, typename... T>
// struct merge<value_list<T...>, value_list<>, less> {
//   using type = value_list<T...>;
// };
// template <template <typename, typename> typename less, typename... T>
// struct merge<value_list<>, value_list<T...>, less> {
//   using type = value_list<T...>;
// };
// template <template <typename, typename> typename less,
//           typename T,
//           typename U,
//           typename... t,
//           typename... u>
// requires less<T, U>::value  //
//     struct merge<value_list<T, t...>, value_list<U, u...>, less> {
//   using type = typename push_front<
//       typename merge<value_list<t...>, value_list<U, u...>, less>::type,
//       T>::type;
// };
// template <template <typename, typename> typename less,
//           typename T,
//           typename U,
//           typename... t,
//           typename... u>
// requires(!less<T, U>::value)  //
//     struct merge<value_list<T, t...>, value_list<U, u...>, less> {
//   using type = typename push_front<
//       typename merge<value_list<T, t...>, value_list<u...>, less>::type,
//       U>::type;
// };

//
// template <instance::value_list list,
//           template <typename, typename>
//           typename less>
// struct sort {
//   static constexpr size_t half = list::size / 2;
//   using type = typename merge<
//       typename sort<typename range<list, 0, half>::type, less>::type,
//       typename sort<typename range<list, half, list::size>::type,
//       less>::type, less>::type;
// };
// template <instance::value_list list,
//           template <typename, typename>
//           typename less>
// requires(list::size <= 1)  //
//     struct sort<list, less> {
//   using type = list;
// };

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
// template <instance::value_list list, template <typename> typename f>
// struct transformation;
// template <template <auto> typename f, auto... values>
// struct transformation<value_list<values...>, f> {
//   using type = value_list<f<values>::value...>;
// };

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

  // template <template <typename, typename> typename less>
  // using sort = typename detail::value_list::sort<self, less>::type;

  // template <typename type, template <typename, typename> typename condition>
  // using insert_when =
  //     typename detail::value_list::insert_when<self, type, condition>::type;

  // template <template <auto> typename f>
  // using transformation =
  //     typename detail::value_list::transformation<self, f>::type;

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
