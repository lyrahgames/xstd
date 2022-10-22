#pragma once
#include <lyrahgames/xstd/forward.hpp>
#include <lyrahgames/xstd/type_list.hpp>
#include <tuple>

namespace lyrahgames::xstd {

// The C++ standard does not specify a memory layout to be used for tuples.
// For issues concerning the copy of memory, a custom tuple may be needed.
// It should provide a contiguous access to the given types in the same order.
//
// The tuple should be trivial if every contained is trivial.

template <typename... types>
struct tuple;

/// For tight constraints in template requirements,
/// we want to be able to decide whether a type is
/// an instance of the 'tuple' template.
namespace detail {
// The implementation of this idea is taken out inside the 'details'
// namespace by a typical template struct predicate specialization.
template <typename t>
struct is_tuple : std::false_type {};
template <typename... types>
struct is_tuple<tuple<types...>> : std::true_type {};
}  // namespace detail

/// To simplify the interface, we use templated variables as alias.
template <typename T>
constexpr bool is_tuple = detail::is_tuple<T>::value;

/// To simplify the usage as concept,
/// we also provide a concept alias inside the 'instance' namespace.
namespace instance {
template <typename T>
concept tuple = is_tuple<T>;
}  // namespace instance

namespace forwardable {
template <typename T>
concept tuple = instance::tuple<std::decay_t<T>>;
}

namespace detail::tuple {

/// Used to implement the tuple structure.
/// Types wrappes any given type so that tuple can use it as a base class.
/// 'N' is used to allow elements with the same type
/// to occur multiple times in tuple.
template <size_t N, typename T>
struct wrapper;

template <typename type>
struct is_wrapper : std::false_type {};
template <size_t N, typename T>
struct is_wrapper<wrapper<N, T>> : std::true_type {};

namespace forwardable {
template <typename T>
concept wrapper = is_wrapper<std::decay_t<T>>::value;
}

template <size_t N, typename T>
struct wrapper {
  using type = T;

  ///
  constexpr wrapper() noexcept(
      noexcept(std::is_nothrow_default_constructible_v<type>)) = default;

  /// Templatized Forward Constructor
  template <typename... types>
  constexpr wrapper(types&&... values) noexcept(
      noexcept(std::is_nothrow_constructible_v<type, types...>))  //
      requires std::constructible_from<type, types...>
      : _value(std::forward<types>(values)...) {}

  constexpr wrapper(forwardable::wrapper auto&& x)
      : _value(std::forward<decltype(x)>(x).value()) {}
  constexpr wrapper& operator=(forwardable::wrapper auto&& x) {
    _value = std::forward<decltype(x)>(x).value();
    return *this;
  }

  constexpr decltype(auto) value() & noexcept {
    return static_cast<type&>(_value);
  }
  constexpr decltype(auto) value() && noexcept {
    return static_cast<type&&>(_value);
  }
  constexpr decltype(auto) value() const& noexcept {
    return static_cast<const type&>(_value);
  }
  constexpr decltype(auto) value() const&& noexcept {
    return static_cast<const type&&>(_value);
  }

  // This seems to be better, but forces the type to be not trivial.
  // So for now, explicit default initialization
  // of the wrapped value will not be used.
  //
  // type _value{};
  //
  type _value;
};

// Specialization for inheritable classes.
template <size_t N, typename T>
requires(std::is_class_v<T> && (!std::is_final_v<T>))  //
    struct wrapper<N, T> : T {
  using type = T;
  using type::type;

  ///
  constexpr wrapper() noexcept(
      noexcept(std::is_nothrow_default_constructible_v<type>)) = default;

  constexpr wrapper(forwardable::wrapper auto&& x)
      : type(std::forward<decltype(x)>(x).value()) {}
  constexpr wrapper& operator=(forwardable::wrapper auto&& x) {
    *this = std::forward<decltype(x)>(x).value();
    return *this;
  }

