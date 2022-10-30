#pragma once
#include <lyrahgames/xstd/static_identifier_list.hpp>
#include <lyrahgames/xstd/static_index_list.hpp>
#include <lyrahgames/xstd/tuple.hpp>

namespace lyrahgames::xstd {

namespace generic {

template <typename tuple_type, static_zstring name>
concept named_tuple_value_access = requires(tuple_type x) {
  {
    value<name>(x)
    } -> std::convertible_to<typename tuple_type::template type<name>>;
};

/// Checks whether a given type fulfills the requirements of a generic tuple.
///
using namespace meta::value_list;
template <typename tuple_type>
concept named_tuple = tuple<tuple_type> &&
    // Require applied check to be valid for all types.
    logic_and<transform<
        // Generate indices to get all types.
        typename tuple_type::names,
        // Check each tuple identifier for consistent access.
        []<static_zstring x>() {
          return named_tuple_value_access<tuple_type, x>;
        }>>;

///
///
template <typename T>
concept reducible_named_tuple = tuple<reduction<T>>;

template <typename tuple_type>
concept unnamed_tuple = tuple<tuple_type> &&(!named_tuple<tuple_type>);
template <typename tuple_type>
concept reducible_unnamed_tuple = unnamed_tuple<reduction<tuple_type>>;

}  // namespace generic

template <instance::static_identifier_list identifiers, generic::tuple T>
requires(identifiers::size == std::tuple_size<T>::value)  //
    struct named_tuple;

namespace detail {
template <typename type>
struct is_named_tuple : std::false_type {};
template <instance::static_identifier_list names, generic::tuple tuple_type>
struct is_named_tuple<named_tuple<names, tuple_type>> : std::true_type {};
}  // namespace detail

template <typename T>
constexpr bool is_named_tuple = detail::is_named_tuple<T>::value;

namespace instance {

///
template <typename T>
concept named_tuple = is_named_tuple<T>;

///
template <typename T>
concept reducible_named_tuple = named_tuple<reduction<T>>;

}  // namespace instance

template <instance::static_identifier_list identifiers, generic::tuple T>
requires(identifiers::size == std::tuple_size<T>::value)  //
    struct named_tuple : T {
  using names = identifiers;
  using tuple_type = T;
  using types = meta::tuple::type_list_cast<tuple_type>;

  template <static_zstring name>
  using type = typename types::template element<names::template index<name>>;

  static constexpr auto size() noexcept -> size_t { return tuple_type::size(); }

  // Enable all constructors of the underlying tuple type.
  //
  using tuple_type::tuple_type;

  ///
  ///
  template <size_t... indices>
  constexpr named_tuple(generic::reducible_unnamed_tuple auto&& x,
                        static_index_list<indices...>) noexcept(  //
      noexcept(named_tuple(value<indices>(std::forward<decltype(x)>(x))...)))
      : named_tuple(value<indices>(std::forward<decltype(x)>(x))...) {}
  ///
  explicit constexpr named_tuple(
      generic::reducible_unnamed_tuple auto&& x) noexcept(  //
      noexcept(named_tuple(std::forward<decltype(x)>(x),
                           meta::static_index_list::iota<size()>{})))
      : named_tuple(std::forward<decltype(x)>(x),
                    meta::static_index_list::iota<size()>{}) {}

  //
  template <static_zstring... names>
  constexpr named_tuple(generic::reducible_named_tuple auto&& x,
                        static_identifier_list<names...>) noexcept(         //
      noexcept(tuple_type(value<names>(std::forward<decltype(x)>(x))...)))  //
      : tuple_type(value<names>(std::forward<decltype(x)>(x))...) {}

  explicit constexpr named_tuple(
      generic::reducible_named_tuple auto&& x) noexcept(  //
      noexcept(named_tuple(std::forward<decltype(x)>(x), names{})))
      : named_tuple(std::forward<decltype(x)>(x), names{}) {}

  // Generic Copy/Move Construction
  //
  // explicit constexpr regular_tuple(
  //     instance::reducible_regular_tuple auto&& x) noexcept(  //
  //     noexcept(tuple_type(std::forward<decltype(x)>(x).tuple())))
  //     : tuple_type(std::forward<decltype(x)>(x).tuple()) {}

  // Generic Assignment Operator
  //
  constexpr named_tuple& operator=(auto&& x) noexcept(  //
      noexcept(
          static_cast<tuple_type&>(*this) = std::forward<decltype(x)>(x))) {
    static_cast<tuple_type&>(*this) = std::forward<decltype(x)>(x);
    return *this;
  }

