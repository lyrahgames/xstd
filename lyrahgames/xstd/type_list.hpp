#pragma once
#include <cstddef>
#include <type_traits>
//
#include <lyrahgames/xstd/meta.hpp>

namespace lyrahgames::xstd {

/// The 'type_list' template should only be used
/// in the context of template meta programming.
/// It enables the access and modification of
/// types inside variadic templates.
template <typename... types>
struct type_list;

/// For tight constraints in template requirements,
/// we want to be able to decide whether a type is
/// an instance of the 'type_list' template.
namespace detail {
// The implementation of this idea is taken out inside 'details'
// by a typical template struct predicate specialization.
template <typename t>
struct is_type_list : std::false_type {};
template <typename... types>
struct is_type_list<type_list<types...>> : std::true_type {};
}  // namespace detail

/// To simplify the interface, we use templated variables as alias.
template <typename T>
constexpr bool is_type_list = detail::is_type_list<T>::value;

/// To simplify the usage as concept, we also provide a concept alias.
namespace instance {
template <typename T>
concept type_list = is_type_list<T>;
}  // namespace instance

/// Helper/Non-Member Type Functions for Type Lists
/// They are all dependent on the member type functions
/// and show how these would be used.
//
// Starting from here, we have to provide implementations
// for helper/non-member type functions for type lists.
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
// Instead, we provide another namespace 'type_list'
// inside the 'meta' namespace.
namespace meta::type_list {

/// Check whether a given type list is empty.
template <instance::type_list list>
constexpr auto empty = list::empty;

/// Returns the count of types inside the type list.
template <instance::type_list list>
constexpr auto size = list::size;

/// Check whether the given type list contains the given type.
template <instance::type_list list, typename type>
static constexpr auto contains = list::template contains<type>;

// Access types inside the type list by their position.
template <instance::type_list list, size_t index>
requires(index < size<list>)  //
    using element = typename list::template element<index>;

/// Access the first type inside the type list.
template <instance::type_list list>
requires(!empty<list>)  //
    using front = typename list::front;

/// Access the last type inside the type list.
template <instance::type_list list>
requires(!empty<list>)  //
    using back = typename list::back;

/// Returns a new type list of all types positioned in [first,last).
template <instance::type_list list, size_t first, size_t last>
requires(first <= last) && (last <= size<list>)  //
    using range = typename list::template range<first, last>;

/// Appends the given type at the front of the type list.
template <instance::type_list list, typename type>
using push_front = typename list::template push_front<type>;

/// Appends the given type at the end of the type list.
template <instance::type_list list, typename type>
using push_back = typename list::template push_back<type>;

/// Insert the given type at the given position inside the type list.
template <instance::type_list list, size_t index, typename type>
requires(index <= size<list>)  //
    using insert = typename list::template insert<index, type>;

/// Insert the given type into the type list at the first position
/// where the condition predicate evaluates to true.
template <instance::type_list list,
          typename type,
          template <typename, typename>
          typename condition>
using insert_when = typename list::template sort_insert<type, condition>;

/// Remove a type inside the type list at the given position.
template <instance::type_list list, size_t index>
requires(index < size<list>)  //
    using remove = typename list::template remove<index>;

///
template <instance::type_list x, instance::type_list y>
using concatenation = typename x::template concat<y>;

/// Get a type list with reversed order of all contained types.
template <instance::type_list list>
using reverse = typename list::reverse;

/// Sort all contained types by the given order relation predicate.
template <instance::type_list list, template <typename, typename> typename less>
using sort = typename list::template sort<less>;

/// Swap two contained types by their given indices.
template <instance::type_list list, size_t i, size_t j>
requires(i < size<list>) && (j < size<list>)  //
    using swap = typename list::template swap<i, j>;

}  // namespace meta::type_list

/// Forward declarations for type function details.
// This is needed for member type functions.
// Also, for every using declaration providing a non-member type function,
// an implementation given by struct specializations has to be declared.
//
// Every type function implementation is provided as its own template.
namespace detail::type_list {
using xstd::type_list;

template <size_t index, typename... types>
struct element;
// Empty lists have no type access.
template <size_t index>
struct element<index> {};
// The first type can be accessed by index zero.
template <typename t, typename... types>
struct element<0, t, types...> {
  using type = t;
};
// For all other indices, we use a recursive call
// to elements with a reduced pack extension and index.
template <size_t index, typename t, typename... types>
struct element<index, t, types...> {
  using type = typename element<index - 1, types...>::type;
};

template <typename... types>
struct front_test;
template <instance::type_list list>
struct front;
template <instance::type_list list>
struct back;

template <instance::type_list x, instance::type_list y>
struct concatenation;
template <typename... t, typename... u>
struct concatenation<type_list<t...>, type_list<u...>> {
  using type = type_list<t..., u...>;
};

template <instance::type_list list, typename type>
struct push_back;
template <instance::type_list list, typename type>
struct push_front;
template <instance::type_list list>
struct pop_front;
template <instance::type_list list>
struct pop_back;
template <instance::type_list list>
struct reverse;
template <typename T, size_t count>
struct trim_front;
template <typename T, size_t count>
struct trim_back;
template <instance::type_list list, size_t first, size_t last>
struct range;

template <typename T, size_t i, size_t j>
struct swap;
template <typename T, size_t i, size_t j>
struct swap {
  using type = typename T::template range<0, i>::
      template push_back<typename T::template element<j>>::template concat<
          typename T::template range<i + 1, j>>::
          template push_back<typename T::template element<i>>::template concat<
              typename T::template range<j + 1, T::size>>;
};
template <typename T, size_t i, size_t j>
requires(j < i)  //
    struct swap<T, i, j> {
  using type = typename swap<T, j, i>::type;
};
template <typename T, size_t i>
struct swap<T, i, i> {
  using type = T;
};

template <typename T, size_t index, typename U>
struct insert;
template <typename T, size_t index>
struct remove;

template <typename T, typename U, template <typename, typename> typename less>
struct merge;
template <template <typename, typename> typename less>
struct merge<type_list<>, type_list<>, less> {
  using type = type_list<>;
};
template <template <typename, typename> typename less, typename... T>
struct merge<type_list<T...>, type_list<>, less> {
  using type = type_list<T...>;
};
template <template <typename, typename> typename less, typename... T>
struct merge<type_list<>, type_list<T...>, less> {
  using type = type_list<T...>;
};
template <template <typename, typename> typename less,
          typename T,
          typename U,
          typename... t,
          typename... u>
requires less<T, U>::value  //
    struct merge<type_list<T, t...>, type_list<U, u...>, less> {
  using type = typename push_front<
      typename merge<type_list<t...>, type_list<U, u...>, less>::type,
      T>::type;
};
template <template <typename, typename> typename less,
          typename T,
          typename U,
          typename... t,
          typename... u>
requires(!less<T, U>::value)  //
    struct merge<type_list<T, t...>, type_list<U, u...>, less> {
  using type = typename push_front<
      typename merge<type_list<T, t...>, type_list<u...>, less>::type,
      U>::type;
};

template <typename T, template <typename, typename> typename less>
struct sort;
template <typename list, template <typename, typename> typename less>
struct sort {
  static constexpr size_t half = list::size / 2;
  using type = typename merge<
      typename sort<typename range<list, 0, half>::type, less>::type,
      typename sort<typename range<list, half, list::size>::type, less>::type,
      less>::type;
};
template <typename list, template <typename, typename> typename less>
requires(list::size <= 1)  //
    struct sort<list, less> {
  using type = list;
};

template <typename T, typename U, template <typename, typename> typename less>
struct sort_insert;
template <typename T, typename U, template <typename, typename> typename less>
requires T::empty  //
    struct sort_insert<T, U, less> {
  using type = typename T::template push_back<U>;
};
template <typename T, typename U, template <typename, typename> typename less>
requires less<U, typename T::front>::value  //
    struct sort_insert<T, U, less> {
  using type = typename T::template push_front<U>;
};
template <typename T, typename U, template <typename, typename> typename less>
requires(!less<U, typename T::front>::value)  //
    struct sort_insert<T, U, less> {
  using type = typename sort_insert<typename T::pop_front, U, less>::type::
      template push_front<typename T::front>;
};

template <>
struct front<type_list<>> {};
template <typename t, typename... types>
struct front<type_list<t, types...>> {
  using type = t;
};

template <typename t>
struct back<type_list<t>> {
  using type = t;
};
template <typename t, typename... types>
struct back<type_list<t, types...>> {
  using type = typename back<type_list<types...>>::type;
};

template <instance::type_list list, size_t first, size_t last>
struct range {
  using type =
      typename trim_front<typename trim_back<list, list::size - last>::type,
                          first>::type;
};

//
template <typename t, typename... types>
struct push_back<type_list<types...>, t> {
  using type = type_list<types..., t>;
};

//
template <typename t, typename... types>
struct push_front<type_list<types...>, t> {
  using type = type_list<t, types...>;
};

//
template <typename t, typename... types>
struct pop_front<type_list<t, types...>> {
  using type = type_list<types...>;
};

//
template <>
struct pop_back<type_list<>> {};
template <typename t>
struct pop_back<type_list<t>> {
  using type = type_list<>;
};
template <typename t, typename... types>
struct pop_back<type_list<t, types...>> {
  using type = typename push_front<typename pop_back<type_list<types...>>::type,
                                   t>::type;
};

template <typename T, size_t count>
struct trim_front {
  using type = typename trim_front<typename T::pop_front, count - 1>::type;
};
template <typename T>
struct trim_front<T, 0> {
  using type = T;
};

//
template <typename T, size_t count>
struct trim_back {
  using type = typename trim_back<typename T::pop_back, count - 1>::type;
};
template <typename T>
struct trim_back<T, 0> {
  using type = T;
};

template <typename T, size_t index, typename U>
struct insert {
  using type = typename T::template range<0, index>::template push_back<
      U>::template concat<typename T::template range<index, T::size>>;
};

template <typename T, size_t index>
struct remove {
  using type = typename T::template range<0, index>::template concat<
      typename T::template range<index + 1, T::size>>;
};

template <>
struct reverse<type_list<>> {
  using type = type_list<>;
};
template <typename t, typename... types>
struct reverse<type_list<t, types...>> {
  using type =
      typename push_back<typename reverse<type_list<types...>>::type, t>::type;
};

template <typename... types>
struct base {
  using self = type_list<types...>;

