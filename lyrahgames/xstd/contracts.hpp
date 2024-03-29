#pragma once
#include <stdexcept>
#include <string>
//
#ifdef __clang__
#include <experimental/source_location>
namespace lyrahgames::xstd {
using source_location = std::experimental::source_location;
}
#else
#include <source_location>
namespace lyrahgames::xstd {
using source_location = std::source_location;
}
#endif
//
#include <lyrahgames/xstd/meta.hpp>
#include <lyrahgames/xstd/utility.hpp>

namespace lyrahgames::xstd {

// (static) constraints: requirements on template arguments
// concepts: named sets of static constraints
// (dynamic) constraints: requirements on values of types
// contract: named sets of dynamic constraints that should be implicitly
// fulfilled (if a program works correctly it should be allowed to remove them).
// The following presents a simple idea for one-variable contracts.
// They can be used to state simple pre- and post-conditions.

namespace generic {

// template <typename T>
// concept constraint = generic::callable<T> && meta::equal < meta::result<T>,
// bool > &&(meta::argument_count<T> == 1) && std::default_initializable<T>;

template <typename C, typename T>
concept constraint = std::is_empty_v<C> &&  //
    requires(C c, const T& t) {
  { c(t) } -> std::convertible_to<bool>;
};

}  // namespace generic

struct contract_violation : std::runtime_error {
  using base = std::runtime_error;
  contract_violation(source_location l)
      : base(std::string(l.file_name()) + ':' + std::to_string(l.line()) + ':' +
             std::to_string(l.column()) + ": " + l.function_name()),
        location(l) {}
  source_location location{};
};

#ifdef NDEBUG

// template <generic::constraint T>
// using contract = meta::argument<T, 0>;

template <typename T, auto C>
using contract = T;

#else  // NDEBUG

template <typename T, auto C>
struct contract {
  using type = T;
  static constexpr auto constraint = C;

  void check(source_location location) {
    if (!constraint(value)) throw contract_violation{location};
  }

  // Default constructor is not allowed.

  // Only one-variable constructors for copying and moving.
  // Otherwise, the source location would not be capturable.
  // template <typename argument>
  // contract(argument&& arg,
  //          source_location location = source_location::current())  //
  //     requires std::constructible_from<type, argument>
  //     : value(std::forward<argument>(arg)) {
  //   check(location);
  // }

  contract(const type& arg,
           source_location location = source_location::current())
      : value(arg) {
    check(location);
  }

  contract(type&& arg, source_location location = source_location::current())
      : value(std::move(arg)) {
    check(location);
  }

  // Implicit conversion back to the underlying type.
  operator type() { return value; }

  type value;
};

// A contract must hold the one-variable contstraint
// and the underlying type.
// template <generic::constraint T>
// struct contract {
//   using type = meta::argument<T, 0>;
//   using constraint = T;

//   void check(source_location location) {
//     if (!constraint{}(value)) throw contract_violation{location};
//   }

//   // Implicit Constructor
//   contract(type v, source_location location = source_location::current())
//       : value(v) {
//     check(location);
//   }

//   // Construction from contracts with a convertible base type
//   template <generic::constraint U>
//   requires std::constructible_from<type, typename contract<U>::type>  //
//   contract(contract<U> c, source_location location =
//   source_location::current())
//       : value(c.value) {
//     check(location);
//   }

//   // Implicit conversion to base type.
//   operator type() { return value; }

//   type value;
// };

#endif  // NDEBUG

// Contracts in this sense should have the name of adjectives.
// This makes them distinguishable from other types and templates.
// Concepts that use adjectives as names can be found in another
// namespace so that there is no name clash.

// template <std::totally_ordered type>
// using positive = contract<decltype([](type v) { return v > type(0); })>;

// template <std::totally_ordered type>
// using non_negative = contract<decltype([](type v) { return v >= type(0); })>;

// template <std::totally_ordered type>
// using negative = contract<decltype([](type v) { return v < type(0); })>;

// template <std::totally_ordered type>
// using non_zero = contract<decltype([](type v) { return v != type(0); })>;

// // [0,1]
// template <std::totally_ordered type>
// using normalized =
//     contract<decltype([](type v) { return (type(0) <= v) && (v <= type(1));
//     })>;
// // (0,1)
// template <std::totally_ordered type>
// using open_normalized =
//     contract<decltype([](type v) { return (type(0) < v) && (v < type(1));
//     })>;
// // [0,1)
// template <std::totally_ordered type>
// using left_normalized =
//     contract<decltype([](type v) { return (type(0) <= v) && (v < type(1));
//     })>;
// // (0,1]
// template <std::totally_ordered type>
// using right_normalized =
//     contract<decltype([](type v) { return (type(0) < v) && (v <= type(1));
//     })>;

// non-empty contract for containers and spans

// using ufloat32 = non_negative<float32>;
// using ufloat64 = non_negative<float64>;
// using nfloat32 = normalized<float32>;
// using nfloat64 = normalized<float64>;

}  // namespace lyrahgames::xstd