  constexpr decltype(auto) value() & noexcept {
    return static_cast<type&>(*this);
  }
  constexpr decltype(auto) value() && noexcept {
    return static_cast<type&&>(*this);
  }
  constexpr decltype(auto) value() const& noexcept {
    return static_cast<const type&>(*this);
  }
  constexpr decltype(auto) value() const&& noexcept {
    return static_cast<const type&&>(*this);
  }
};

}  // namespace detail::tuple

/// Empty Tuple Specialization
template <>
struct tuple<> {
  using types = type_list<>;
  static constexpr size_t size() { return 0; }
};

/// Tuple Implementation
/// Uses inheritance to make use of empty-base-class optimization.
template <typename T, typename... U>
struct tuple<T, U...> : detail::tuple::wrapper<sizeof...(U), T>, tuple<U...> {
  using types = type_list<T, U...>;

  template <size_t index>
  using type = typename types::template element<index>;

  /// Returns the count of elements inside the tuple.
  static constexpr size_t size() { return 1 + sizeof...(U); }

  using head = detail::tuple::wrapper<sizeof...(U), T>;
  using tail = tuple<U...>;

  constexpr decltype(auto) head_cast() & noexcept {
    return static_cast<head&>(*this);
  }
  constexpr decltype(auto) head_cast() && noexcept {
    return static_cast<head&&>(*this);
  }
  constexpr decltype(auto) head_cast() const& noexcept {
    return static_cast<const head&>(*this);
  }
  constexpr decltype(auto) head_cast() const&& noexcept {
    return static_cast<const head&&>(*this);
  }

  constexpr decltype(auto) tail_cast() & noexcept {
    return static_cast<tail&>(*this);
  }
  constexpr decltype(auto) tail_cast() && noexcept {
    return static_cast<tail&&>(*this);
  }
  constexpr decltype(auto) tail_cast() const& noexcept {
    return static_cast<const tail&>(*this);
  }
  constexpr decltype(auto) tail_cast() const&& noexcept {
    return static_cast<const tail&&>(*this);
  }

  /// Default Constructor
  // constexpr tuple() noexcept(
  //     noexcept(std::is_nothrow_default_constructible_v<head>&&
  //                  std::is_nothrow_default_constructible_v<tail>)) //
  //     requires // std::default_initializable<head> &&
  //     std::default_initializable<tail>  //
  // = default;
  constexpr tuple() noexcept = default;

  /// Templatized Forward Constructor
  template <typename V, typename... W>
  constexpr tuple(V&& v, W&&... w)  //
      requires((sizeof...(W) == sizeof...(U)) &&
               std::constructible_from<tail, W...> &&
               std::constructible_from<head, V>)
      : head(std::forward<V>(v)), tail(std::forward<W>(w)...) {}

  template <forwardable::tuple V>
  requires(size() == std::decay_t<V>::size())  //
      constexpr tuple(V&& x)
      : head(std::forward<V>(x).head_cast()),
        tail(std::forward<V>(x).tail_cast()) {}

  template <forwardable::tuple V>
  requires(size() == std::decay_t<V>::size())  //
      constexpr tuple&
      operator=(V&& x) {
    head_cast() = std::forward<V>(x).head_cast();
    tail_cast() = std::forward<V>(x).tail_cast();
    return *this;
  }
};

/// Access the elements of a tuple by their index.
template <size_t index>
constexpr decltype(auto) value(forwardable::tuple auto&& t) noexcept {
  if constexpr (index == 0)
    return std::forward<decltype(t)>(t).head_cast().value();
  else
    return value<index - 1>(std::forward<decltype(t)>(t).tail_cast());
}

}  // namespace lyrahgames::xstd

/// Support for Structured Bindings
//
namespace lyrahgames::xstd {
/// This function is needed to make structured bindings available.
/// Here, it is a simple wrapper function template for 'value'.
template <size_t index>
constexpr decltype(auto) get(auto&& t) noexcept {
  return value<index>(std::forward<decltype(t)>(t));
}
}  // namespace lyrahgames::xstd
//
namespace std {
/// Provides support for using structured bindings with tuple.
template <typename... T>
struct tuple_size<lyrahgames::xstd::tuple<T...>> {
  static constexpr size_t value = lyrahgames::xstd::tuple<T...>::size();
};
/// Provides support for using structured bindings with tuple.
template <size_t N, typename... T>  //
requires(N < sizeof...(T))          //
    struct tuple_element<N, lyrahgames::xstd::tuple<T...>> {
  using type = typename lyrahgames::xstd::tuple<T...>::template type<N>;
};
}  // namespace std
