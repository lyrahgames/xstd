#pragma once
#include <lyrahgames/xstd/type_list/type_list.hpp>
#include <lyrahgames/xstd/value_list/value_list.hpp>

namespace lyrahgames::xstd {

template <typename... types>
struct struct_tuple;

template <typename... types>
struct reverse_struct_tuple;

/// For tight constraints in template requirements,
/// we want to be able to decide whether a type is
/// an instance of the 'tuple' template.
//
namespace detail {
// The implementation of this idea is taken out inside the 'details'
// namespace by a typical template struct predicate specialization.
//
template <typename t>
struct is_struct_tuple : std::false_type {};
template <typename... types>
struct is_struct_tuple<struct_tuple<types...>> : std::true_type {};
template <typename t>
struct is_reverse_struct_tuple : std::false_type {};
template <typename... types>
struct is_reverse_struct_tuple<reverse_struct_tuple<types...>>
    : std::true_type {};
}  // namespace detail

/// To simplify the interface, we use templated variables as alias.
///
template <typename T>
constexpr bool is_struct_tuple = detail::is_struct_tuple<T>::value;

/// To simplify the interface, we use templated variables as alias.
///
template <typename T>
constexpr bool is_reverse_struct_tuple =
    detail::is_reverse_struct_tuple<T>::value;

/// To simplify the usage as concept,
/// we also provide a concept alias inside the 'instance' namespace.
///
namespace instance {
template <typename T>
concept struct_tuple = is_struct_tuple<T>;
template <typename T>
concept reverse_struct_tuple = is_reverse_struct_tuple<T>;
}  // namespace instance

namespace forwardable {
template <typename T>
concept struct_tuple = instance::struct_tuple<std::decay_t<T>>;
template <typename T>
concept reverse_struct_tuple = instance::reverse_struct_tuple<std::decay_t<T>>;
}  // namespace forwardable

template <>
struct reverse_struct_tuple<> {
  using types = type_list<>;
  static constexpr size_t size() { return 0; }
};

template <typename first, typename... tail>
struct reverse_struct_tuple<first, tail...> : reverse_struct_tuple<tail...> {
  using types = type_list<first, tail...>;

  template <size_t index>
  using type = typename types::template element<index>;

  static constexpr auto size() noexcept -> size_t { return types::size; }

  using data_type = first;
  using next_type = reverse_struct_tuple<tail...>;

  reverse_struct_tuple() = default;
  ~reverse_struct_tuple() = default;
  reverse_struct_tuple(const reverse_struct_tuple&) = default;
  reverse_struct_tuple& operator=(const reverse_struct_tuple&) = default;
  reverse_struct_tuple(reverse_struct_tuple&&) = default;
  reverse_struct_tuple& operator=(reverse_struct_tuple&&) = default;

  template <typename data_init, typename... next_init>
  explicit constexpr reverse_struct_tuple(
      data_init&& d,
      next_init&&... n) noexcept(noexcept(data_type(std::
                                                        forward<data_init>(
                                                            d))) &&  //
                                 noexcept(
                                     next_type(std::forward<next_init>(n)...)))
      : _data(std::forward<data_init>(d)),
        next_type(std::forward<next_init>(n)...) {}

  // Generic Copy/Move Construction
  //
  constexpr reverse_struct_tuple(
      forwardable::reverse_struct_tuple auto&&
          x) noexcept(noexcept(data_type(std::forward<decltype(x)>(x)
                                             .data())) &&  //
                      noexcept(next_type(std::forward<decltype(x)>(x).next())))
      : _data(std::forward<decltype(x)>(x).data()),
        next_type(std::forward<decltype(x)>(x).next()) {}

  // Generic Assignment Operator
  //
  constexpr reverse_struct_tuple&
  operator=(forwardable::reverse_struct_tuple auto&& x) noexcept(
      noexcept(_data = std::forward<decltype(x)>(x).data()) &&  //
      noexcept(static_cast<next_type&>(*this) =
                   std::forward<decltype(x)>(x).next())) {
    _data = std::forward<decltype(x)>(x).data();
    static_cast<next_type&>(*this) = std::forward<decltype(x)>(x).next();
    return *this;
  }

  // Accessing the member will discard qualifiers.
  // So, we need these accessor functions.
  constexpr decltype(auto) data() & noexcept {
    return static_cast<data_type&>(_data);
  }
  constexpr decltype(auto) data() && noexcept {
    return static_cast<data_type&&>(_data);
  }
  constexpr decltype(auto) data() const& noexcept {
    return static_cast<const data_type&>(_data);
  }
  constexpr decltype(auto) data() const&& noexcept {
    return static_cast<const data_type&&>(_data);
  }

