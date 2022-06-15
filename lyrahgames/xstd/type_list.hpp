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
// The implementation of this idea is taken out inside the 'details'
// namespace by a typical template struct predicate specialization.
template <typename t>
struct is_type_list : std::false_type {};
template <typename... types>
struct is_type_list<type_list<types...>> : std::true_type {};
}  // namespace detail

/// To simplify the interface, we use templated variables as alias.
template <typename T>
constexpr bool is_type_list = detail::is_type_list<T>::value;

/// To simplify the usage as concept,
/// we also provide a concept alias inside the 'instance' namespace.
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
using insert_when = typename list::template insert_when<type, condition>;

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

/// Transform algorithm for a type list
/// which transforms every contained type according to a given predicate.
// We only provide this interface style
// because there is no technique in C++
// to provide variadic template predicates
// in a lambda expression style.
template <instance::type_list list, template <typename> typename f>
using transformation = typename list::template transformation<f>;

/// Generate code based on given lambda expression for each contained type.
template <instance::type_list list>
constexpr void for_each(auto&& f) {
  list::for_each(std::forward<decltype(f)>(f));
}

}  // namespace meta::type_list

/// Forward declarations for type function details.
// This is needed for member type functions.
// Also, for every using declaration providing a non-member type function,
// an implementation given by struct specializations has to be declared.
//
// Every type function implementation is provided as its own template.
namespace detail::type_list {

using xstd::type_list;

//
template <instance::type_list list>
struct size;
template <typename... types>
struct size<type_list<types...>> {
  static constexpr size_t value = sizeof...(types);
};

//
template <instance::type_list list>
struct empty {
  static constexpr bool value = size<list>::value == 0;
};

// Empty lists have no type access.
template <instance::type_list list, size_t index>
struct element {};
// The first type can be accessed by index zero.
template <typename front, typename... tail>
struct element<type_list<front, tail...>, 0> {
  using type = front;
};
// For all other indices, we use a recursive call
// to elements with a reduced pack extension and index.
template <size_t index, typename front, typename... tail>
struct element<type_list<front, tail...>, index> {
  using type = typename element<type_list<tail...>, index - 1>::type;
};

//
template <instance::type_list list>
struct front {};
template <typename t, typename... types>
struct front<type_list<t, types...>> {
  using type = t;
};

//
template <instance::type_list list>
struct back {};
template <typename t>
struct back<type_list<t>> {
  using type = t;
};
template <typename t, typename... types>
struct back<type_list<t, types...>> {
  using type = typename back<type_list<types...>>::type;
};

//
template <instance::type_list x, instance::type_list y>
struct concatenation {};
template <typename... t, typename... u>
struct concatenation<type_list<t...>, type_list<u...>> {
  using type = type_list<t..., u...>;
};

//
template <instance::type_list list, typename type>
struct push_back {};
template <typename t, typename... types>
struct push_back<type_list<types...>, t> {
  using type = type_list<types..., t>;
};

//
template <instance::type_list list, typename type>
struct push_front {};
template <typename t, typename... types>
struct push_front<type_list<types...>, t> {
  using type = type_list<t, types...>;
};

//
template <instance::type_list list>
struct pop_front {};
template <typename t, typename... types>
struct pop_front<type_list<t, types...>> {
  using type = type_list<types...>;
};

//
template <instance::type_list list>
struct pop_back {};
template <typename t>
struct pop_back<type_list<t>> {
  using type = type_list<>;
};
template <typename t, typename... types>
struct pop_back<type_list<t, types...>> {
  using type = typename push_front<typename pop_back<type_list<types...>>::type,
                                   t>::type;
};

//
template <instance::type_list list>
struct reverse {
  using type = list;
};
template <typename t, typename... types>
struct reverse<type_list<t, types...>> {
  using type =
      typename push_back<typename reverse<type_list<types...>>::type, t>::type;
};

//
template <instance::type_list list, size_t count>
requires(count <= list::size)  //
    struct trim_front {
  using type =
      typename trim_front<typename pop_front<list>::type, count - 1>::type;
};
template <instance::type_list list>
struct trim_front<list, 0> {
  using type = list;
};

//
template <instance::type_list list, size_t count>
requires(count <= list::size)  //
    struct trim_back {
  using type =
      typename trim_back<typename pop_back<list>::type, count - 1>::type;
};
template <instance::type_list list>
struct trim_back<list, 0> {
  using type = list;
};

//
template <instance::type_list list, size_t first, size_t last>
requires(first <= last) && (last <= list::size)  //
    struct range {
  using type =
      typename trim_front<typename trim_back<list, list::size - last>::type,
                          first>::type;
};

template <instance::type_list list, size_t i, size_t j>
struct swap;
template <instance::type_list list, size_t i, size_t j>
requires(i < j) && (j < list::size)  //
    struct swap<list, i, j> {
  using left = typename range<list, 0, i>::type;
  using middle = typename range<list, i + 1, j>::type;
  using right = typename range<list, j + 1, list::size>::type;
  using xi = typename element<list, i>::type;
  using xj = typename element<list, j>::type;
  using new_left = typename push_back<left, xj>::type;
  using new_right = typename push_front<right, xi>::type;
  using tmp = typename concatenation<new_left, middle>::type;
  using type = typename concatenation<tmp, new_right>::type;
};
template <instance::type_list list, size_t i, size_t j>
requires(j < i) && (i < list::size)  //
    struct swap<list, i, j> {
  using type = typename swap<list, j, i>::type;
};
template <instance::type_list list, size_t i>
struct swap<list, i, i> {
  using type = list;
};

//
template <instance::type_list list, size_t index, typename t>
requires(index <= list::size)  //
    struct insert {
  using left = typename range<list, 0, index>::type;
  using right = typename range<list, index, list::size>::type;
  using tmp = typename push_back<left, t>::type;
  using type = typename concatenation<tmp, right>::type;
};

//
template <typename list, size_t index>
requires(index < list::size)  //
    struct remove {
  using left = typename range<list, 0, index>::type;
  using right = typename range<list, index + 1, list::size>::type;
  using type = typename concatenation<left, right>::type;
};

//
template <instance::type_list left,
          instance::type_list right,
          template <typename, typename>
          typename less>
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

//
template <instance::type_list list, template <typename, typename> typename less>
struct sort {
  static constexpr size_t half = list::size / 2;
  using type = typename merge<
      typename sort<typename range<list, 0, half>::type, less>::type,
      typename sort<typename range<list, half, list::size>::type, less>::type,
      less>::type;
};
template <instance::type_list list, template <typename, typename> typename less>
requires(list::size <= 1)  //
    struct sort<list, less> {
  using type = list;
};

//
template <instance::type_list list,
          typename t,
          template <typename, typename>
          typename less>
struct insert_when;
template <typename t, template <typename, typename> typename less>
struct insert_when<type_list<>, t, less> {
  using type = type_list<t>;
};
template <typename t,
          typename front,
          template <typename, typename>
          typename less,
          typename... types>
struct insert_when<type_list<front, types...>, t, less> {
  using type = type_list<t, front, types...>;
};
template <typename t,
          typename front,
          template <typename, typename>
          typename less,
          typename... types>
requires(!less<t, front>::value)  //
    struct insert_when<type_list<front, types...>, t, less> {
  using type = typename push_front<
      typename insert_when<type_list<types...>, t, less>::type,
      front>::type;
};

//
template <instance::type_list list, template <typename> typename f>
struct transformation;
template <template <typename> typename f, typename... types>
struct transformation<type_list<types...>, f> {
  using type = type_list<typename f<types>::type...>;
};

template <typename... types>
struct base {
  using self = type_list<types...>;