  static constexpr size_t size = sizeof...(types);

  static constexpr bool empty = (size == 0);

  template <typename type>
  static constexpr bool contains = (meta::equal<type, types> || ...);

  template <size_t index>
  // requires(index <= sizeof...(types))  //
  // using element = typename detail::type_list::element<index, T,
  // types...>::type;
  using element = typename detail::type_list::element<index, types...>::type;

  template <typename type>
  using push_front = typename detail::type_list::push_front<self, type>::type;

  template <typename type>
  using push_back = typename detail::type_list::push_back<self, type>::type;

  template <size_t index, typename type>
  using insert = typename detail::type_list::insert<self, index, type>::type;

  template <size_t index>
  using remove = typename detail::type_list::remove<self, index>::type;

  template <size_t first, size_t last>
  using range = typename detail::type_list::range<self, first, last>::type;

  // using pop_front = typename detail::type_list::pop_front<self>::type;
  // using pop_back = typename detail::type_list::pop_back<self>::type;

  template <size_t count>
  using trim_front = typename detail::type_list::trim_front<self, count>::type;

  template <size_t count>
  using trim_back = typename detail::type_list::trim_back<self, count>::type;

  template <typename list>
  using concat = typename detail::type_list::concatenation<self, list>::type;

  using reverse = typename detail::type_list::reverse<self>::type;