  constexpr decltype(auto) next() & noexcept {
    return static_cast<next_type&>(*this);
  }
  constexpr decltype(auto) next() && noexcept {
    return static_cast<next_type&&>(*this);
  }
  constexpr decltype(auto) next() const& noexcept {
    return static_cast<const next_type&>(*this);
  }
  constexpr decltype(auto) next() const&& noexcept {
    return static_cast<const next_type&&>(*this);
  }

  data_type _data;
};

template <typename T>
struct cast {};
template <typename... types>
struct cast<type_list<types...>> {
  using type = reverse_struct_tuple<types...>;
};

template <typename... T>
struct struct_tuple : cast<typename type_list<T...>::reverse>::type {
  using types = type_list<T...>;

  template <size_t index>
  using type = typename types::template element<index>;

  using reverse_type = typename cast<typename type_list<T...>::reverse>::type;
  // using reverse_type::reverse_type;

  static constexpr auto size() noexcept -> size_t { return sizeof...(T); }
  using permutation = typename meta::value_list::iota<size()>::reverse;

  struct_tuple() = default;

  template <size_t... indices>
  struct_tuple(value_list<indices...>, auto&&... args)  //
      requires(size() == sizeof...(args))
      : reverse_type(forward_element<indices>(
            std::forward<decltype(args)>(args)...)...) {}

  struct_tuple(auto&&... args)  //
      requires(size() == sizeof...(args))
      : struct_tuple(permutation{}, std::forward<decltype(args)>(args)...) {}

  constexpr decltype(auto) reverse() & noexcept {
    return static_cast<reverse_type&>(*this);
  }
  constexpr decltype(auto) reverse() && noexcept {
    return static_cast<reverse_type&&>(*this);
  }
  constexpr decltype(auto) reverse() const& noexcept {
    return static_cast<const reverse_type&>(*this);
  }
  constexpr decltype(auto) reverse() const&& noexcept {
    return static_cast<const reverse_type&&>(*this);
  }
};

// Deduction Guides
template <typename... types>
struct_tuple(types...) -> struct_tuple<types...>;
template <typename... types>
reverse_struct_tuple(types...) -> reverse_struct_tuple<types...>;

template <size_t index>
constexpr decltype(auto) value(
    forwardable::reverse_struct_tuple auto&& t) noexcept {
  if constexpr (index == 0)
    return std::forward<decltype(t)>(t).data();
  else
    return value<index - 1>(std::forward<decltype(t)>(t).next());
}

template <size_t index>
constexpr decltype(auto) value(forwardable::struct_tuple auto&& t) noexcept {
  return value<std::decay_t<decltype(t)>::size() - index - 1>(
      std::forward<decltype(t)>(t).reverse());
}

template <size_t index>
constexpr decltype(auto) get(
    forwardable::reverse_struct_tuple auto&& t) noexcept {
  return value<index>(std::forward<decltype(t)>(t));
}

template <size_t index>
constexpr decltype(auto) get(forwardable::struct_tuple auto&& t) noexcept {
  return value<index>(std::forward<decltype(t)>(t));
}

}  // namespace lyrahgames::xstd

namespace std {

/// Provides support for using structured bindings with reverse_struct_tuple.
template <typename... T>
struct tuple_size<lyrahgames::xstd::reverse_struct_tuple<T...>> {
  static constexpr size_t value =
      lyrahgames::xstd::reverse_struct_tuple<T...>::size();
};
template <typename... T>
struct tuple_size<lyrahgames::xstd::struct_tuple<T...>> {
  static constexpr size_t value = lyrahgames::xstd::struct_tuple<T...>::size();
};

/// Provides support for using structured bindings with reverse_struct_tuple.
template <size_t N, typename... T>  //
requires(N < sizeof...(T))          //
    struct tuple_element<N, lyrahgames::xstd::reverse_struct_tuple<T...>> {
  using type =
      typename lyrahgames::xstd::reverse_struct_tuple<T...>::template type<N>;
};
template <size_t N, typename... T>  //
requires(N < sizeof...(T))          //
    struct tuple_element<N, lyrahgames::xstd::struct_tuple<T...>> {
  using type = typename lyrahgames::xstd::struct_tuple<T...>::template type<N>;
};

}  // namespace std
