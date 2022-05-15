#include <doctest/doctest.h>
//
#include <iomanip>
#include <iostream>
//
#include <lyrahgames/xstd/static_zstring.hpp>

using namespace std;
using namespace lyrahgames::xstd;

template <static_zstring name>
void run() {
  cout << static_zstring("undefined command ") + '"' + name + '"' << endl;
}

template <>
void run<"Do something.">() {
  cout << "I am doing something." << endl;
}

SCENARIO("static_zstring") {
  run<"TEST">();
  run<"Do something.">();
}