  template <size_t i, size_t j>
  using swap = typename detail::type_list::swap<self, i, j>::type;

  template <template <typename, typename> typename less>
  using sort = typename detail::type_list::sort<self, less>::type;

  template <typename type, template <typename, typename> typename condition>
  using sort_insert =
      typename detail::type_list::sort_insert<self, type, condition>::type;
};

}  // namespace detail::type_list

// template <typename... types>
// struct type_list_base {
//   using self = type_list<types...>;

//   static constexpr size_t size = sizeof...(types);

//   static constexpr bool empty = (size == 0);

//   template <typename type>
//   static constexpr bool contains = (meta::equal<type, types> || ...);

//   template <size_t index>
//   using element = detail::type_list::element<index, types...>;

//   // These type functions have to be type-dependent.
//   // Otherwise, the empty list will exhibit compiler error.
//   // using front = detail::type_list::front_test<types...>::type;
//   // using back = detail::type_list::back<self>::type;

//   template <size_t first, size_t last>
//   using range = detail::type_list::range<self, first, last>::type;

//   template <typename type>
//   using push_front = typename detail::type_list::push_front<self,
//   type>::type;

//   template <typename type>
//   using push_back = typename detail::type_list::push_back<self, type>::type;

//   // using pop_front = typename detail::type_list::pop_front<self>::type;
//   // using pop_back = typename detail::type_list::pop_back<self>::type;

//   template <size_t index, typename type>
//   using insert = typename detail::type_list::insert<self, index, type>::type;

//   template <size_t index>
//   using remove = typename detail::type_list::remove<self, index>::type;

//   template <size_t count>
//   using trim_front = typename detail::type_list::trim_front<self,
//   count>::type;

//   template <size_t count>
//   using trim_back = typename detail::type_list::trim_back<self, count>::type;

//   template <instance::type_list list>
//   using concat = typename detail::type_list::concatenation<self, list>::type;

//   using reverse = typename detail::type_list::reverse<self>::type;

//   template <size_t i, size_t j>
//   using swap = typename detail::type_list::swap<self, i, j>::type;

//   template <template <typename, typename> typename less>
//   using sort = typename detail::type_list::sort<self, less>::type;

//   template <typename type, template <typename, typename> typename less>
//   using sort_insert =
//       typename detail::type_list::sort_insert<self, type, less>::type;
// };

// template <typename... types>
// struct type_list : type_list_base<types...> {
//   using self = type_list<types...>;
//   using front = detail::type_list::front<self>::type;
//   using back = detail::type_list::back<self>::type;
//   using pop_front = typename detail::type_list::pop_front<self>::type;
//   using pop_back = typename detail::type_list::pop_back<self>::type;
//   // using reverse = typename detail::type_list::reverse<self>::type;
// };
// template <>
// struct type_list<> : type_list_base<> {};

// template <typename... types>
// struct type_list_base {
//   using self = type_list<types...>;

//   static constexpr size_t size = sizeof...(types);

//   static constexpr bool empty = (size == 0);

//   template <typename type>
//   static constexpr bool contains = (meta::equal<type, types> || ...);

//   template <size_t index>
//   // requires(index <= sizeof...(types))  //
//   // using element = typename detail::type_list::element<index, T,
//   // types...>::type;
//   using element = typename detail::type_list::element<index, types...>::type;

//   template <typename type>
//   using push_front = typename detail::type_list::push_front<self,
//   type>::type;

//   template <typename type>
//   using push_back = typename detail::type_list::push_back<self, type>::type;

//   template <size_t index, typename type>
//   using insert = typename detail::type_list::insert<self, index, type>::type;

//   template <size_t index>
//   using remove = typename detail::type_list::remove<self, index>::type;

//   template <size_t first, size_t last>
//   using range = typename detail::type_list::range<self, first, last>::type;

//   // using pop_front = typename detail::type_list::pop_front<self>::type;
//   // using pop_back = typename detail::type_list::pop_back<self>::type;

//   template <size_t count>
//   using trim_front = typename detail::type_list::trim_front<self,
//   count>::type;

//   template <size_t count>
//   using trim_back = typename detail::type_list::trim_back<self, count>::type;

//   template <typename list>
//   using concat = typename detail::type_list::concatenation<self, list>::type;

//   using reverse = typename detail::type_list::reverse<self>::type;

//   template <size_t i, size_t j>
//   using swap = typename detail::type_list::swap<self, i, j>::type;

//   template <template <typename, typename> typename less>
//   using sort = typename detail::type_list::sort<self, less>::type;

//   template <typename type, template <typename, typename> typename condition>
//   using sort_insert =
//       typename detail::type_list::sort_insert<self, type, condition>::type;
// };

/// The type specialization for an empty type list.
template <>
struct type_list<> : detail::type_list::base<> {
  // static constexpr size_t size = 0;
  // static constexpr bool empty = true;
  // template <typename T>
  // static constexpr bool contains = false;
  using self = type_list<>;
  // template <typename T>
  // using push_back = type_list<T>;
  // template <typename T>
  // using push_front = type_list<T>;
  // using reverse = type_list<>;
  // template <typename U>
  // using concat = typename detail::type_list::concatenation<self, U>::type;

