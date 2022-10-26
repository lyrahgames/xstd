#pragma once
#include <lyrahgames/xstd/tuple.hpp>

namespace lyrahgames::xstd {

/// The actual custom tuple structure.
/// Types are stored in the same order as they are given.
/// This includes alignment and padding as it would be done in structs.
/// Due to inheritance and EBCO, empty classes should not
/// take up any space when used only once.
///
/// "Empty base optimization is prohibited if one of the empty base classes is
/// also the type or the base of the type of the first non-static data member,
/// since the two base subobjects of the same type are required to have
/// different addresses within the object representation of the most derived
/// type." [cppreference.com: Empty Base Optimization]
/// https://en.cppreference.com/w/cpp/language/ebo
///
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
concept reducable_regular_tuple = regular_tuple<reduction<T>>;

}  // namespace instance

/// Access the elements of a regular_tuple by their index.
///
template <size_t index>
constexpr decltype(auto) value(
    instance::reducable_regular_tuple auto&& t) noexcept {
  if constexpr (index == 0)
    return std::forward<decltype(t)>(t).data().data();
  else
    return value<index - 1>(std::forward<decltype(t)>(t).next());
}

namespace detail::regular_tuple {

/// Used to implement the regular_tuple structure.
/// Types wrappes any given type so that regular_tuple can use it as a base
/// class. 'N' is used to allow elements with the same type to occur multiple
/// times in regular_tuple.
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
  using data_type = T;

  // The default functions of the compiler
  // will automatically have the best properties.
  // We still explicitly define them to make sure they exist.
  wrapper() = default;
  wrapper(const wrapper&) = default;
  wrapper(wrapper&&) = default;
  wrapper& operator=(const wrapper&) = default;
  wrapper& operator=(wrapper&&) = default;

  /// Forward Constructor
  template <typename type>
  explicit constexpr wrapper(type&& value) noexcept(
      noexcept(data_type(std::forward<type>(value))))
      : _data(std::forward<type>(value)) {}

  // Generic Forward Constructor for Similar Wrapper Types
  explicit constexpr wrapper(forwardable::wrapper auto&& x) noexcept(
      noexcept(data_type(std::forward<decltype(x)>(x).data())))
      : _data(std::forward<decltype(x)>(x).data()) {}

  // Generic Assignment of Similar Wrapper Types
  constexpr wrapper& operator=(forwardable::wrapper auto&& x) noexcept(
      noexcept(_data = std::forward<decltype(x)>(x).data())) {
    _data = std::forward<decltype(x)>(x).data();
    return *this;
  }

  // Accessing the member will discard the qualifiers.
  // So, we need these accessor functions.
  // Additionally, they provide a uniform interface
  // for different wrapper specializations.
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

  // The alternative
  //
  // data_type _data{};
  //
  // seems to be better, but forces
  // the data_type to be non-trivial.
  // So for now, explicit default initialization
  // of the wrapped value will not be used.
  //
  data_type _data;
};

// Specialization for inheritable classes.
// This will enable empty base class optimization.
template <size_t N, typename T>
requires(std::is_class_v<T> && (!std::is_final_v<T>))  //
    struct wrapper<N, T> : T {
  using data_type = T;
  using data_type::data_type;

  // The default functions of the compiler
  // will automatically have the best properties.
  // We still explicitly define them to make sure they exist.
  //
  wrapper() = default;
  wrapper(const wrapper&) = default;
  wrapper(wrapper&&) = default;
  wrapper& operator=(const wrapper&) = default;
  wrapper& operator=(wrapper&&) = default;

  // Generic Forward Constructor for Similar Wrapper Types
  //
  explicit constexpr wrapper(forwardable::wrapper auto&& x) noexcept(
      noexcept(data_type(std::forward<decltype(x)>(x).data())))
      : data_type(std::forward<decltype(x)>(x).data()) {}

  // Generic Assignment of Similar Wrapper Types
  //
  constexpr wrapper& operator=(forwardable::wrapper auto&& x) noexcept(noexcept(
      static_cast<data_type&>(*this) = std::forward<decltype(x)>(x).data())) {
    static_cast<data_type&>(*this) = std::forward<decltype(x)>(x).data();
    return *this;
  }

  // Accessing the member will discard the qualifiers.
  // So, we need these accessor functions.
  // Additionally, they provide a uniform interface
  // for different wrapper specializations.
  //
  constexpr decltype(auto) data() & noexcept {
    return static_cast<data_type&>(*this);
  }
  constexpr decltype(auto) data() && noexcept {
    return static_cast<data_type&&>(*this);
  }
  constexpr decltype(auto) data() const& noexcept {
    return static_cast<const data_type&>(*this);
  }
  constexpr decltype(auto) data() const&& noexcept {
    return static_cast<const data_type&&>(*this);
  }
};

}  // namespace detail::regular_tuple

