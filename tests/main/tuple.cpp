#include <array>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
//
#include <doctest/doctest.h>
//
#include <lyrahgames/xstd/tuple.hpp>
//
#include "log_value.hpp"

using namespace std;
using namespace lyrahgames;
using namespace xstd;

namespace {

struct nothing {};

struct my_int {
  int value;
};

}  // namespace

SCENARIO("Tuple Value Wrapper Properties") {
  type_list<char, int, float, double, string, nothing, vector<int>,
            array<float, 10>>  //
      ::for_each([]<typename type> {
        using wrapped_type = detail::tuple::wrapper<0, type>;
        // Size ond Alignment
        static_assert(sizeof(type) == sizeof(wrapped_type));
        static_assert(alignof(type) == alignof(wrapped_type));
        // Default Construction
        static_assert(is_default_constructible_v<type> ==
                      is_default_constructible_v<wrapped_type>);
        static_assert(is_trivially_default_constructible_v<type> ==
                      is_trivially_default_constructible_v<wrapped_type>);
        static_assert(is_nothrow_default_constructible_v<type> ==
                      is_nothrow_default_constructible_v<wrapped_type>);
        // Copy Construction
        static_assert(is_copy_constructible_v<type> ==
                      is_copy_constructible_v<wrapped_type>);
        static_assert(is_trivially_copy_constructible_v<type> ==
                      is_trivially_copy_constructible_v<wrapped_type>);
        static_assert(is_nothrow_copy_constructible_v<type> ==
                      is_nothrow_copy_constructible_v<wrapped_type>);
        // Move Construction
        static_assert(is_move_constructible_v<type> ==
                      is_move_constructible_v<wrapped_type>);
        static_assert(is_trivially_move_constructible_v<type> ==
                      is_trivially_move_constructible_v<wrapped_type>);
        static_assert(is_nothrow_move_constructible_v<type> ==
                      is_nothrow_move_constructible_v<wrapped_type>);
        // Destruction
        static_assert(is_destructible_v<type> ==
                      is_destructible_v<wrapped_type>);
        static_assert(is_trivially_destructible_v<type> ==
                      is_trivially_destructible_v<wrapped_type>);
        static_assert(is_nothrow_destructible_v<type> ==
                      is_nothrow_destructible_v<wrapped_type>);
        static_assert(has_virtual_destructor_v<type> ==
                      has_virtual_destructor_v<wrapped_type>);
        // Copy Assignment
        static_assert(is_copy_assignable_v<type> ==
                      is_copy_assignable_v<wrapped_type>);
        static_assert(is_trivially_copy_assignable_v<type> ==
                      is_trivially_copy_assignable_v<wrapped_type>);
        static_assert(is_nothrow_copy_assignable_v<type> ==
                      is_nothrow_copy_assignable_v<wrapped_type>);
        // Move Assignment
        static_assert(is_move_assignable_v<type> ==
                      is_move_assignable_v<wrapped_type>);
        static_assert(is_trivially_move_assignable_v<type> ==
                      is_trivially_move_assignable_v<wrapped_type>);
        static_assert(is_nothrow_move_assignable_v<type> ==
                      is_nothrow_move_assignable_v<wrapped_type>);
        // Swap
        static_assert(is_swappable_v<type> == is_swappable_v<wrapped_type>);
        static_assert(is_nothrow_swappable_v<type> ==
                      is_nothrow_swappable_v<wrapped_type>);
        //
        // Clang erroneously evaluates the triviality to false.
        // static_assert(is_trivial_v<type> == is_trivial_v<wrapped_type>);
        static_assert(is_standard_layout_v<type> ==
                      is_standard_layout_v<wrapped_type>);
        static_assert(is_trivially_copyable_v<type> ==
                      is_trivially_copyable_v<wrapped_type>);
        //
        // Layout Compatibility is a too strong requirement.
        // static_assert(is_layout_compatible_v<type, wrapped_type>);
      });
}

