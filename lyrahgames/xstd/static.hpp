#pragma once
#include <lyrahgames/xstd/type_list.hpp>
#include <lyrahgames/xstd/value_list.hpp>

namespace lyrahgames::xstd {

namespace detail::value_list {

//
template <size_t size, auto offset, auto increment>
struct iota {
  using type = typename iota<size - 1, increment(offset), increment>::type::
      template push_front<offset>;
};
template <auto offset, auto increment>
struct iota<0, offset, increment> {
  using type = value_list<>;
};

}  // namespace detail::value_list

namespace meta::value_list {

inline constexpr auto default_increment = [](auto x) { return ++x; };

/// Get a value list with the given size
/// of consecutive values starting from
/// the given offset and generated
/// by the provided unary increment function.
/// The default increment uses '++offset'.
///
template <size_t size,
          auto offset = size_t{0},
          auto increment = default_increment>
using iota =
    typename xstd::detail::value_list::iota<size, offset, increment>::type;

}  // namespace meta::value_list

}  // namespace lyrahgames::xstd
