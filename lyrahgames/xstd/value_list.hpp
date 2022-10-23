#pragma once
#include <lyrahgames/xstd/type_list.hpp>

namespace lyrahgames::xstd {

/// The 'value_list' template should only be used
/// in the context of template meta programming.
/// It enables the access and modification of
/// constexpr values inside variadic templates.
template <auto... values>
struct value_list;

// Implementation Details for Value List Concepts
#include "detail/value_list/traits.ipp"

/// For tight constraints in template requirements,
/// we want to be able to decide whether a type is
/// an instance of the 'value_list' template.
///
/// To simplify the interface, we use templated variables as alias.
///
template <typename T>
constexpr bool is_value_list = detail::is_value_list<T>::value;

///
///
template <typename list, template <auto> typename constraint>
constexpr bool is_constrained_value_list =
    detail::is_constrained_value_list<list, constraint>::value;

/// Checks, whether the given type is a value list
/// consisting only of values of a specific type.
///
template <typename list, typename type>
constexpr bool is_uniform_value_list =
    detail::is_uniform_value_list<list, type>::value;

/// To simplify the usage as concept,
/// we also provide a concept alias inside the 'instance' namespace.
///
namespace instance {

/// A fulfilling type is an instance of the 'value_list' template.
///
template <typename T>
concept value_list = is_value_list<T>;

template <typename T, template <auto> typename constraint>
concept constrained_value_list = is_constrained_value_list<T, constraint>;

/// A fulfilling type is an instance of the 'value_list' template
/// that contains only values of the given type.
///
template <typename list, typename type>
concept uniform_value_list = is_uniform_value_list<list, type>;

/// The type is a value list that only contains values of type 'size_t'.
///
template <typename list>
concept index_list = uniform_value_list<list, size_t>;

/// The type is a value list that only contains values of type 'bool'.
///
template <typename list>
concept bool_list = uniform_value_list<list, bool>;

}  // namespace instance

// Implementation Details for Value List Member and Non-Member Functions
//
#include "detail/value_list/functions.ipp"
#include "detail/value_list/struct.ipp"

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
//
namespace meta::value_list {

/// Check whether a given value list is empty.
///
template <instance::value_list list>
constexpr auto empty = list::empty;

/// Returns the count of values inside the value list.
///
template <instance::value_list list>
constexpr auto size = list::size;

/// Returns a list of the types of values inside the given value list.
///
template <instance::value_list list>
using types = typename list::types;

/// Check whether the given value list contains the given value.
///
template <instance::value_list list, auto value>
constexpr bool contains = list::template contains<value>;

///
///
template <instance::value_list list>
constexpr bool is_set = list::is_set;

/// Access values inside the value list by their position.
///
template <instance::value_list list, size_t index>
requires(index < size<list>)  //
    constexpr decltype(auto) element = list::template element<index>;

/// Get back the first index of the given value contained in the value list.
///
template <instance::value_list list, auto value>
requires(contains<list, value>)  //
    constexpr auto index = list::template index<value>;

/// Access the first value inside the value list.
///
template <instance::value_list list>
requires(!empty<list>)  //
    constexpr decltype(auto) front = list::front;

/// Access the last value inside the value list.
///
template <instance::value_list list>
requires(!empty<list>)  //
    constexpr decltype(auto) back = list::back;

/// Returns a new value list of all values positioned in [first,last).
///
template <instance::value_list list, size_t first, size_t last>
requires(first <= last) && (last <= size<list>)  //
    using range = typename list::template range<first, last>;

/// Appends the given value at the front of the value list.
///
template <instance::value_list list, auto value>
using push_front = typename list::template push_front<value>;

/// Appends the given value at the end of the value list.
///
template <instance::value_list list, auto value>
using push_back = typename list::template push_back<value>;

/// Insert the given value at the given position inside the type list.
///
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
///
template <instance::value_list list, size_t index>
requires(index < size<list>)  //
    using remove = typename list::template remove<index>;

///
///
template <instance::value_list list, auto value>
requires(contains<list, value>)  //
    using remove_value = typename list::template remove_value<value>;

///
///
template <instance::value_list list, auto value>
using try_remove_value = typename list::template try_remove_value<value>;

/// Assign a new value given by its index.
///
template <instance::value_list list, size_t index, auto value>
requires(index < size<list>)  //
    using assignment = typename list::template assignment<index, value>;

/// Concatenate two given value lists.
///
template <instance::value_list x, instance::value_list y>
using concatenation = typename x::template concat<y>;

/// Get a value list with reversed order of all contained values.
///
template <instance::value_list list>
using reverse = typename list::reverse;

/// Sort all contained types by the given order relation predicate.
///
template <instance::value_list list, auto less>
using sort = typename list::template sort<less>;

/// Swap two contained values by their given indices.
///
template <instance::value_list list, size_t i, size_t j>
requires(i < size<list>) && (j < size<list>)  //
    using swap = typename list::template swap<i, j>;

/// Transform algorithm for a value list
/// which transforms every contained value according to
/// a given unary constexpr function or lambda expression.
///
template <instance::value_list list, auto function>
using transform = typename list::template transform<function>;

/// Reduces the whole value list to a value
/// according to function and the initial value.
///
template <instance::value_list list, auto function, auto init>
constexpr auto reduce = list::template reduce<function, init>;

/// Generate code based on given lambda expression for each contained value.
///
template <instance::value_list list>
constexpr void for_each(auto&& f) {
  list::for_each(std::forward<decltype(f)>(f));
}

/// Statically run a for loop until the result
/// of the function evaluation becomes 'true'.
/// The function returns 'false' if no evaluation returned 'true'.
///
template <instance::value_list list>
constexpr bool for_each_until(auto&& f) {
  return list::for_each_until(std::forward<decltype(f)>(f));
}

/// Reduce the value list containing only of bools
/// by applying a logical 'and' operation.
///
template <instance::bool_list list>
constexpr auto logic_and =
    reduce<list, []<bool x, bool y> { return x && y; }, true>;

/// Reduce the value list containing only of bools
/// by applying a logical 'or' operation.
///
template <instance::bool_list list>
constexpr auto logic_or =
    reduce<list, []<bool x, bool y> { return x || y; }, false>;

}  // namespace meta::value_list

}  // namespace lyrahgames::xstd
