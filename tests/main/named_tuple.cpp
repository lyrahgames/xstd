#include <doctest/doctest.h>
//
#include <iomanip>
#include <iostream>
#include <string>
//
#include <lyrahgames/xstd/named_tuple.hpp>
#include <lyrahgames/xstd/regular_tuple.hpp>

using namespace std;
using namespace lyrahgames::xstd;

// static_assert(is_static_zstring_list<static_zstring_list<>>);
// static_assert(is_static_zstring_list<static_zstring_list<"first">>);
// static_assert(is_static_zstring_list<static_zstring_list<"first",
// "second">>); static_assert(!is_static_zstring_list<value_list<1,
// "second"_sz>>);

SCENARIO("Named Tuple") {
  auto package =
      auto_named_tuple<regular_tuple, "id", "type", "value">(-1, 'c', 1.23f);
  using package_type = decltype(package);

  // using package_type =
  //     named_tuple<static_identifier_list<"id", "type", "value">,
  //                 std::tuple<int, char, float>>;
  // package_type package{-1, 'c', 1.23f};

  // static_assert(package_type::contains<"id">);
  // static_assert(package_type::contains<"type">);
  // static_assert(package_type::contains<"value">);
  // static_assert(!package_type::contains<"xyz">);

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

  // Make sure structured bindings are still working for named tuples.
  auto& [id, type, value] = package;

  CHECK(id == 0);
  CHECK(type == 'a');
  CHECK(value == 1.23f);

  ++id;
  type = 'b';
  value = 3.14f;

  CHECK(id == 1);
  CHECK(type == 'b');
  CHECK(value == 3.14f);
}

// SCENARIO("Named Tuple 2") {
//   auto package0 = auto_named_tuple<"message">("This is some information.");

//   int x = 1;
//   auto package1 = push_back<"severity">(package0, x);

//   auto package2 = push_back<"string">(package1, "Hello, World!"s);

//   // meta::print_type(package0);
//   // meta::print_type(package1);
//   // meta::print_type(package2);
//   // static_assert(meta::equal<decltype(package0)::types, type_list<const
//   // char*>>);
//   // MESSAGE(sizeof(package0));

//   value<"message">(package2);
//   value<"severity">(package2) = 2;

//   // for_each with index set?
//   std::decay_t<decltype(package2)>::names::for_each([&package2]<auto name> {
//     MESSAGE(name.data() << " = " << value<name>(package2));
//   });
// }

// void print_package(auto&& package) {
//   using package_type = decay_t<decltype(package)>;
//   if constexpr (package_type::template contains<"severity">)
//     cout << (value<"severity">(forward<decltype(package)>(package))
//                  ? ("ERROR: ")
//                  : ("INFO: "));
//   if constexpr (package_type::template contains<"message">)
//     cout << value<"message">(forward<decltype(package)>(package));

//   using attributes = typename package_type::names::template try_remove_value<
//       "severity"_sz>::template try_remove_value<"message"_sz>;
//   attributes::for_each([&]<auto name> {
//     cout << " [" << name.data() << " = "
//          << value<name>(forward<decltype(package)>(package)) << "]";
//   });
//   cout << endl;
// }

// SCENARIO("") {
//   auto package0 = auto_named_tuple<"message">("This is some information.");
//   auto package1 = push_back<"severity">(package0, 1);
//   auto package2 = push_back<"note">(package0, "Hello, World!"s);

//   print_package(package0);
//   print_package(package1);
//   value<"severity">(package1) = 0;
//   print_package(package1);
//   print_package(package2);
// }

// SCENARIO("Offset of Named Tuple Elements") {
//   struct package_struct {
//     char first;
//     double second;
//     int third;
//   };
//   auto package = auto_named_tuple<"first", "second", "third">('c', 3.14, -1);

//   static_assert(sizeof(package) == sizeof(package_struct));
//   static_assert(alignof(package) == alignof(package_struct));
//   static_assert(offset<decltype(package), "first">() ==
//                 offsetof(package_struct, first));
//   static_assert(offset<decltype(package), "second">() ==
//                 offsetof(package_struct, second));
//   static_assert(offset<decltype(package), "third">() ==
//                 offsetof(package_struct, third));
// }
