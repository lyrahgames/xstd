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

static_assert(substring<2, 4>("--help"_sz) == "help"_sz);
static_assert(prefix<2>("--help"_sz) == "--"_sz);
static_assert(tail<2>("--help"_sz) == "help"_sz);
static_assert(suffix<3>("my_file.txt"_sz) == "txt"_sz);
static_assert(prefix_match_index("version"_sz, "verbose"_sz) == 3);
static_assert(prefix_match_index("version"_sz, ""_sz) == 0);
static_assert(prefix_match_index("help"_sz, "help"_sz) == 4);

static_assert(instance::static_zstring<"help"_sz>);
static_assert(!instance::static_zstring<1>);

static_assert(is_static_zstring("help"_sz));
static_assert(!is_static_zstring("help"));
static_assert(!is_static_zstring(1.0f));
static_assert(!is_static_zstring('a'));

SCENARIO("static_zstring") {
  run<"TEST">();
  run<"Do something.">();
}
