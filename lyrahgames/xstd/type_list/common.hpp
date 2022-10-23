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
///
template <typename... types>
struct type_list;

// Implementation Details for Type List Concepts
//
#include "detail/traits.ipp"

/// For tight constraints in template requirements,
/// we want to be able to decide whether a type is
/// an instance of the 'type_list' template.
///
/// To simplify the interface, we use templated variables as alias.
///
template <typename T>
constexpr bool is_type_list = detail::is_type_list<T>::value;

///
///
template <typename list, template <typename> typename constraint>
constexpr bool is_constrained_type_list =
    detail::is_constrained_type_list<list, constraint>::value;

/// To simplify the usage as concept,
/// we also provide a concept alias inside the 'instance' namespace.
namespace instance {

template <typename T>
concept type_list = is_type_list<T>;

template <typename T, template <typename> typename constraint>
concept constrained_type_list = is_constrained_type_list<T, constraint>;

}  // namespace instance

}  // namespace lyrahgames::xstd