  static constexpr size_t size = detail::type_list::size<self>::value;

  static constexpr bool empty = detail::type_list::empty<self>::value;

  template <typename type>
  static constexpr bool contains = (meta::equal<type, types> || ...);

  template <size_t index>
  // requires(index <= sizeof...(types))  //
  // using element = typename detail::type_list::element<index, T,
  // types...>::type;
  // using element = typename detail::type_list::element<index, types...>::type;
  using element = typename detail::type_list::element<self, index>::type;

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
  using insert_when =
      typename detail::type_list::insert_when<self, type, condition>::type;

  template <template <typename> typename f>
  using transformation =
      typename detail::type_list::transformation<self, f>::type;

  static constexpr auto for_each(auto&& f) {
    (f.template operator()<types>(), ...);
  }
};

}  // namespace detail::type_list

/// The type specialization for an empty type list.
/// An empty type list does not provide access to the front or back.
/// Also, to pop from the front or back makes no sense.
template <>
struct type_list<> : detail::type_list::base<> {};

/// The type specialization for a non-empty type list.
template <typename T, typename... types>
struct type_list<T, types...> : detail::type_list::base<T, types...> {
  using base = detail::type_list::base<T, types...>;
  using base::size;

  using self = type_list<T, types...>;

  using front = typename detail::type_list::front<self>::type;
  using back = typename detail::type_list::back<self>::type;

  using pop_front = typename detail::type_list::pop_front<self>::type;
  using pop_back = typename detail::type_list::pop_back<self>::type;
};

}  // namespace lyrahgames::xstd