SCENARIO("") {
  using xstd::tuple;
  static_assert(sizeof(tuple<>) == 1);
  static_assert(alignof(tuple<>) == 1);
  static_assert(is_empty_v<tuple<>>);
  static_assert(is_empty_v<tuple<nothing>>);
  //
  static_assert(sizeof(tuple<char>) == sizeof(char));
  static_assert(alignof(tuple<char>) == alignof(char));
  //
  static_assert(sizeof(tuple<short>) == sizeof(short));
  static_assert(alignof(tuple<short>) == alignof(short));
  //
  static_assert(sizeof(tuple<float>) == sizeof(float));
  static_assert(alignof(tuple<float>) == alignof(float));
  //
  static_assert(sizeof(tuple<double>) == sizeof(double));
  static_assert(alignof(tuple<double>) == alignof(double));

  {
    using type = tuple<>;
    static_assert(is_default_constructible_v<type>);
    static_assert(is_trivially_default_constructible_v<type>);
    static_assert(is_nothrow_default_constructible_v<type>);

    static_assert(is_trivially_destructible_v<type>);

    static_assert(is_trivially_copyable_v<type>);

    static_assert(is_standard_layout_v<type>);
    static_assert(is_trivial_v<type>);
  }

  //

  {
    static_assert(is_trivially_default_constructible_v<nothing>);

    using type = detail::tuple::wrapper<0, nothing>;
    static_assert(is_default_constructible_v<type>);
    static_assert(is_trivially_default_constructible_v<type>);
    static_assert(is_nothrow_default_constructible_v<type>);
  }
  {
    using type = tuple<int, char, double>;
    static_assert(is_default_constructible_v<type>);
    static_assert(is_trivially_default_constructible_v<type>);
    static_assert(is_nothrow_default_constructible_v<type>);

    static_assert(is_trivially_destructible_v<type>);
    // static_assert(is_trivially_copyable_v<type>);

    type x{};
    CHECK(value<0>(x) == 0);
    CHECK(value<1>(x) == '\0');
    CHECK(value<2>(x) == 0.0);
  }
}

