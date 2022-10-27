#pragma once
#include <tuple>
//
#include <lyrahgames/xstd/type_list/type_list.hpp>
#include <lyrahgames/xstd/value_list/value_list.hpp>

namespace lyrahgames::xstd {

// Multiple tuple structures may exist due to several reasons.
// All of them may be very important in different circumstances.
// Hence, a concept for generic tuples may be useful.
// Here, the requirements for structured bindings
// are used as a generalization mechanism.
//
namespace generic {

template <typename tuple_type, size_t index>
concept tuple_value_access = requires(tuple_type value) {
  {
    get<index>(value)
    } -> std::convertible_to<  //
        typename std::tuple_element<index, tuple_type>::type>;
};

/// Checks whether a given type fulfills the requirements of a generic tuple.
///
using namespace meta::value_list;
template <typename tuple_type>
concept tuple =  // Require applied check to be valid for all types.
    logic_and < transform <
    // Generate indices to get all types.
    iota<std::tuple_size<tuple_type>::value>,
// Check each tuple identifier for consistent access.
[]<size_t x>() { return tuple_value_access<tuple_type, x>; } >> ;

///
///
template <typename T>
concept reducible_tuple = tuple<reduction<T>>;

}  // namespace generic

namespace detail::tuple {

using xstd::type_list;
using xstd::value_list;

//
template <generic::tuple tuple_type, instance::value_list indices>
struct type_list_cast {};
//
template <generic::tuple tuple_type, size_t... indices>
struct type_list_cast<tuple_type, value_list<indices...>> {
  using type =
      type_list<typename std::tuple_element<indices, tuple_type>::type...>;
};

}  // namespace detail::tuple

namespace meta::tuple {

///
template <generic::tuple tuple_type>
constexpr auto size = std::tuple_size<tuple_type>::value;

/// This meta function returns a type list
/// that contains all types of the generic tuple in the same order.
///
template <generic::tuple tuple_type,
          instance::value_list indices = value_list::iota<size<tuple_type>>>
using type_list_cast =
    typename detail::tuple::type_list_cast<tuple_type, indices>::type;

}  // namespace meta::tuple

/// Helper Function for Generic Tuple Cast
///
template <generic::tuple tuple_type, size_t... indices>
constexpr auto tuple_cast(generic::tuple auto&& t,
                          value_list<indices...>) noexcept(  //
    noexcept(tuple_type{get<indices>(std::forward<decltype(t)>(t))...})) {
  // This implementation requires a templatized forward constructor.
  return tuple_type{get<indices>(std::forward<decltype(t)>(t))...};
}

/// Generic Tuple Cast
/// Copies one generic tuple type to another.
///
template <generic::tuple tuple_type>
constexpr auto tuple_cast(generic::tuple auto&& t) noexcept(  //
    noexcept(tuple_cast<tuple_type>(
        std::forward<decltype(t)>(t),
        meta::value_list::iota<
            std::tuple_size<std::decay_t<decltype(t)>>::value>{}))) {
  using namespace meta::value_list;
  constexpr auto size = std::tuple_size<std::decay_t<decltype(t)>>::value;
  return tuple_cast<tuple_type>(std::forward<decltype(t)>(t), iota<size>{});
}

}  // namespace lyrahgames::xstd