  /// Default Lexicographic Ordering
  ///
  friend auto operator<=>(const named_tuple&, const named_tuple&) = default;

  // Make forwarding of the underlying tuple type easy.
  //
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

///
// template <instance::named_tuple list, size_t index>
// consteval auto offset() noexcept {
//   return list::types::template tuple_offset<index>;
// }

///
// template <instance::named_tuple list, static_zstring name>
// consteval auto offset() noexcept {
//   return offset<list, list::names::template index<name>>();
// }

template <template <typename...> typename tuple_template,
          static_zstring... names,
          typename... types>
constexpr auto auto_named_tuple(types&&... t) {
  using tuple_type = decltype(tuple_template(std::forward<types>(t)...));
  return named_tuple<static_identifier_list<names...>, tuple_type>{
      std::forward<types>(t)...};
  // return named_tuple<static_identifier_list<names...>,
  //                    std::unwrap_ref_decay_t<types>...>(
  //     std::forward<types>(t)...);
}

// template <static_zstring... names, typename... types>
// constexpr auto named_tuple_forward(types&&... t) noexcept {
//   return named_tuple<static_identifier_list<names...>, types&&...>(
//       std::forward<types>(t)...);
// }

template <size_t index>
constexpr decltype(auto)
value(instance::reducible_named_tuple auto&& t) noexcept(
    noexcept(get<index>(std::forward<decltype(t)>(t).tuple()))) {
  return get<index>(std::forward<decltype(t)>(t).tuple());
}

template <static_zstring name>
constexpr decltype(auto) value(
    instance::reducible_named_tuple auto&& t) noexcept {
  using type = meta::reduction<decltype(t)>;
  using names = typename type::names;
  using namespace meta::static_identifier_list;
  return get<index<names, name>>(std::forward<decltype(t)>(t).tuple());
}

// namespace detail {
// template <static_zstring name, std::size_t... indices>
// constexpr auto push_back(instance::reducible_named_tuple auto&& t,
//                          auto&& x,
//                          std::index_sequence<indices...>) {
//   using tuple_type = std::decay_t<decltype(t)>;
//   using namespace meta::value_list;
//   return auto_named_tuple<element<typename tuple_type::names, indices>...,
//                           name>(value<indices>(std::forward<decltype(t)>(t))...,
//                                 std::forward<decltype(x)>(x));
// }
// }  // namespace detail

// template <static_zstring name>
// constexpr auto push_back(instance::reducible_named_tuple auto&& t,
//                          auto&& value) {
//   return detail::push_back<name>(
//       std::forward<decltype(t)>(t), std::forward<decltype(value)>(value),
//       std::make_index_sequence<std::decay_t<decltype(t)>::names::size>{});
// }

// namespace detail {
// template <static_zstring name, std::size_t... indices>
// constexpr auto push_back_forward(forwardable::named_tuple auto&& t,
//                                  auto&& x,
//                                  std::index_sequence<indices...>) {
//   using tuple_type = std::decay_t<decltype(t)>;
//   using namespace meta::value_list;
//   return named_tuple_forward<element<typename tuple_type::names, indices>...,
//                              name>(
//       value<indices>(std::forward<decltype(t)>(t))...,
//       std::forward<decltype(x)>(x));
// }
// }  // namespace detail

// template <static_zstring name>
// constexpr auto push_back_forward(forwardable::named_tuple auto&& t, auto&& x)
// {
//   return detail::push_back_forward<name>(
//       std::forward<decltype(t)>(t), std::forward<decltype(x)>(x),
//       std::make_index_sequence<std::decay_t<decltype(t)>::names::size>{});
// }

/// This function is needed to make structured bindings available.
/// Here, it is a simple wrapper function template for 'value'.
///
template <size_t index>
constexpr decltype(auto) get(
    instance::reducible_named_tuple auto&& t) noexcept {
  return value<index>(std::forward<decltype(t)>(t));
}

}  // namespace lyrahgames::xstd

namespace std {

/// Provides support for using structured bindings with tuple.
///
template <lyrahgames::xstd::instance::static_identifier_list names,
          lyrahgames::xstd::generic::tuple tuple_type>
struct tuple_size<lyrahgames::xstd::named_tuple<names, tuple_type>> {
  static constexpr size_t value = std::tuple_size<tuple_type>::value;
};

/// Provides support for using structured bindings with tuple.
///
template <size_t index,
          lyrahgames::xstd::instance::static_identifier_list names,
          lyrahgames::xstd::generic::tuple tuple_type>
struct tuple_element<index, lyrahgames::xstd::named_tuple<names, tuple_type>> {
  using type = typename std::tuple_element<index, tuple_type>::type;
};

}  // namespace std
