#pragma once
#include <lyrahgames/xstd/forward.hpp>

namespace lyrahgames::xstd {

namespace detail {

template <size_t N, typename T>
struct tuple_element;

template <typename T>
struct tuple_element_instantiated : std::false_type {};

template <size_t N, typename T>
struct tuple_element_instantiated<tuple_element<N, T>> : std::true_type {};

template <typename T>
concept tuple_element_instantiation =
    tuple_element_instantiated<std::decay_t<T>>::value;

/// Used to implement the tuple structure.
/// Types wrappes any given type so that tuple can use it as a base class.
/// 'N' is used to allow elements with the same type
/// to occur multiple times in tuple.
template <size_t N, typename T>
struct tuple_element {
  /// Wrapped Type
  using type = T;

  /// Non-Constant Access to the Wrapped Value
  constexpr decltype(auto) value() & { return static_cast<type&>(data); }

  constexpr decltype(auto) value() && { return std::move(data); }

  /// Constant Access to the Wrapped Value
  constexpr decltype(auto) value() const& {
    return static_cast<const type&>(data);
  }

  /// Default Constructor
  constexpr tuple_element()                      //
      requires std::default_initializable<type>  //
  = default;

  template <detail::tuple_element_instantiation V>
  constexpr explicit tuple_element(V&& t) : data(std::forward<V>(t).value()) {}

  template <detail::tuple_element_instantiation V>
  constexpr tuple_element& operator=(V&& t) {
    data = std::forward<V>(t).value();
    return *this;
  }

  /// Templatized Forward Constructor
  template <generic::forward_constructible<type> V>
  requires(!detail::tuple_element_instantiation<V>)  //
      constexpr explicit tuple_element(V&& v)
      : data(std::forward<V>(v)) {}

  /// Templatized Value Assignment
  template <generic::forward_assignable<type> V>
  requires(!detail::tuple_element_instantiation<V>)  //
      constexpr tuple_element&
      operator=(V&& v) {
    data = std::forward<V>(v);
    return *this;
  }

  // /// Templatized Move Constructor
  // template <size_t M, generic::move_constructible<type> U>
  // constexpr tuple_element(tuple_element<M, U>&& t)
  //     : data(std::move(t.value())) {}

  // /// Templatized Move Assignment
  // template <size_t M, generic::move_assignable<type> U>
  // constexpr tuple_element& operator=(tuple_element<M, U>&& t) {
  //   data = std::move(t.value());
  //   return *this;
  // }

  // /// Templatized Copy Constructor
  // template <size_t M, generic::copy_constructible<type> U>
  // constexpr tuple_element(const tuple_element<M, U>& t) : data(t.value()) {}

  // /// Templatized Copy Assignment
  // template <size_t M, generic::copy_assignable<type> U>
  // constexpr tuple_element& operator=(const tuple_element<M, U>& t) {
  //   data = t.value();
  //   return *this;
  // }

  // /// Templatized Copy Constructor
  // template <size_t M, generic::copy_constructible<type> U>
  // constexpr tuple_element(tuple_element<M, U>& t) : data(t.value()) {}

  // /// Templatized Copy Assignment
  // template <size_t M, generic::copy_assignable<type> U>
  // constexpr tuple_element& operator=(tuple_element<M, U>& t) {
  //   data = t.value();
  //   return *this;
  // }

  type data{};
};

/// Specialization for types that can be used as a direct parent.
template <size_t N, typename T>
requires std::is_class_v<T> &&(!std::is_final_v<T>)  //
    struct tuple_element<N, T> : T {
  using type = T;
  using base = T;

  /// Non-Constant Access to Inherited Type
  constexpr decltype(auto) value() & { return static_cast<type&>(*this); }

  constexpr decltype(auto) value() && { return std::move(*this); }

  /// Constant Access to Inherited Type
  constexpr decltype(auto) value() const& {
    return static_cast<const type&>(*this);
  }

  /// Default Constructor
  constexpr tuple_element()                   //
      requires std::default_initializable<T>  //
  = default;

