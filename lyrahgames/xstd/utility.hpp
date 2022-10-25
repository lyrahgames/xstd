#pragma once
// This header file is the base header
// that all other headers of this library will most likely use.
// It provides basic STL header inclusions, concepts, typedefs,
// and functions that may be needed almost everywhere.

// We always want to be able to use 'assert'.
//
#include <cassert>

// Add the standard typedefs for bytes, integer, and pointer types.
//
#include <cstddef>
#include <cstdint>

// Nearly every project will make use of templates.
// So, the standard headers for the
// meta programming facilities are always needed.
//
#include <compare>
#include <concepts>
#include <functional>
#include <type_traits>
#include <typeinfo>

// We will always need to handle runtime errors.
// For that, also standard string functions are required.
//
#include <stdexcept>
#include <string>

namespace lyrahgames::xstd {

// The names 'float' and 'double' are rather inconsistent.
// For some consistency, providing their bit size seems to be promising.
//
using float32 = float;
using float64 = double;

// '_t' seems to introduce some redundancy.
// So, for the basic integer types, we get rid of it.
//
using int8 = int8_t;
using int16 = int16_t;
using int32 = int32_t;
using int64 = int64_t;
//
using uint8 = uint8_t;
using uint16 = uint16_t;
using uint32 = uint32_t;
using uint64 = uint64_t;

// According to the C++ guidelines, the usage of C-style strings
// should be made clear by at least using different naming scheme.
//
using zstring = char*;
using czstring = const char*;

// Meta functions (functions, that work on types)
// will be put in the namespace 'meta'.
// This is due to the fact that meta functions in C++
// do not provide any overloading facilities.
//
namespace meta {

/// These two functions will only be used in the development process.
/// The make sure the compiler fails to compile the source code.
/// The important side effect thereby is to let the compiler print the given
/// type. It is not totally accurate but easy to provide those functions. They
/// can give a small and fast insight on compile-time bugs.
///
template <typename T>
constexpr auto print_type() = delete;
///
template <typename T>
constexpr auto print_type(T) = delete;

// For the sake of the 'xstd', it seems to be rather impractical
// to use the standard name 'decay' for the respective type function.
// We rename it to 'reduction'.
//
template <typename type>
using reduction = std::decay_t<type>;

// In the 'xstd', we strive for a more mathematical naming style.
// The 'std' naming scheme for some concepts
// is modeled after the use of natural language.
//
template <typename T, typename U>
constexpr bool equal = std::same_as<T, U>;

}  // namespace meta

// The introduction of C++ concepts introduces some problems
// when trying to consistently name concepts and structures.
// In this library, the solution will strive to put all concepts
// inside the namespace 'generic' to be able
// to have concepts and types with the same name.
//
namespace generic {

// In the 'generic' namespace, we always work with meta functions.
// It makes sense, to enable the 'meta' namespace here by default.
//
using namespace meta;

/// The concept checks if the two given types are the same.
///
template <typename x, typename y>
concept identical = equal<x, y>;

/// The concept checks, whether 'T' is reducible to 'U'.
/// That is, by using a reduction on 'T',
/// it will be the same as 'U'.
///
template <typename T, typename U>
concept reducible = identical<reduction<T>, U>;

/// An irreducible type does not change when applying a reduction.
///
template <typename T>
concept irreducible = reducible<T, T>;

}  // namespace generic

// The namespace 'instance' is used for concepts that check,
// whether a given type is a specialization of a specific type template.
// Because types and type templates will sometimes provide the same name
// as similar concepts, this namespace is mandatory to differentiate them.
//
namespace instance {

// As in the namespace 'generic', we always work with type functions.
// Hence, we make them available by default by using the namespace 'meta'.
//
using namespace meta;

}  // namespace instance

/// With respect to the given offset,
/// compute the distance to the next offset
/// that fulfills the given alignment.
///
constexpr auto aligned_offset_padding(size_t offset, size_t alignment) noexcept
    -> size_t {
  return alignment - 1 - ((offset + alignment - 1) % alignment);
}

/// With respect to the given offset,
/// compute the next offset
/// that fulfills the given alignment.
///
constexpr auto aligned_offset(size_t offset, size_t alignment) noexcept
    -> size_t {
  return offset + aligned_offset_padding(offset, alignment);
}

///
template <size_t index>
constexpr decltype(auto)  //
forward_element(auto&& first, auto&&... tail) noexcept {
  if constexpr (index == 0)
    return std::forward<decltype(first)>(first);
  else
    return forward_element<index - 1>(std::forward<decltype(tail)>(tail)...);
}

}  // namespace lyrahgames::xstd
