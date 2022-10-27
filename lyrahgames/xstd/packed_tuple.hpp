#pragma once
#include <lyrahgames/xstd/regular_tuple.hpp>
#include <lyrahgames/xstd/static_index_list.hpp>

namespace lyrahgames::xstd {

namespace detail::packed_tuple {

using xstd::type_list;
using xstd::value_list;

template <typename... types>
struct base {
  using list = type_list<types...>;
  static constexpr auto alignment_order = []<auto x, auto y> {
    return alignof(meta::type_list::element<list, x>) >=
           alignof(meta::type_list::element<list, y>);
  };
  using indices = meta::value_list::iota<list::size>;
  using permutation = meta::value_list::sort<indices, alignment_order>;
  static constexpr auto permutation_order = []<auto x, auto y> {
    return meta::value_list::element<permutation, x> <=
           meta::value_list::element<permutation, y>;
  };
  using inverse_permutation =
      meta::value_list::sort<indices, permutation_order>;
  using permuted_list = meta::type_list::permutation<list, permutation>;
  using type = xstd::regular_tuple_from_type_list<permuted_list>;
};

}  // namespace detail::packed_tuple

///
/// "Empty base optimization is prohibited if one of the empty base classes is
/// also the type or the base of the type of the first non-static data member,
/// since the two base subobjects of the same type are required to have
/// different addresses within the object representation of the most derived
/// type." [cppreference.com: Empty Base Optimization]
/// https://en.cppreference.com/w/cpp/language/ebo
///
template <typename... types>
struct packed_tuple : detail::packed_tuple::base<types...>::type {
  using traits = detail::packed_tuple::base<types...>;
  using base = typename traits::type;
  using permutation = typename traits::permutation;
  using inverse_permutation = typename traits::inverse_permutation;

  template <size_t index>
  using type = typename traits::list::template element<index>;

  static constexpr auto size() noexcept -> size_t { return sizeof...(types); }

  packed_tuple() = default;

  template <size_t... indices>
  packed_tuple(static_index_list<indices...>, auto&&... args)  //
      requires(size() == sizeof...(args))
      : base(forward_element<indices>(
            std::forward<decltype(args)>(args)...)...) {}

  packed_tuple(auto&&... args)  //
      requires(size() == sizeof...(args))
      : packed_tuple(permutation{}, std::forward<decltype(args)>(args)...) {}

  constexpr decltype(auto) tuple() & noexcept {
    return static_cast<base&>(*this);
  }
  constexpr decltype(auto) tuple() && noexcept {
    return static_cast<base&&>(*this);
  }
  constexpr decltype(auto) tuple() const& noexcept {
    return static_cast<const base&>(*this);
  }
  constexpr decltype(auto) tuple() const&& noexcept {
    return static_cast<const base&&>(*this);
  }
};

namespace detail {

// The implementation of this idea is taken out inside the 'details'
// namespace by a typical template struct predicate specialization.
//
template <typename t>
struct is_packed_tuple : std::false_type {};
template <typename... types>
struct is_packed_tuple<xstd::packed_tuple<types...>> : std::true_type {};

}  // namespace detail

/// To simplify the interface, we use templated variables as alias.
///
template <typename T>
constexpr bool is_packed_tuple = detail::is_packed_tuple<T>::value;

namespace instance {

template <typename T>
concept packed_tuple = is_packed_tuple<T>;

template <typename T>
concept reducible_packed_tuple = packed_tuple<reduction<T>>;

}  // namespace instance

/// Access the elements of a packed_tuple by their index.
///
template <size_t index>
constexpr decltype(auto) value(
    instance::reducible_packed_tuple auto&& t) noexcept {
  using namespace meta::static_index_list;
  using permutation =
      typename meta::reduction<decltype(t)>::inverse_permutation;
  return value<element<permutation, index>>(
      std::forward<decltype(t)>(t).tuple());
}

/// This function is needed to make structured bindings available.
/// Here, it is a simple wrapper function template for 'value'.
///
template <size_t index>
constexpr decltype(auto) get(
    instance::reducible_packed_tuple auto&& t) noexcept {
  return value<index>(std::forward<decltype(t)>(t));
}

}  // namespace lyrahgames::xstd

namespace std {

/// Provides support for using structured bindings with packed_tuple.
///
template <typename... T>
struct tuple_size<lyrahgames::xstd::packed_tuple<T...>> {
  static constexpr size_t value = lyrahgames::xstd::packed_tuple<T...>::size();
};

/// Provides support for using structured bindings with packed_tuple.
///
template <size_t N, typename... T>  //
struct tuple_element<N, lyrahgames::xstd::packed_tuple<T...>> {
  using type = typename lyrahgames::xstd::packed_tuple<T...>::template type<N>;
};

}  // namespace std