  template <detail::tuple_element_instantiation V>
  constexpr explicit tuple_element(V&& t) : base(std::forward<V>(t).value()) {}

  template <detail::tuple_element_instantiation V>
  constexpr tuple_element& operator=(V&& t) {
    static_cast<base&>(*this) = std::forward<V>(t).value();
    return *this;
  }

  /// Templatized Forward Constructor
  template <generic::forward_constructible<type> V>
  requires(!detail::tuple_element_instantiation<V>)  //
      constexpr tuple_element(V&& v)
      : base(std::forward<V>(v)) {}

  /// Templatized Forward Assignment
  template <generic::forward_assignable<type> V>
  requires(!detail::tuple_element_instantiation<V>)  //
      constexpr tuple_element&
      operator=(V&& v) {
    static_cast<base&>(*this) = std::forward<V>(v);
    return *this;
  }

  // /// Templatized Move Constructor
  // template <size_t M, generic::move_constructible<type> U>
  // constexpr tuple_element(tuple_element<M, U>&& t)
  //     : base(std::move(t.value())) {}

  // /// Templatized Move Assignment
  // template <size_t M, generic::move_assignable<type> U>
  // constexpr tuple_element& operator=(tuple_element<M, U>&& t) {
  //   static_cast<base&>(*this) = std::move(t.value());
  //   return *this;
  // }

  // /// Templatized Copy Constructor
  // template <size_t M, generic::copy_constructible<type> U>
  // constexpr tuple_element(const tuple_element<M, U>& t) : base(t.value()) {}

  // /// Templatized Copy Assignment
  // template <size_t M, generic::copy_assignable<type> U>
  // constexpr tuple_element& operator=(const tuple_element<M, U>& t) {
  //   static_cast<base&>(*this) = t.value();
  //   return *this;
  // }

  // /// Templatized Copy Constructor
  // template <size_t M, generic::copy_constructible<type> U>
  // constexpr tuple_element(tuple_element<M, U>& t) : base(t.value()) {}

  // /// Templatized Copy Assignment
  // template <size_t M, generic::copy_assignable<type> U>
  // constexpr tuple_element& operator=(tuple_element<M, U>& t) {
  //   static_cast<base&>(*this) = t.value();
  //   return *this;
  // }
};

}  // namespace detail

/// Custom Tuple Structure
template <typename... T>
struct tuple;

namespace detail {

/// Used to get the types of elements inside a tuple.
template <typename tuple, size_t index>
struct element {
  using type = typename element<typename tuple::next, index - 1>::type;
};

/// Used to get the types of elements inside a tuple.
/// Specialization for the head element.
template <typename tuple>
struct element<tuple, 0> {
  using type = typename tuple::head::type;
};

}  // namespace detail

/// Empty Tuple Specialization used as a Sentinel
template <>
struct tuple<> {
  static constexpr size_t size() { return 0; }
};

/// Tuple Implementation
/// Uses inheritance to make use of empty-base-class optimization.
template <typename T, typename... U>
struct tuple<T, U...> : detail::tuple_element<sizeof...(U), T>, tuple<U...> {
  using base = tuple<U...>;
  using next = base;
  using head = detail::tuple_element<sizeof...(U), T>;

  /// Returns the count of elements inside the tuple.
  static constexpr size_t size() { return 1 + sizeof...(U); }

  constexpr decltype(auto) head_cast() & { return static_cast<head&>(*this); }
  constexpr decltype(auto) head_cast() && { return static_cast<head&&>(*this); }
  constexpr decltype(auto) head_cast() const& {
    return static_cast<const head&>(*this);
  }

  constexpr decltype(auto) next_cast() & { return static_cast<next&>(*this); }
  constexpr decltype(auto) next_cast() && { return static_cast<next&&>(*this); }
  constexpr decltype(auto) next_cast() const& {
    return static_cast<const next&>(*this);
  }

  /// Meta member function to get the type of an element.
  template <size_t N>
  requires(N <= sizeof...(U))  //
      using type = typename detail::element<tuple<T, U...>, N>::type;

  /// Default Constructor
  constexpr tuple()                              //
      requires std::default_initializable<head>  //
      && std::default_initializable<next>        //
  = default;

