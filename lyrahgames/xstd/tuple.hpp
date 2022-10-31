#pragma once
#include <tuple>
//
#include <lyrahgames/xstd/static_index_list.hpp>
#include <lyrahgames/xstd/type_list/type_list.hpp>

namespace lyrahgames::xstd {

// Multiple tuple structures may exist due to several reasons.
// All of them may be very important in different circumstances.
// Hence, a concept for generic tuples may be useful.
// Here, the requirements for structured bindings
// are used as a generalization mechanism.
//

namespace generic {

using namespace meta::static_index_list;

template <typename tuple_type, size_t index>
concept tuple_get_access = requires(tuple_type value) {
  {
    get<index>(value)
    } -> std::convertible_to<  //
        typename std::tuple_element<index, tuple_type>::type>;
};

/// Checks whether a given type fulfills the requirements of a generic tuple.
///
template <typename tuple_type>
concept tuple =
    // Require applied check to be valid for all types.
    logic_and < transform <
    // Generate indices to get all types.
    iota<std::tuple_size<tuple_type>::value>,
// Check each tuple identifier for consistent access.
[]<size_t x>() { return tuple_get_access<tuple_type, x>; } >> ;

template <typename T>
concept reducible_tuple = tuple<reduction<T>>;

template <typename tuple_type, size_t index>
concept xstd_tuple_value_access = requires(tuple_type tuple) {
  {
    value<index>(tuple)
    } -> std::convertible_to<typename tuple_type::template type<index>>;
  // Only ask for specific type.
  // For an array, you would not want to provide a type list.
};

template <typename tuple_type>
concept xstd_tuple = logic_and < transform < iota<tuple_type::size()>,
[]<size_t index> { return xstd_tuple_value_access<tuple_type, index>; } >> ;

template <typename tuple_type>
concept reducible_xstd_tuple = xstd_tuple<reduction<tuple_type>>;

}  // namespace generic

namespace detail::tuple {

using xstd::static_index_list;
using xstd::type_list;

//
template <generic::tuple tuple_type, instance::static_index_list indices>
struct type_list_cast {};
//
template <generic::tuple tuple_type, size_t... indices>
struct type_list_cast<tuple_type, static_index_list<indices...>> {
  using type =
      type_list<typename std::tuple_element<indices, tuple_type>::type...>;
};

template <typename tuple_type, size_t index>
struct byte_offset;

}  // namespace detail::tuple

namespace generic {

template <typename tuple_type, size_t index>
concept tuple_member_offset = requires {
  {
    detail::tuple::byte_offset<tuple_type, index>::value
    } -> std::convertible_to<size_t>;
};

template <typename tuple_type>
concept static_layout_tuple =
    tuple<tuple_type> &&  //
    logic_and < transform < iota<std::tuple_size<tuple_type>::value>,
[]<size_t index> { return tuple_member_offset<tuple_type, index>; } >> ;

}  // namespace generic

namespace meta::tuple {

/// This meta function returns a type list
/// that contains all types of the generic tuple in the same order.
///
template <generic::tuple tuple_type,
          instance::static_index_list indices =
              static_index_list::iota<std::tuple_size<tuple_type>::value>>
using type_list_cast =
    typename detail::tuple::type_list_cast<tuple_type, indices>::type;

///
template <generic::static_layout_tuple tuple_type, size_t index>
constexpr auto byte_offset =
    detail::tuple::byte_offset<tuple_type, index>::value;

}  // namespace meta::tuple

/// Helper Function for Generic Tuple Cast
///
// template <generic::tuple tuple_type, size_t... indices>
// constexpr auto tuple_cast(generic::tuple auto&& t,
//                           static_index_list<indices...>) noexcept(  //
//     noexcept(tuple_type{get<indices>(std::forward<decltype(t)>(t))...})) {
//   // This implementation requires a templatized forward constructor.
//   return tuple_type{get<indices>(std::forward<decltype(t)>(t))...};
// }

/// Generic Tuple Cast
/// Copies one generic tuple type to another.
///
// template <generic::tuple tuple_type>
// constexpr auto tuple_cast(generic::tuple auto&& t) noexcept(  //
//     noexcept(tuple_cast<tuple_type>(
//         std::forward<decltype(t)>(t),
//         meta::static_index_list::iota<
//             std::tuple_size<std::decay_t<decltype(t)>>::value>{}))) {
//   using namespace meta::static_index_list;
//   constexpr auto size = std::tuple_size<std::decay_t<decltype(t)>>::value;
//   return tuple_cast<tuple_type>(std::forward<decltype(t)>(t), iota<size>{});
// }

// ///
// ///
// template <generic::tuple tuple_type, size_t... indices>
// constexpr void
// assign(tuple_type& x, static_index_list<indices...>, auto&&... args)
// noexcept(
//     noexcept(get<indices>(x) = forward_element<indices>(
//                   std::forward<decltype(args)>(args)...),
//               ...)) ){
//   (get<indices>(x) =
//        forward_element<indices>(std::forward<decltype(args)>(args)...),
//    ...);
// }

// ///
// ///
// template <generic::tuple tuple_type>
// constexpr void assign(tuple_type& x, auto&&... args) noexcept(  //
//     noexcept(assign(
//         x,
//         meta::static_index_list::iota<std::tuple_size<tuple_type>::value>{},
//         std::forward<decltype(args)>(args)...)))  //
//     requires(sizeof...(args) == std::tuple_size<tuple_type>::value) {
//   assign(x,
//   meta::static_index_list::iota<std::tuple_size<tuple_type>::value>{},
//          std::forward<decltype(args)>(args)...);
// }

///
// template <size_t index>
// constexpr decltype(auto) value(generic::reducible_std_tuple auto&& x)
// noexcept(
//     noexcept(get<index>(std::forward<decltype(x)>(x))))  //
//     requires(index < std::tuple_size<meta::reduction<decltype(x)>>::value) {
//   return get<index>(std::forward<decltype(x)>(x));
// }

///
// template <size_t index>
// constexpr decltype(auto) get(generic::reducible_xstd_tuple auto&& x)
// noexcept(
//     noexcept(value<index>(std::forward<decltype(x)>(x))))  //
//     requires(index < meta::reduction<decltype(x)>::size()) {
//   return value<index>(std::forward<decltype(x)>(x));
// }

}  // namespace lyrahgames::xstd

// namespace std {

// ///
// template <lyrahgames::xstd::generic::xstd_tuple tuple_type>
// struct tuple_size<tuple_type> {
//   static constexpr size_t value = tuple_type::size();
// };

// ///
// template <size_t index, lyrahgames::xstd::generic::xstd_tuple tuple_type>
// struct tuple_element<index, tuple_type> {
//   using type = typename tuple_type::template type<index>;
// };

// }  // namespace std