/// Empty regular_Tuple Specialization
template <>
struct regular_tuple<> {
  using types = type_list<>;
  static constexpr size_t size() { return 0; }
};

/// regular_Tuple Implementation
/// Uses inheritance to make use of empty-base-class optimization.
template <typename T, typename... U>
struct regular_tuple<T, U...> : detail::regular_tuple::wrapper<sizeof...(U), T>,
                                regular_tuple<U...> {
  using types = type_list<T, U...>;

  template <size_t index>
  using type = typename types::template element<index>;

  /// Returns the count of elements inside the regular_tuple.
  static constexpr size_t size() { return 1 + sizeof...(U); }

  using data_type = detail::regular_tuple::wrapper<sizeof...(U), T>;
  using next_type = regular_tuple<U...>;

  constexpr decltype(auto) data() & noexcept {
    return static_cast<data_type&>(*this);
  }
  constexpr decltype(auto) data() && noexcept {
    return static_cast<data_type&&>(*this);
  }
  constexpr decltype(auto) data() const& noexcept {
    return static_cast<const data_type&>(*this);
  }
  constexpr decltype(auto) data() const&& noexcept {
    return static_cast<const data_type&&>(*this);
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

  regular_tuple() = default;
  ~regular_tuple() = default;
  regular_tuple(const regular_tuple&) = default;
  regular_tuple& operator=(const regular_tuple&) = default;
  regular_tuple(regular_tuple&&) = default;
  regular_tuple& operator=(regular_tuple&&) = default;

  // Templatized Forward Constructor
  //
  template <typename data_init, typename... next_init>
  explicit constexpr regular_tuple(data_init&& d, next_init&&... n) noexcept(
      noexcept(data_type(std::forward<data_init>(d))) &&  //
      noexcept(next_type(std::forward<next_init>(n)...)))
      : data_type(std::forward<data_init>(d)),
        next_type(std::forward<next_init>(n)...) {}

  // Generic Copy/Move Construction
  //
  constexpr regular_tuple(instance::reducable_regular_tuple auto&& x) noexcept(
      noexcept(data_type(std::forward<decltype(x)>(x).data())) &&  //
      noexcept(next_type(std::forward<decltype(x)>(x).next())))
      : data_type(std::forward<decltype(x)>(x).data()),
        next_type(std::forward<decltype(x)>(x).next()) {}

  // Generic Assignment Operator
  //
  constexpr regular_tuple&
  operator=(instance::reducable_regular_tuple auto&& x) noexcept(
      noexcept(data() = std::forward<decltype(x)>(x).data()) &&  //
      noexcept(next() = std::forward<decltype(x)>(x).next())) {
    data() = std::forward<decltype(x)>(x).data();
    next() = std::forward<decltype(x)>(x).next();
    return *this;
  }
};

/// Deduction Guides
///
template <typename... types>
regular_tuple(types...) -> regular_tuple<types...>;

///
///
template <size_t... indices>
constexpr auto auto_tuple(generic::tuple auto&& t,
                          value_list<indices...>) noexcept(  //
    noexcept(regular_tuple{get<indices>(std::forward<decltype(t)>(t))...})) {
  return regular_tuple{get<indices>(std::forward<decltype(t)>(t))...};
}

///
///
constexpr auto auto_tuple(generic::tuple auto&& t) noexcept(  //
    noexcept(
        auto_tuple(std::forward<decltype(t)>(t),
                   meta::value_list::iota<
                       std::tuple_size<std::decay_t<decltype(t)>>::value>{}))) {
  using namespace meta::value_list;
  constexpr auto size = std::tuple_size<std::decay_t<decltype(t)>>::value;
  return auto_tuple(std::forward<decltype(t)>(t), iota<size>{});
}

/// This function is needed to make structured bindings available.
/// Here, it is a simple wrapper function template for 'value'.
///
template <size_t index>
constexpr decltype(auto) get(
    instance::reducable_regular_tuple auto&& t) noexcept {
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
requires(N < sizeof...(T))          //
    struct tuple_element<N, lyrahgames::xstd::regular_tuple<T...>> {
  using type = typename lyrahgames::xstd::regular_tuple<T...>::template type<N>;
};

}  // namespace std
