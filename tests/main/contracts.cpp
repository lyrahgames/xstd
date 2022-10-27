#include <doctest/doctest.h>
//
#include <iomanip>
#include <iostream>
//
#include <lyrahgames/xstd/contracts.hpp>

using namespace std;
using namespace lyrahgames::xstd;

// constexpr auto compute(contract::positive auto x, auto y) {
//   return y / sqrt(x);
// }

SCENARIO("Contracts") {
  using lambda_type = decltype([](auto x) { return x > 0; });
  static_assert(semiregular<lambda_type>);
  static_assert(is_empty_v<lambda_type>);
  static_assert(generic::constraint<lambda_type, float>);
  static_assert(generic::constraint<lambda_type, int>);

  // contract<float, lambda_type{}> x{1.0f};
  // contract<int, lambda_type{}> y = x;
  // const float& v = x;
}