  /// Templatized Forward Constructor
  template <generic::forward_constructible<head> V, typename... W>
  constexpr tuple(V&& v, W&&... w)  //
      requires((sizeof...(W) == sizeof...(U)) &&
               std::constructible_from<next, W...>)
      : head(std::forward<V>(v)), next(std::forward<W>(w)...) {}

  template <generic::instantiation<tuple> V>
  requires(size() == std::decay_t<V>::size())  //
      constexpr tuple(V&& x)
      : head(std::forward<V>(x).head_cast()),
        next(std::forward<V>(x).next_cast()) {}

  template <generic::instantiation<tuple> V>
  requires(size() == std::decay_t<V>::size())  //
      constexpr tuple&
      operator=(V&& x) {
    head_cast() = std::forward<V>(x).head_cast();
    next_cast() = std::forward<V>(x).next_cast();
    return *this;
  }

  // /// Templatized Move Constructor
  // template <typename V, typename... W>
  // constexpr tuple(tuple<V, W...>&& x)  //
  //     requires(sizeof...(W) == sizeof...(U))
  //     : head(static_cast<typename tuple<V, W...>::head&&>(x)),
  //       next(static_cast<typename tuple<V, W...>::next&&>(x)) {}

  // /// Templatized Move Assignment
  // template <typename V, typename... W>
  // constexpr tuple& operator=(tuple<V, W...>&& x)  //
  //     requires(sizeof...(W) == sizeof...(U)) {
  //   static_cast<head&>(*this) = static_cast<typename tuple<V,
  //   W...>::head&&>(x); static_cast<next&>(*this) = static_cast<typename
  //   tuple<V, W...>::next&&>(x); return *this;
  // }

  // /// Templatized Copy Constructor
  // template <typename V, typename... W>
  // constexpr tuple(const tuple<V, W...>& x)  //
  //     requires(sizeof...(W) == sizeof...(U))
  //     : head(static_cast<const typename tuple<V, W...>::head&>(x)),
  //       next(static_cast<const typename tuple<V, W...>::next&>(x)) {}

  // /// Templatized Copy Assignment
  // template <typename V, typename... W>
  // constexpr tuple& operator=(const tuple<V, W...>& x)  //
  //     requires(sizeof...(W) == sizeof...(U)) {
  //   static_cast<head&>(*this) =
  //       static_cast<const typename tuple<V, W...>::head&>(x);
  //   static_cast<next&>(*this) =
  //       static_cast<const typename tuple<V, W...>::next&>(x);
  //   return *this;
  // }

  /// Constant Variant of Element Access
  template <size_t index>
  constexpr decltype(auto) element() const  //
      requires(index == 0) {
    return static_cast<const head&>(*this).value();
  }

  /// Constant Variant of Element Access
  template <size_t index>
  constexpr decltype(auto) element() const  //
      requires((index != 0) && (index <= sizeof...(U))) {
    return static_cast<const next&>(*this).template element<index - 1>();
  }

  /// Non-Constant Variant of Element Access
  template <size_t index>
  constexpr decltype(auto) element()  //
      requires(index == 0) {
    return static_cast<head&>(*this).value();
  }

  /// Non-Constant Variant of Element Access
  template <size_t index>
  constexpr decltype(auto) element()  //
      requires((index != 0) && (index <= sizeof...(U))) {
    return static_cast<next&>(*this).template element<index - 1>();
  }
};

/// Get access to the value of an element of a tuple.
template <size_t index, typename tuple>
constexpr decltype(auto) element(tuple&& t) {
  return std::forward<tuple>(t).template element<index>();
}

namespace meta {

/// Meta function for accessing the element type of a tuple.
// template <typename tuple, size_t index>
// using element = typename detail::element<tuple, index>::type;

}  // namespace meta

/// Provides support for using structured bindings with tuple.
template <size_t index, typename tuple>
constexpr decltype(auto) get(tuple&& t) {
  return std::forward<tuple>(t).template element<index>();
}

}  // namespace lyrahgames::xstd

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