  // template <size_t index, typename T>
  // requires(index == 0)  //
  //     using insert = type_list<T>;

  // template <size_t count>
  // requires(count == 0)  //
  //     using trim_front = self;

  // template <size_t count>
  // requires(count == 0)  //
  //     using trim_back = self;

  // template <template <typename, typename> typename>
  // using sort = self;
  // template <template <typename, typename> typename less>
  // using sort = typename detail::type_list::sort<self, less>::type;

  // template <typename U, template <typename, typename> typename less>
  // using sort_insert = type_list<U>;
};

/// The type specialization for a non-empty type list.
template <typename T, typename... types>
struct type_list<T, types...> : detail::type_list::base<T, types...> {
  using base = detail::type_list::base<T, types...>;
  // using base::element;
  using base::size;
  // static constexpr size_t size = 1 + sizeof...(types);
  // static constexpr bool empty = false;

  // template <typename U>
  // static constexpr bool contains = meta::equal<U, T> ||
  //                                  (meta::equal<U, types> || ...);

  // template <size_t index>
  // // requires(index <= sizeof...(types))  //
  // // using element = typename detail::type_list::element<index, T,
  // // types...>::type;
  // using element = typename detail::type_list::element<index, T,
  // types...>::type;

  using front = typename base::template element<0>;
  using back = typename base::template element<size - 1>;

