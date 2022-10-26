#pragma once
#include <lyrahgames/xstd/value_list/value_list.hpp>

namespace lyrahgames::xstd {

/// Static index lists are value lists
/// that contain only unique static zstrings.
/// No inheritance is used, because
/// all value list meta functions still need to be applyable.
///
template <size_t... indices>
using static_index_list = value_list<indices...>;

namespace detail {
//
template <typename type>
struct is_static_index_list : std::false_type {};
//
template <size_t... indices>
struct is_static_index_list<static_index_list<indices...>> : std::true_type {};
}  // namespace detail

/// Meta function that checks whether
/// the given type is a static index list.
///
template <typename list>
constexpr bool is_static_index_list = detail::is_static_index_list<list>::value;

// The namespace 'instance' is always used to provide concepts
// that check whether a given type is
// the instance of a specific type template.
//
namespace instance {

/// The concept checks whether the given type
/// is a static index list.
///
template <typename T>
concept static_index_list = is_static_index_list<T>;

}  // namespace instance

// Meta functions need to be provided in a separate namespace,
// because there is no overload facility for using declarations.
//
namespace meta::static_index_list {

// The static index list is only an alias of a values list.
// So, all value list meta functions are enabled by using its namespace.
//
using namespace meta::value_list;

/// Check whether each contained inside a
/// static index list is smaller than the given integer.
///
template <instance::static_index_list list, size_t bound>
constexpr bool bounded =
    logic_and<transform<list, []<size_t x> { return x < bound; }>>;

}  // namespace meta::static_index_list

}  // namespace lyrahgames::xstd
