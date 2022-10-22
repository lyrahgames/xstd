#include <doctest/doctest.h>
//
#include <iomanip>
#include <iostream>
//
#include <lyrahgames/xstd/static.hpp>

using namespace std;
using namespace lyrahgames;
using namespace xstd;

SCENARIO("") {
  using namespace meta::value_list;

  static_assert(meta::equal<iota<0>, value_list<>>);
  static_assert(meta::equal<iota<1>, value_list<size_t{0}>>);
  static_assert(meta::equal<iota<2>, value_list<size_t{0}, size_t{1}>>);
  static_assert(
      meta::equal<iota<3>, value_list<size_t{0}, size_t{1}, size_t{2}>>);
  static_assert(
      meta::equal<iota<4>,
                  value_list<size_t{0}, size_t{1}, size_t{2}, size_t{3}>>);
  static_assert(meta::equal<iota<5>, value_list<size_t{0}, size_t{1}, size_t{2},
                                                size_t{3}, size_t{4}>>);

  // The given offset at the same time
  // provides the type for the values of the list.
  //
  static_assert(meta::equal<iota<0, -3>, value_list<>>);
  static_assert(meta::equal<iota<1, -3>, value_list<-3>>);
  static_assert(meta::equal<iota<2, -3>, value_list<-3, -2>>);
  static_assert(meta::equal<iota<3, -3>, value_list<-3, -2, -1>>);
  static_assert(meta::equal<iota<4, -3>, value_list<-3, -2, -1, 0>>);
  static_assert(meta::equal<iota<5, -3>, value_list<-3, -2, -1, 0, 1>>);

  // The given offset at the same time
  // provides the type for the values of the list.
  //
  constexpr auto inc = [](auto x) { return x + 2; };
  static_assert(meta::equal<iota<0, -3, inc>, value_list<>>);
  static_assert(meta::equal<iota<1, -3, inc>, value_list<-3>>);
  static_assert(meta::equal<iota<2, -3, inc>, value_list<-3, -1>>);
  static_assert(meta::equal<iota<3, -3, inc>, value_list<-3, -1, 1>>);
  static_assert(meta::equal<iota<4, -3, inc>, value_list<-3, -1, 1, 3>>);
  static_assert(meta::equal<iota<5, -3, inc>, value_list<-3, -1, 1, 3, 5>>);
}