  using self = type_list<T, types...>;

  // template <typename U>
  // using push_back = type_list<T, types..., U>;

  // template <typename U>
  // using push_front = type_list<U, T, types...>;

  // using pop_front = type_list<types...>;

  // using reverse = typename type_list<types...>::reverse::template
  // push_back<T>;

  // using pop_back = typename reverse::pop_front::reverse;

  using pop_front = typename detail::type_list::pop_front<self>::type;
  using pop_back = typename detail::type_list::pop_back<self>::type;

  // template <typename U>
  // using concat = typename detail::type_list::concatenation<self, U>::type;

  // template <size_t count>
  // using trim_front = typename detail::type_list::trim_front<self,
  // count>::type;

  // template <size_t count>
  // using trim_back = typename detail::type_list::trim_back<self, count>::type;

  // template <size_t i, size_t j>
  // using swap = typename detail::type_list::swap<self, i, j>::type;

  // template <size_t first, size_t last>
  // using range = typename trim_back<size - last>::template trim_front<first>;

  // template <size_t index, typename U>
  // using insert = typename detail::type_list::insert<self, index, U>::type;

  // template <size_t index>
  // // using remove = typename detail::type_list::remove<self, index>::type;
  // using remove =
  //     typename detail::type_list::concatenation<range<0, index>,
  //                                               range<index + 1,
  //                                               size>>::type;

  // template <template <typename, typename> typename less>
  // using sort = typename detail::type_list::sort<self, less>::type;

