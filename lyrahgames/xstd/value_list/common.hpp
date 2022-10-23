#pragma once
#include <cstddef>
#include <type_traits>
//
#include <lyrahgames/xstd/meta.hpp>

namespace lyrahgames::xstd {

/// The 'value_list' template should only be used
/// in the context of template meta programming.
/// It enables the access and modification of
/// constexpr values inside variadic templates.
template <auto... values>
struct value_list;

// Implementation Details for Value List Concepts
#include "detail/traits.ipp"

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

}  // namespace lyrahgames::xstd
