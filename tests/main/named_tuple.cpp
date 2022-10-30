#include <doctest/doctest.h>
//
#include <iomanip>
#include <iostream>
#include <string>
//
#include <lyrahgames/xstd/named_tuple.hpp>
#include <lyrahgames/xstd/regular_tuple.hpp>

using namespace std;
using namespace lyrahgames;
using namespace xstd;

SCENARIO("Named Tuple Element Access") {
  SUBCASE("Irreducible Types") {
    using tuple_type = named_tuple<static_identifier_list<"id", "value">,
                                   regular_tuple<int, float>>;
    //
    tuple_type x{-1, 3.14f};
    const auto& y = x;

    SUBCASE("Access by Index") {
      static_assert(meta::equal<int&, decltype(value<0>(x))>);
      static_assert(meta::equal<float&, decltype(value<1>(x))>);
      static_assert(meta::equal<int&&, decltype(value<0>(move(x)))>);
      static_assert(meta::equal<float&&, decltype(value<1>(move(x)))>);
      CHECK(value<0>(x) == -1);
      CHECK(value<1>(x) == 3.14f);
      ++value<0>(x);
      value<1>(x) = 1.23f;
      CHECK(value<0>(x) == 0);
      CHECK(value<1>(x) == 1.23f);
      //
      static_assert(meta::equal<const int&, decltype(value<0>(y))>);
      static_assert(meta::equal<const float&, decltype(value<1>(y))>);
      static_assert(meta::equal<const int&&, decltype(value<0>(move(y)))>);
      static_assert(meta::equal<const float&&, decltype(value<1>(move(y)))>);
      CHECK(value<0>(y) == 0);
      CHECK(value<1>(y) == 1.23f);
    }
    SUBCASE("Access by Identifier") {
      static_assert(meta::equal<int&, decltype(value<"id">(x))>);
      static_assert(meta::equal<float&, decltype(value<"value">(x))>);
      static_assert(meta::equal<int&&, decltype(value<"id">(move(x)))>);
      static_assert(meta::equal<float&&, decltype(value<"value">(move(x)))>);
      CHECK(value<"id">(x) == -1);
      CHECK(value<"value">(x) == 3.14f);
      ++value<"id">(x);
      value<"value">(x) = 1.23f;
      CHECK(value<"id">(x) == 0);
      CHECK(value<"value">(x) == 1.23f);
      //
      static_assert(meta::equal<const int&, decltype(value<"id">(y))>);
      static_assert(meta::equal<const float&, decltype(value<"value">(y))>);
      static_assert(meta::equal<const int&&, decltype(value<"id">(move(y)))>);
      static_assert(
          meta::equal<const float&&, decltype(value<"value">(move(y)))>);
      CHECK(value<"id">(y) == 0);
      CHECK(value<"value">(y) == 1.23f);
    }
  }
}

SCENARIO("Regular Tuple Constructors and Assignments") {
  using tuple_type = named_tuple<static_identifier_list<"id", "name", "data">,
                                 regular_tuple<int, string, float>>;

  SUBCASE("Default Constructor") {
    tuple_type x{};
    CHECK(value<0>(x) == int{});
    CHECK(value<1>(x) == string{});
    CHECK(value<2>(x) == float{});
  }

  SUBCASE("Forward Constructor") {
    tuple_type x{-1, "Test", 3.14f};
    CHECK(value<0>(x) == -1);
    CHECK(value<1>(x) == "Test");
    CHECK(value<2>(x) == 3.14f);

    SUBCASE("Copy Constructor") {
      tuple_type y = x;
      CHECK(value<0>(y) == -1);
      CHECK(value<1>(y) == "Test");
      CHECK(value<2>(y) == 3.14f);

      SUBCASE("Copy Assignment") {
        value<0>(y) = 2;
        value<1>(y) += " 2";
        value<2>(y) = 1.23f;

        CHECK(value<0>(y) == 2);
        CHECK(value<1>(y) == "Test 2");
        CHECK(value<2>(y) == 1.23f);

        x = y;
        CHECK(value<0>(x) == 2);
        CHECK(value<1>(x) == "Test 2");
        CHECK(value<2>(x) == 1.23f);
      }

      SUBCASE("Move Assignment") {
        value<0>(y) = 2;
        value<1>(y) += " 2";
        value<2>(y) = 1.23f;

        x = move(y);
        CHECK(value<0>(x) == 2);
        CHECK(value<1>(x) == "Test 2");
        CHECK(value<2>(x) == 1.23f);
        //
        // CHECK(value<0>(y) == 2);
        // CHECK(value<1>(y) == "");
        // CHECK(value<2>(y) == 1.23f);
      }
    }

    SUBCASE("Move Constructor") {
      tuple_type y = move(x);
      CHECK(value<0>(y) == -1);
      CHECK(value<1>(y) == "Test");
      CHECK(value<2>(y) == 3.14f);
      //
      // CHECK(value<0>(x) == -1);
      // CHECK(value<1>(x) == "");
      // CHECK(value<2>(x) == 3.14f);
    }
  }

  SUBCASE("Conversions and Assignments from Convertible Tuples") {
    using other_tuple_type = regular_tuple<size_t, czstring, double>;
    other_tuple_type x{1, "Hello", 3.14};

    // Has to be down explicitly.
    tuple_type y{x};
    CHECK(value<0>(y) == 1);
    CHECK(value<1>(y) == "Hello");
    CHECK(value<2>(y) == 3.14f);

    value<0>(x) = 2;
    value<1>(x) = "Something";
    value<2>(x) = 1.23;

    y = x;
    CHECK(value<0>(y) == 2);
    CHECK(value<1>(y) == "Something");
    CHECK(value<2>(y) == 1.23f);
  }

  SUBCASE("Conversions and Assignments from Other Named Tuples") {
    //
    using other_tuple_type =
        named_tuple<static_identifier_list<"data", "useless", "id", "name">,
                    regular_tuple<double, char, size_t, czstring>>;
    other_tuple_type x{3.14, 'c', 1, "Test"};
    tuple_type y{x};
    CHECK(value<"id">(y) == 1);
    CHECK(value<"name">(y) == "Test");
    CHECK(value<"data">(y) == 3.14f);
  }
}

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
  // auto& [id, type, value] = package;

  // CHECK(id == 0);
  // CHECK(type == 'a');
  // CHECK(value == 1.23f);

  // ++id;
  // type = 'b';
  // value = 3.14f;

  // CHECK(id == 1);
  // CHECK(type == 'b');
  // CHECK(value == 3.14f);
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
