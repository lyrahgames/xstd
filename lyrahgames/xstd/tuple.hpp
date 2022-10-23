#pragma once
#include <lyrahgames/xstd/forward.hpp>
#include <lyrahgames/xstd/type_list.hpp>
#include <lyrahgames/xstd/value_list.hpp>
#include <tuple>

namespace lyrahgames::xstd {

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

}  // namespace generic

// The C++ standard does not specify a memory layout to be used for tuples.
// For issues concerning the copy of memory, a custom tuple may be needed.
// It should provide a contiguous access to the given types in the same
// order.
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

/// Access the elements of a tuple by their index.
///
template <size_t index>
constexpr decltype(auto) value(forwardable::tuple auto&& t) noexcept {
  if constexpr (index == 0)
    return std::forward<decltype(t)>(t).data().data();
  else
    return value<index - 1>(std::forward<decltype(t)>(t).next());
}

namespace detail::tuple {

// "Empty base optimization is prohibited if one of the empty base classes is
// also the type or the base of the type of the first non-static data member,
// since the two base subobjects of the same type are required to have different
// addresses within the object representation of the most derived type."
// [cppreference.com: Empty Base Optimization]
// https://en.cppreference.com/w/cpp/language/ebo

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

  using data_type = detail::tuple::wrapper<sizeof...(U), T>;
  using next_type = tuple<U...>;

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

  tuple() = default;
  ~tuple() = default;
  tuple(const tuple&) = default;
  tuple& operator=(const tuple&) = default;
  tuple(tuple&&) = default;
  tuple& operator=(tuple&&) = default;

  // Templatized Forward Constructor
  //
  template <typename data_init, typename... next_init>
  explicit constexpr tuple(data_init&& d, next_init&&... n) noexcept(
      noexcept(data_type(std::forward<data_init>(d))) &&  //
      noexcept(next_type(std::forward<next_init>(n)...)))
      : data_type(std::forward<data_init>(d)),
        next_type(std::forward<next_init>(n)...) {}

  // Generic Copy/Move Construction
  //
  constexpr tuple(forwardable::tuple auto&& x) noexcept(
      noexcept(data_type(std::forward<decltype(x)>(x).data())) &&  //
      noexcept(next_type(std::forward<decltype(x)>(x).next())))
      : data_type(std::forward<decltype(x)>(x).data()),
        next_type(std::forward<decltype(x)>(x).next()) {}

  // Generic Assignment Operator
  //
  constexpr tuple& operator=(forwardable::tuple auto&& x) noexcept(
      noexcept(data() = std::forward<decltype(x)>(x).data()) &&  //
      noexcept(next() = std::forward<decltype(x)>(x).next())) {
    data() = std::forward<decltype(x)>(x).data();
    next() = std::forward<decltype(x)>(x).next();
    return *this;
  }
};

/// This function is needed to make structured bindings available.
/// Here, it is a simple wrapper function template for 'value'.
template <size_t index>
constexpr decltype(auto) get(forwardable::tuple auto&& t) noexcept {
  return value<index>(std::forward<decltype(t)>(t));
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
