#include <doctest/doctest.h>
//
#include <iomanip>
#include <iostream>
#include <string>
//
#include <lyrahgames/xstd/named_type.hpp>

using namespace std;
using namespace lyrahgames::xstd;

SCENARIO("") {
  named_type<"message", string> x = "ab";
  x += 'c';
  x += x;
  MESSAGE(x);

  named_type<"id", int> y{};
  // y += 1;
  MESSAGE(y);
}
