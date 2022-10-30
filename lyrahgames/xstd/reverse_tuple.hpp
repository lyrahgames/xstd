#pragma once
#include <lyrahgames/xstd/tuple.hpp>
#include <lyrahgames/xstd/type_list/type_list.hpp>

namespace lyrahgames::xstd {

/// The reverse tuple stores the given types in reverse order
/// as if they would have been given as member variables in a struct.
/// Typically, it is used as a base type for more advanced tuple types
/// and does neither add any special memory optimizations for empty classes
/// nor special allocation handling via allocators
/// to keep the type as trivial as possible.
///
template <typename... types>
struct reverse_tuple;

/// For tight constraints in template requirements,
/// we want to be able to decide whether a type is
/// an instance of the 'reverse_tuple' template.
//
namespace detail {
// The implementation of this idea is taken out inside the 'details'
// namespace by a typical template struct predicate specialization.
//
template <typename t>
struct is_reverse_tuple : std::false_type {};
template <typename... types>
struct is_reverse_tuple<reverse_tuple<types...>> : std::true_type {};
}  // namespace detail

/// To simplify the interface, we use templated variables as alias.
///
template <typename T>
constexpr bool is_reverse_tuple = detail::is_reverse_tuple<T>::value;

/// To simplify the usage as concept,
/// we also provide a concept alias inside the 'instance' namespace.
///
namespace instance {

template <typename T>
concept reverse_tuple = is_reverse_tuple<T>;

template <typename T>
concept reducible_reverse_tuple = reverse_tuple<reduction<T>>;

}  // namespace instance

namespace detail::reverse_tuple {

using xstd::reverse_tuple;
using xstd::type_list;

//
template <instance::type_list types>
struct from_type_list {};
//
template <typename... types>
struct from_type_list<type_list<types...>> {
  using type = reverse_tuple<types...>;
};

}  // namespace detail::reverse_tuple

namespace meta::reverse_tuple {

/// Returns an instance of 'reverse_tuple' which
/// takes all the types given in the type list.
///
template <instance::type_list types>
using from_type_list =
    typename detail::reverse_tuple::from_type_list<types>::type;

/// Returns the offset in bytes of the element
/// inside the structure given by the index.
///
template <instance::reverse_tuple tuple_type, size_t index>
constexpr auto offset = tuple_type::template offset<index>();

}  // namespace meta::reverse_tuple

/// Get forward access to the element given by index inside a reverse tuple.
///
template <size_t index>
constexpr decltype(auto) value(
    instance::reducible_reverse_tuple auto&& t) noexcept {
  if constexpr (index == 0)
    return std::forward<decltype(t)>(t).data();
  else
    return value<index - 1>(std::forward<decltype(t)>(t).next());
}

// Specialization of an Empty Reverse Tuple
// This part is mainly used as a sentinel for other tuple functions.
//
template <>
struct reverse_tuple<> {
  using types = type_list<>;
  static constexpr auto size() -> size_t { return 0; }
  static constexpr auto unaligned_byte_size() -> size_t { return 0; }
  friend auto operator<=>(const reverse_tuple&, const reverse_tuple&) = default;
};

// Specialization of Reverse Tuples with at Least One Element
// No multiple inheritance is used.
// Otherwise, virtual inheritance as well as
// alignment and padding of wrapped types may
// unknowingly increase the size and padding of the tuple type.
// Hence, no EBCO for empty classes is available in this type.
// We inherit from the tailing tuple type.
// For the empty tuple type, EBCO will make sure
// no additional storage is used.
//
template <typename first, typename... tail>
struct reverse_tuple<first, tail...> : reverse_tuple<tail...> {
  using data_type = first;
  using next_type = reverse_tuple<tail...>;

  /// Type list storing the types of elements.
  ///
  using types = type_list<first, tail...>;

  /// Returns the type for the element given by index.
  ///
  template <size_t index>
  using type = typename types::template element<index>;

