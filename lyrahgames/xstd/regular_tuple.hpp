#pragma once
#include <lyrahgames/xstd/functional.hpp>
#include <lyrahgames/xstd/reverse_tuple.hpp>
#include <lyrahgames/xstd/static_index_list.hpp>

// The C++ standard does not specify a memory layout to be used for tuples.
// For issues concerning the copy of memory, a custom tuple may be needed.
// It should provide a contiguous access to the given types in the same
// order as they are given in the template parameter list.

namespace lyrahgames::xstd {

/// The actual custom tuple structure.
/// Types are stored in the same order as they are given.
/// This includes alignment and padding as it would be done in structs.
///
template <typename... types>
struct regular_tuple;

/// For tight constraints in template requirements,
/// we want to be able to decide whether a type is
/// an instance of the 'regular_tuple' template.
///
namespace detail {

// The implementation of this idea is taken out inside the 'details'
// namespace by a typical template struct predicate specialization.
//
template <typename t>
struct is_regular_tuple : std::false_type {};
template <typename... types>
struct is_regular_tuple<regular_tuple<types...>> : std::true_type {};

//
template <instance::type_list list>
struct regular_tuple_from_type_list {};
//
template <typename... types>
struct regular_tuple_from_type_list<xstd::type_list<types...>> {
  using type = regular_tuple<types...>;
};

}  // namespace detail

/// To simplify the interface, we use templated variables as alias.
///
template <typename T>
constexpr bool is_regular_tuple = detail::is_regular_tuple<T>::value;

/// Get the regular tuple type from a given type list.
///
template <instance::type_list list>
using regular_tuple_from_type_list =
    typename detail::regular_tuple_from_type_list<list>::type;

/// To simplify the usage as concept,
/// we also provide a concept alias inside the 'instance' namespace.
///
namespace instance {

template <typename T>
concept regular_tuple = is_regular_tuple<T>;

template <typename T>
concept reducible_regular_tuple = regular_tuple<reduction<T>>;

}  // namespace instance

namespace detail::regular_tuple {

using xstd::regular_tuple;
using xstd::type_list;

//
template <instance::type_list types>
struct from_type_list {};
//
template <typename... types>
struct from_type_list<type_list<types...>> {
  using type = regular_tuple<types...>;
};

}  // namespace detail::regular_tuple

namespace meta::regular_tuple {

/// Returns an instance of 'regular_tuple' which
/// takes all the types given in the type list.
///
template <instance::type_list types>
using from_type_list =
    typename detail::regular_tuple::from_type_list<types>::type;

/// Returns the offset in bytes of the element
/// inside the structure given by the index.
///
template <instance::regular_tuple tuple_type, size_t index>
constexpr auto offset = meta::reverse_tuple::offset<
    typename tuple_type::tuple_type,
    tuple_type::permutation::template element<index>>;

}  // namespace meta::regular_tuple

namespace detail::tuple {

template <instance::regular_tuple tuple_type, size_t index>
struct byte_offset<tuple_type, index> {
  static constexpr size_t value =
      meta::regular_tuple::offset<tuple_type, index>;
};

}  // namespace detail::tuple

/// Access the elements of a regular_tuple by their index.
///
template <size_t index>
constexpr decltype(auto) value(
    instance::reducible_regular_tuple auto&& t) noexcept {
  using tuple_type = meta::reduction<decltype(t)>;
  using permutation = typename tuple_type::permutation;
  return value<permutation::template element<index>>(
      std::forward<decltype(t)>(t).tuple());
}

template <typename... T>
struct regular_tuple
    : meta::reverse_tuple::from_type_list<typename type_list<T...>::reverse> {
  using types = type_list<T...>;

  template <size_t index>
  using type = typename types::template element<index>;

  using tuple_type =
      meta::reverse_tuple::from_type_list<typename types::reverse>;
  using permutation =
      typename meta::static_index_list::iota<types::size>::reverse;

  static constexpr auto size() noexcept -> size_t { return types::size; }

  regular_tuple() = default;

  //
  template <size_t... indices>
  constexpr regular_tuple(static_index_list<indices...>, auto&&... args) noexcept(
      noexcept(tuple_type(forward_element<indices>(
          std::forward<decltype(args)>(args)...)...)))  //
      requires(size() == sizeof...(args))
      : tuple_type(forward_element<indices>(
            std::forward<decltype(args)>(args)...)...) {}

  // The forward constructor needs to call
  // the base tuple constructor in permuted order.
  //
  explicit constexpr regular_tuple(auto&&... args) noexcept(noexcept(
      regular_tuple(permutation{}, std::forward<decltype(args)>(args)...)))  //
      requires(size() == sizeof...(args))
      : regular_tuple(permutation{}, std::forward<decltype(args)>(args)...) {}

  // Generic Copy/Move Construction
  //
  // explicit constexpr regular_tuple(
  //     instance::reducible_regular_tuple auto&& x) noexcept(  //
  //     noexcept(tuple_type(std::forward<decltype(x)>(x).tuple())))
  //     : tuple_type(std::forward<decltype(x)>(x).tuple()) {}

  ///
  ///
  template <size_t... indices>
  constexpr regular_tuple(generic::reducible_tuple auto&& x,
                          static_index_list<indices...>) noexcept(  //
      noexcept(regular_tuple(get<indices>(std::forward<decltype(x)>(x))...)))
      : regular_tuple(get<indices>(std::forward<decltype(x)>(x))...) {}
  ///
  explicit constexpr regular_tuple(
      generic::reducible_tuple auto&& x) noexcept(  //
      noexcept(regular_tuple(std::forward<decltype(x)>(x),
                             meta::static_index_list::iota<size()>{})))
      : regular_tuple(std::forward<decltype(x)>(x),
                      meta::static_index_list::iota<size()>{}) {}

  // Generic Assignment Operator
  //
  // constexpr regular_tuple& operator=(
  //     instance::reducible_regular_tuple auto&& x) noexcept(  //
  //     noexcept(static_cast<tuple_type&>(*this) =
  //                  std::forward<decltype(x)>(x).tuple())) {
  //   static_cast<tuple_type&>(*this) = std::forward<decltype(x)>(x).tuple();
  //   return *this;
  // }

  // The inherited assign methods also provide the arguments in the wrong order.
  // So, we reorderd the arguments and define them again.
  template <size_t... indices>
  constexpr void assign(static_index_list<indices...>,
                        auto&&... args) noexcept(  //
      noexcept(static_cast<tuple_type&>(*this).assign(
          forward_element<permutation::template element<indices>>(
              std::forward<decltype(args)>(args)...)...)))  //
      requires(sizeof...(args) == size()) {
    static_cast<tuple_type&>(*this).assign(
        forward_element<permutation::template element<indices>>(
            std::forward<decltype(args)>(args)...)...);
  }

  constexpr void assign(auto&&... args) noexcept(  //
      noexcept(assign(meta::static_index_list::iota<size()>{},
                      std::forward<decltype(args)>(args)...)))  //
      requires(sizeof...(args) == size()) {
    assign(meta::static_index_list::iota<size()>{},
           std::forward<decltype(args)>(args)...);
  }

  template <size_t... indices>
  constexpr void assign(generic::reducible_tuple auto&& x,
                        static_index_list<indices...>) noexcept(  //
      noexcept(assign(get<indices>(std::forward<decltype(x)>(x))...))) {
    assign(get<indices>(std::forward<decltype(x)>(x))...);
  }

  constexpr regular_tuple& operator=(
      generic::reducible_tuple auto&& x) noexcept(  //
      noexcept(assign(std::forward<decltype(x)>(x),
                      meta::static_index_list::iota<size()>{}))) {
    assign(std::forward<decltype(x)>(x),
           meta::static_index_list::iota<size()>{});
    return *this;
  }

  friend auto operator<=>(const regular_tuple&, const regular_tuple&) = default;

  constexpr decltype(auto) tuple() & noexcept {
    return static_cast<tuple_type&>(*this);
  }
  constexpr decltype(auto) tuple() && noexcept {
    return static_cast<tuple_type&&>(*this);
  }
  constexpr decltype(auto) tuple() const& noexcept {
    return static_cast<const tuple_type&>(*this);
  }
  constexpr decltype(auto) tuple() const&& noexcept {
    return static_cast<const tuple_type&&>(*this);
  }
};

// Deduction Guides
//
template <typename... types>
regular_tuple(types&&...) -> regular_tuple<std::unwrap_ref_decay_t<types>...>;

///
///
// template <size_t... indices>
// constexpr auto auto_tuple(generic::tuple auto&& t,
//                           value_list<indices...>) noexcept(  //
//     noexcept(regular_tuple{get<indices>(std::forward<decltype(t)>(t))...})) {
//   return regular_tuple{get<indices>(std::forward<decltype(t)>(t))...};
// }

///
///
// constexpr auto auto_tuple(generic::tuple auto&& t) noexcept(  //
//     noexcept(
//         auto_tuple(std::forward<decltype(t)>(t),
//                    meta::value_list::iota<
//                        std::tuple_size<std::decay_t<decltype(t)>>::value>{})))
//                        {
//   using namespace meta::value_list;
//   constexpr auto size = std::tuple_size<std::decay_t<decltype(t)>>::value;
//   return auto_tuple(std::forward<decltype(t)>(t), iota<size>{});
// }

/// This function is needed to make structured bindings available.
/// Here, it is a simple wrapper function template for 'value'.
///
template <size_t index>
constexpr decltype(auto) get(
    instance::reducible_regular_tuple auto&& t) noexcept {
  return value<index>(std::forward<decltype(t)>(t));
}

}  // namespace lyrahgames::xstd

namespace std {

/// Provides support for using structured bindings with regular_tuple.
///
template <typename... T>
struct tuple_size<lyrahgames::xstd::regular_tuple<T...>> {
  static constexpr size_t value = lyrahgames::xstd::regular_tuple<T...>::size();
};

/// Provides support for using structured bindings with regular_tuple.
///
template <size_t N, typename... T>  //
struct tuple_element<N, lyrahgames::xstd::regular_tuple<T...>> {
  using type = typename lyrahgames::xstd::regular_tuple<T...>::template type<N>;
};

}  // namespace std
