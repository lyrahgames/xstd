#include <string>
//
#include <doctest/doctest.h>
//
#include <lyrahgames/xstd/named_tuple.hpp>

using namespace std;
using namespace lyrahgames::xstd;

static_assert(is_static_zstring_list<static_zstring_list<>>);
static_assert(is_static_zstring_list<static_zstring_list<"first">>);
static_assert(is_static_zstring_list<static_zstring_list<"first", "second">>);
static_assert(!is_static_zstring_list<value_list<1, "second"_sz>>);

SCENARIO("Named Tuple") {
  auto package = auto_named_tuple<"id", "type", "value">(-1, 'c', 1.23f);
  using package_type = decltype(package);

  static_assert(package_type::contains<"id">);
  static_assert(package_type::contains<"type">);
  static_assert(package_type::contains<"value">);
  static_assert(!package_type::contains<"xyz">);

  CHECK(value<0>(package) == -1);
  CHECK(value<1>(package) == 'c');
  CHECK(value<2>(package) == 1.23f);

  CHECK(value<"id">(package) == -1);
  CHECK(value<"type">(package) == 'c');
  CHECK(value<"value">(package) == 1.23f);

  ++value<"id">(package);

  CHECK(value<"id">(package) == 0);
  CHECK(value<"type">(package) == 'c');
  CHECK(value<"value">(package) == 1.23f);

  value<"type">(package) = 'a';

  CHECK(value<"id">(package) == 0);
  CHECK(value<"type">(package) == 'a');
  CHECK(value<"value">(package) == 1.23f);

  package_type::names::for_each([&package]<auto name> {
    MESSAGE(name.data() << " = " << value<name>(package));
  });
}

SCENARIO("Named Tuple 2") {
  auto package0 = auto_named_tuple<"message">("This is some information.");

  int x = 1;
  auto package1 = push_back<"severity">(package0, x);

  auto package2 = push_back<"string">(package1, "Hello, World!"s);

  // meta::print_type(package0);
  // meta::print_type(package1);
  // meta::print_type(package2);
  // static_assert(meta::equal<decltype(package0)::types, type_list<const
  // char*>>);
  // MESSAGE(sizeof(package0));

  value<"message">(package2);
  value<"severity">(package2) = 2;

  // for_each with index set?
  std::decay_t<decltype(package2)>::names::for_each([&package2]<auto name> {
    MESSAGE(name.data() << " = " << value<name>(package2));
  });
}