  // This type is used to get the correct byte size and alignment
  // for reference types when evaluating the offset of identifiers.
  // alignof(T&) == alignof(T)
  // sizeof(T&) == sizeof(T)
  //
  struct wrapped_data_type {
    data_type data;
  };

  /// Returns the count of stored elements.
  ///
  static consteval auto size() noexcept -> size_t { return types::size; }

  /// Returns the byte offset of an element given by index
  /// inside the tuple structure.
  ///
  template <size_t index>
  static consteval auto offset() noexcept -> size_t {
    if constexpr (index == 0)
      return aligned_offset(next_type::unaligned_byte_size(),
                            alignof(wrapped_data_type));
    else
      return next_type::template offset<index - 1>();
  }

  /// Returns the byte size of the whole structure without
  /// the tailing padding bytes to fulfill its alignment.
  /// When using this structure as a base class,
  /// this may the next possible offset to put member variables at.
  ///
  static consteval auto unaligned_byte_size() noexcept -> size_t {
    return offset<0>() + sizeof(wrapped_data_type);
  }

  //
  reverse_tuple() = default;
  ~reverse_tuple() = default;
  reverse_tuple(const reverse_tuple&) = default;
  reverse_tuple& operator=(const reverse_tuple&) = default;
  reverse_tuple(reverse_tuple&&) = default;
  reverse_tuple& operator=(reverse_tuple&&) = default;

  template <typename data_init, typename... next_init>
  explicit constexpr reverse_tuple(data_init&& d,
                                   next_init&&... n)               //
      noexcept(noexcept(data_type(std::forward<data_init>(d))) &&  //
               noexcept(next_type(std::forward<next_init>(n)...)))
      : _data(std::forward<data_init>(d)),
        next_type(std::forward<next_init>(n)...) {}

  // Generic Copy/Move Construction
  //
  explicit constexpr reverse_tuple(
      instance::reducible_reverse_tuple auto&& x)                           //
      noexcept(noexcept(data_type(std::forward<decltype(x)>(x).data())) &&  //
               noexcept(next_type(std::forward<decltype(x)>(x).next())))
      : _data(std::forward<decltype(x)>(x).data()),
        next_type(std::forward<decltype(x)>(x).next()) {}

  // Generic Assignment Operator
  //
  constexpr reverse_tuple&
  operator=(instance::reducible_reverse_tuple auto&& x) noexcept(
      noexcept(_data = std::forward<decltype(x)>(x).data()) &&  //
      noexcept(static_cast<next_type&>(*this) =
                   std::forward<decltype(x)>(x).next())) {
    _data = std::forward<decltype(x)>(x).data();
    static_cast<next_type&>(*this) = std::forward<decltype(x)>(x).next();
    return *this;
  }

  // Default Ordering
  //
  friend auto operator<=>(const reverse_tuple&, const reverse_tuple&) = default;

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

// Deduction Guides
//
template <typename... types>
reverse_tuple(types&&...) -> reverse_tuple<std::unwrap_ref_decay_t<types>...>;

// Structured Bindings Function Support
//
template <size_t index>
constexpr decltype(auto) get(
    instance::reducible_reverse_tuple auto&& t) noexcept {
  return value<index>(std::forward<decltype(t)>(t));
}

}  // namespace lyrahgames::xstd

namespace std {

/// Provides support for using structured bindings with reverse_tuple.
///
template <typename... T>
struct tuple_size<lyrahgames::xstd::reverse_tuple<T...>> {
  static constexpr size_t value = lyrahgames::xstd::reverse_tuple<T...>::size();
};

/// Provides support for using structured bindings with reverse_tuple.
///
template <size_t N, typename... T>  //
struct tuple_element<N, lyrahgames::xstd::reverse_tuple<T...>> {
  using type = typename lyrahgames::xstd::reverse_tuple<T...>::template type<N>;
};

}  // namespace std
