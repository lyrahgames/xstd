#pragma once
#include <lyrahgames/xstd/type_list/common.hpp>
#include <lyrahgames/xstd/value_list/common.hpp>

namespace lyrahgames::xstd {

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
//
namespace meta::type_list {

/// Check whether a given type list is empty.
///
template <instance::type_list list>
constexpr auto empty = list::empty;

/// Returns the count of types inside the type list.
///
template <instance::type_list list>
constexpr auto size = list::size;

/// Returns the alignment of the whole type list
/// as if the list would be a struct or a tuple.
///
template <instance::type_list list>
constexpr auto alignment = list::alignment;

/// Returns the byte size of the whole type list
/// as if the type list would be a struct
/// only containing member variables with type and order
/// as given by the types of the type list.
///
template <instance::type_list list>
constexpr auto struct_byte_size = list::struct_byte_size;

/// Returns the byte size of the whole type list
/// as if the type list would be a tuple.
/// This means empty types should not increase the size of the tuple.
///
template <instance::type_list list>
constexpr auto tuple_byte_size = list::tuple_byte_size;

/// Returns the offset of an indexed type inside the type list
/// as if the type list would be a struct
/// only containing member variables with type and order
/// as given by the types of the type list.
///
template <instance::type_list list, size_t index>
requires(index < size<list>)  //
    constexpr auto struct_offset = list::template struct_offset<index>;

/// Returns the offset of an indexed type inside the type list
/// as if the type list would be a tuple.
/// This means empty types should not increase the size of the tuple.
///
template <instance::type_list list, size_t index>
requires(index < size<list>)  //
    constexpr auto tuple_offset = list::template tuple_offset<index>;

/// Returns the padding of an indexed type up to the following type
/// inside the type list as if the type list would be a struct
/// only containing member variables with type and order
/// as given by the types of the type list.
///
template <instance::type_list list, size_t index>
requires(index < size<list>)  //
    constexpr auto struct_padding = list::template struct_padding<index>;

/// Check whether the given type list contains the given type.
///
template <instance::type_list list, typename type>
constexpr auto contains = list::template contains<type>;

/// Access types inside the type list by their position.
///
template <instance::type_list list, size_t index>
requires(index < size<list>)  //
    using element = typename list::template element<index>;

/// Get back the first index of the given type contained in the type list.
///
template <instance::type_list list, typename type>
requires(contains<list, type>)  //
    constexpr auto index = list::template index<type>;

/// Access the first type inside the type list.
///
template <instance::type_list list>
requires(!empty<list>)  //
    using front = typename list::front;

/// Access the last type inside the type list.
///
template <instance::type_list list>
requires(!empty<list>)  //
    using back = typename list::back;

/// Returns a new type list of all types positioned in [first,last).
///
template <instance::type_list list, size_t first, size_t last>
requires(first <= last) && (last <= size<list>)  //
    using range = typename list::template range<first, last>;

/// Appends the given type at the front of the type list.
///
template <instance::type_list list, typename type>
using push_front = typename list::template push_front<type>;

/// Appends the given type at the end of the type list.
///
template <instance::type_list list, typename type>
using push_back = typename list::template push_back<type>;

/// Insert the given type at the given position inside the type list.
///
template <instance::type_list list, size_t index, typename type>
requires(index <= size<list>)  //
    using insert = typename list::template insert<index, type>;

/// Insert the given type into the type list at the first position
/// where the condition predicate evaluates to true.
///
template <instance::type_list list,
          typename type,
          template <typename, typename>
          typename condition>
using insert_when = typename list::template insert_when<type, condition>;

/// Remove a type inside the type list at the given position.
///
template <instance::type_list list, size_t index>
requires(index < size<list>)  //
    using remove = typename list::template remove<index>;

/// Assign a new type given by its index.
///
template <instance::type_list list, size_t index, typename type>
requires(index < size<list>)  //
    using assignment = typename list::template assignment<index, type>;

///
///
template <instance::type_list x, instance::type_list y>
using concatenation = typename x::template concat<y>;

/// Get a type list with reversed order of all contained types.
///
template <instance::type_list list>
using reverse = typename list::reverse;

/// Sort all contained types by the given order relation predicate.
///
template <instance::type_list list, template <typename, typename> typename less>
using sort = typename list::template sort<less>;

/// Swap two contained types by their given indices.
///
template <instance::type_list list, size_t i, size_t j>
requires(i < size<list>) && (j < size<list>)  //
    using swap = typename list::template swap<i, j>;

/// Transform algorithm for a type list
/// which transforms every contained type according to a given predicate.
/// We only provide this interface style
/// because there is no technique in C++
/// to provide variadic template predicates
/// in a lambda expression style.
///
template <instance::type_list list, template <typename> typename f>
using transformation = typename list::template transformation<f>;

/// Permute the elements of the given type list according to the given indices.
///
template <instance::type_list list, instance::index_list indices>
using permutation = typename list::template permutation<indices>;

/// Generate code based on given lambda expression for each contained type.
///
template <instance::type_list list>
constexpr void for_each(auto&& f) {
  list::for_each(std::forward<decltype(f)>(f));
}

///
///
template <instance::type_list list>
constexpr bool for_each_until(auto&& f) {
  return list::for_each_until(std::forward<decltype(f)>(f));
}

///
///
template <instance::type_list list>
constexpr auto apply(auto&& f) {
  return list::apply(std::forward<decltype(f)>(f));
}

}  // namespace meta::type_list

}  // namespace lyrahgames::xstd
