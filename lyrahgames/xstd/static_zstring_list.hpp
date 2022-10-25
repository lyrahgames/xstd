#pragma once
#include <lyrahgames/xstd/static_zstring.hpp>
#include <lyrahgames/xstd/value_list/value_list.hpp>

namespace lyrahgames::xstd {

/// Static zstring lists are value lists
/// that contain only static zstrings.
/// No inheritance is used, because
/// all value list meta functions still need to be applyable.
///
template <static_zstring... strings>
using static_zstring_list = value_list<strings...>;

namespace detail {
//
template <typename type>
struct is_static_zstring_list : std::false_type {};
//
template <static_zstring... strings>
struct is_static_zstring_list<static_zstring_list<strings...>>
    : std::true_type {};
}  // namespace detail

/// Meta function that checks whether
/// the given type is a static zstring list.
///
template <typename list>
constexpr bool is_static_zstring_list =
    detail::is_static_zstring_list<list>::value;

// The namespace 'instance' is always used to provide concepts
// that check whether a given type is
// the instance of a specific type template.
//
namespace instance {

/// The concept checks whether the given type
/// is a static zstring list.
///
template <typename T>
concept static_zstring_list = is_static_zstring_list<T>;

}  // namespace instance

// Meta functions need to be provided in a separate namespace,
// because there is no overload facility for using declarations.
//
namespace meta::static_zstring_list {

// The static zstring list is only an alias of a values list.
// So, all value list meta functions are enabled by using its namespace.
//
using namespace value_list;

}  // namespace meta::static_zstring_list

}  // namespace lyrahgames::xstd