  // template <typename U, template <typename, typename> typename less>
  // using sort_insert =
  //     typename detail::type_list::sort_insert<self, U, less>::type;
};

// Starting from here, we have to provide implementations
// for accessors and modifiers for a type list.
// The namespace 'detail' is used globally inside this library.
// As a consequence, different templates may need to use
// the same names for their type functions.
// In C++, using declarations cannot be overloaded or specialized.
// Concerning template parameters, only functions are allowed
// to overload them by using concepts or values.
// The problem is that each function is only able
// to return a value and not a type.
// This means that in either case
// the interface cannot be uniform and consistent.
// Instead, we provide another namespace 'type_list'
// inside the 'detail' namespace.
namespace detail::type_list {
// We need this using declaration.
// Otherwise all type functions will see 'type_list' as namespace.
using xstd::type_list;

//
// template <typename t, typename u>
// struct concatenation;
// template <typename... t, typename... u>
// struct concatenation<type_list<t...>, type_list<u...>> {
//   using type = type_list<t..., u...>;
// };

// // Empty lists have no type access.
// template <size_t index>
// struct element<index> {};
// // The first type can be accessed by index zero.
// template <typename t, typename... types>
// struct element<0, t, types...> {
//   using type = t;
// };
// // For all other indices, we use a recursive call
// // to elements with a reduced pack extension and index.
// template <size_t index, typename t, typename... types>
// struct element<index, t, types...> {
//   using type = typename element<index - 1, types...>::type;
// };

// template <instance::type_list list, size_t index>
// struct element{
//   using type = typename element<typename list::pop_front,index-1>::type;
// }
// template <instance::type_list list>
// struct element<list, 0>{
//   using type = typename list::front;
// }

// template <typename... types>
// struct front_test {};
// template <typename t, typename... types>
// struct front_test<t, types...> {
//   using type = t;
// };

// template <>
// struct front<type_list<>> {};
// template <typename t, typename... types>
// struct front<type_list<t, types...>> {
//   using type = t;
// };

// template <typename t>
// struct back<type_list<t>> {
//   using type = t;
// };
// template <typename t, typename... types>
// struct back<type_list<t, types...>> {
//   using type = typename back<type_list<types...>>::type;
// };

// //
// template <typename t, typename... types>
// struct push_back<type_list<types...>, t> {
//   using type = type_list<types..., t>;
// };

// //
// template <typename t, typename... types>
// struct push_front<type_list<types...>, t> {
//   using type = type_list<t, types...>;
// };

// //
// template <typename t, typename... types>
// struct pop_front<type_list<t, types...>> {
//   using type = type_list<types...>;
// };

// //
// template <typename t>
// struct pop_back<type_list<t>> {
//   using type = type_list<>;
// };
// template <typename t, typename... types>
// struct pop_back<type_list<t, types...>> {
//   using type = typename push_front<typename
//   pop_back<type_list<types...>>::type,
//                                    t>::type;
// };

//
// template <>
// struct reverse<type_list<>> {
//   using type = type_list<>;
// };
// template <typename t, typename... types>
// struct reverse<type_list<t, types...>> {
//   using type =
//       typename push_back<typename reverse<type_list<types...>>::type,
//       t>::type;
// };

//
// template <typename T, size_t count>
// struct trim_front {
//   using type = typename trim_front<typename T::pop_front, count - 1>::type;
// };
// template <typename T>
// struct trim_front<T, 0> {
//   using type = T;
// };

// //
// template <typename T, size_t count>
// struct trim_back {
//   using type = typename trim_back<typename T::pop_back, count - 1>::type;
// };
// template <typename T>
// struct trim_back<T, 0> {
//   using type = T;
// };

//
// template <instance::type_list list, size_t first, size_t last>
// struct range {
//   using type =
//       typename trim_front<typename trim_back<list, list::size - last>::type,
//                           first>::type;
// };

//
// template <typename T, size_t i, size_t j>
// struct swap {
//   using type = typename T::template range<0, i>::
//       template push_back<typename T::template element<j>>::template concat<
//           typename T::template range<i + 1, j>>::
//           template push_back<typename T::template element<i>>::template
//           concat<
//               typename T::template range<j + 1, T::size>>;
// };
// template <typename T, size_t i, size_t j>
// requires(j < i)  //
//     struct swap<T, i, j> {
//   using type = typename swap<T, j, i>::type;
// };
// template <typename T, size_t i>
// struct swap<T, i, i> {
//   using type = T;
// };

//
// template <typename T, size_t index, typename U>
// struct insert {
//   using type = typename T::template range<0, index>::template push_back<
//       U>::template concat<typename T::template range<index, T::size>>;
// };
// template <typename T, size_t index, typename U>
// requires(index == 0)  //
//     struct insert<T, index, U> {
//   using type = typename T::template push_front<U>;
// };
// template <typename T, size_t index, typename U>
// requires(index == T::size)  //
//     struct insert<T, index, U> {
//   using type = typename T::template push_back<U>;
// };

//
// template <typename T, size_t index>
// struct remove {
//   using type = typename T::template range<0, index>::template concat<
//       typename T::template range<index + 1, T::size>>;
// };

// Merge algorithm
// template <typename T, typename U, template <typename, typename> typename
// less> struct merge;
// template <template <typename, typename> typename less>
// struct merge<type_list<>, type_list<>, less> {
//   using type = type_list<>;
// };
// template <template <typename, typename> typename less, typename... T>
// struct merge<type_list<T...>, type_list<>, less> {
//   using type = type_list<T...>;
// };
// template <template <typename, typename> typename less, typename... T>
// struct merge<type_list<>, type_list<T...>, less> {
//   using type = type_list<T...>;
// };
// template <template <typename, typename> typename less,
//           typename T,
//           typename U,
//           typename... t,
//           typename... u>
// requires less<T, U>::value  //
//     struct merge<type_list<T, t...>, type_list<U, u...>, less> {
//   using type = typename push_front<
//       typename merge<type_list<t...>, type_list<U, u...>, less>::type,
//       T>::type;
// };
// template <template <typename, typename> typename less,
//           typename T,
//           typename U,
//           typename... t,
//           typename... u>
// requires(!less<T, U>::value)  //
//     struct merge<type_list<T, t...>, type_list<U, u...>, less> {
//   using type = typename push_front<
//       typename merge<type_list<T, t...>, type_list<u...>, less>::type,
//       U>::type;
// };

// //
// template <typename list, template <typename, typename> typename less>
// struct sort {
//   static constexpr size_t half = list::size / 2;
//   using type = typename merge<
//       typename sort<typename range<list, 0, half>::type, less>::type,
//       typename sort<typename range<list, half, list::size>::type,
//       less>::type, less>::type;
// };
// template <typename list, template <typename, typename> typename less>
// requires(list::size <= 1)  //
//     struct sort<list, less> {
//   using type = list;
// };

//
// template <typename T, typename U, template <typename, typename> typename
// less> struct sort_insert; template <typename T, typename U, template
// <typename, typename> typename less> requires T::empty  //
//     struct sort_insert<T, U, less> {
//   using type = typename T::template push_back<U>;
// };
// template <typename T, typename U, template <typename, typename> typename
// less> requires less<U, typename T::front>::value  //
//     struct sort_insert<T, U, less> {
//   using type = typename T::template push_front<U>;
// };
// template <typename T, typename U, template <typename, typename> typename
// less> requires(!less<U, typename T::front>::value)  //
//     struct sort_insert<T, U, less> {
//   using type = typename sort_insert<typename T::pop_front, U, less>::type::
//       template push_front<typename T::front>;
// };

}  // namespace detail::type_list

// Non-Member Type Functions
// Need to be provided inside the meta namespace.
// namespace meta {

// template <instance::type_list list>
// struct detail::size<list> {
//   static constexpr size_t value = list::size;
// };

// template <instance::type_list list>
// struct detail::empty<list> {
//   static constexpr bool value = list::empty;
// };

// template <instance::type_list list, typename T>
// struct detail::contains<list, T> {
//   static constexpr bool value = list::template contains<T>;
// };

// template <instance::type_list list, std::integral auto index>
// requires(0 <= index) && (index < list::size)  //
//     struct detail::element<list, index> {
//   using type = typename list::template element<index>;
// };

// template <instance::type_list list>
// requires(!list::empty)  //
//     struct detail::front<list> {
//   using type = typename list::front;
// };

// template <instance::type_list list>
// requires(!list::empty)  //
//     struct detail::back<list> {
//   using type = typename list::back;
// };

// template <instance::type_list list, typename T>
// using push_back = typename list::template push_back<T>;

// template <instance::type_list list, typename T>
// using push_front = typename list::template push_front<T>;

// }  // namespace meta

}  // namespace lyrahgames::xstd