SCENARIO("") {
  struct log::state log_int_state {};
  using log_int = basic_log_value<int, unique_log>;

  struct log::state log_string_state {};
  using log_string = basic_log_value<string, unique_log>;

  SUBCASE("") {
    reset(log_int::log);
    reset(log_string::log);

    {
      using xstd::tuple;
      tuple<log_int, log_int, log_string> x{1, 2, "Test"};

      CHECK(value<0>(x).value == 1);
      CHECK(value<1>(x).value == 2);
      CHECK(value<2>(x).value == string("Test"));

      // Check that only constructors have been called.
      log_int_state.counters[log_int_state.construct_calls] = 2;
      CHECK(log_int::log == log_int_state);
      log_string_state.counters[log_string_state.construct_calls] = 1;
      CHECK(log_string::log == log_string_state);

      tuple<log_int&, const log_int&, log_string&> y = x;

      CHECK(log_int::log == log_int_state);
      CHECK(log_string::log == log_string_state);

      CHECK(value<0>(x).value == 1);
      CHECK(value<1>(x).value == 2);
      CHECK(value<2>(x).value == string("Test"));

      CHECK(value<0>(y).value == 1);
      CHECK(value<1>(y).value == 2);
      CHECK(value<2>(y).value == string("Test"));

      ++value<0>(y).value;

      CHECK(value<0>(x).value == 2);
      CHECK(value<0>(y).value == 2);

      --value<1>(x).value;

      CHECK(value<1>(x).value == 1);
      CHECK(value<1>(y).value == 1);

      value<2>(x).value += " more";
      value<2>(y).value += " and even more";

      CHECK(value<2>(x).value == "Test more and even more");
      CHECK(value<2>(y).value == "Test more and even more");

      // tuple<log_int, log_int, log_string> z{
      //     move(value<0>(y)), move(value<1>(y)), move(value<2>(y))};
      tuple<log_int, log_int, log_string> z = std::move(y);
      // tuple<log_int, log_int, log_string> z{};
      // z = std::move(y);

      log_int_state.counters[log_int_state.copy_construct_calls] += 2;
      CHECK(log_int::log == log_int_state);

      log_string_state.counters[log_string_state.copy_construct_calls] += 1;
      CHECK(log_string::log == log_string_state);

      CHECK(value<0>(x).value == 2);
      CHECK(value<0>(y).value == 2);
      CHECK(value<0>(z).value == 2);

      CHECK(value<1>(x).value == 1);
      CHECK(value<1>(y).value == 1);
      CHECK(value<1>(z).value == 1);

      CHECK(value<2>(x).value == "Test more and even more");
      CHECK(value<2>(y).value == "Test more and even more");
      CHECK(value<2>(z).value == "Test more and even more");

      ++value<0>(y).value;
      --value<1>(x).value;

      CHECK(value<0>(x).value == 3);
      CHECK(value<0>(y).value == 3);
      CHECK(value<0>(z).value == 2);

      CHECK(value<1>(x).value == 0);
      CHECK(value<1>(y).value == 0);
      CHECK(value<1>(z).value == 1);

      CHECK(value<2>(x).value == "Test more and even more");
      CHECK(value<2>(y).value == "Test more and even more");
      CHECK(value<2>(z).value == "Test more and even more");

      //       CHECK(value<0>(z).value == 2);
      //       CHECK(value<1>(z).value == 1);
      //       CHECK(value<2>(z).value == string("Test more and even more"));

      {
        const auto& [a, b, c] = z;

        CHECK(log_int::log == log_int_state);
        CHECK(log_string::log == log_string_state);

        CHECK(a.value == 2);
        CHECK(b.value == 1);
        CHECK(c.value == string("Test more and even more"));
      }

      tuple<log_int, log_int, log_string> v{10, 20, "test"};

      log_int_state.counters[log_int_state.construct_calls] += 2;
      CHECK(log_int::log == log_int_state);
      log_string_state.counters[log_string_state.construct_calls] += 1;
      CHECK(log_string::log == log_string_state);

      {
        const auto& [a, b, c] = v;
        CHECK(a.value == 10);
        CHECK(b.value == 20);
        CHECK(c.value == string("test"));
      }

      // Cannot rassign y due to references.
      // y = v;
      tuple<log_int&, const log_int&, log_string&> w = v;

      CHECK(log_int::log == log_int_state);
      CHECK(log_string::log == log_string_state);

      {
        const auto& [a, b, c] = w;
        CHECK(a.value == 10);
        CHECK(b.value == 20);
        CHECK(c.value == string("test"));
      }

      // z = w;

      // log_int_state.counters[log_int_state.copy_assign_calls] += 2;
      // CHECK(log_int::log == log_int_state);
      // log_string_state.counters[log_string_state.copy_assign_calls] += 1;
      // CHECK(log_string::log == log_string_state);

      // {
      //   const auto& [a, b, c] = z;
      //   CHECK(a.value == 10);
      //   CHECK(b.value == 20);
      //   CHECK(c.value == string("test"));
      // }

      //       z = std::move(w);

      //       log_int_state.counters[log_int_state.move_assign_calls] += 1;
      //       log_int_state.counters[log_int_state.copy_assign_calls] += 1;
      //       CHECK(log_int::log == log_int_state);
      //       log_string_state.counters[log_string_state.move_assign_calls] +=
      //       1; CHECK(log_string::log == log_string_state);

      //       {
      //         const auto& [a, b, c] = z;
      //         CHECK(a.value == 10);
      //         CHECK(b.value == 20);
      //         CHECK(c.value == string("test"));
      //       }
    }

    // log_int_state.counters[log_int_state.destruct_calls] = 4;
    // CHECK(log_int::log == log_int_state);
    // log_string_state.counters[log_string_state.destruct_calls] = 2;
    // CHECK(log_string::log == log_string_state);
  }
}
