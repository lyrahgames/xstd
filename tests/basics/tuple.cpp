#include <iomanip>
#include <iostream>
#include <string>
//
#include <doctest/doctest.h>
//
#include <lyrahgames/xstd/tuple.hpp>
//
#include "log_value.hpp"

using namespace std;
using namespace lyrahgames;
using namespace xstd;

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

      CHECK(element<0>(x).value == 1);
      CHECK(element<1>(x).value == 2);
      CHECK(element<2>(x).value == string("Test"));

      static_assert(
          generic::instantiation<tuple<log_int, log_int, log_string>, tuple>);
      static_assert(
          generic::instantiation<tuple<log_int&, const log_int&, log_string&>,
                                 tuple>);

      log_int_state.counters[log_int_state.construct_calls] = 2;
      CHECK(log_int::log == log_int_state);
      log_string_state.counters[log_string_state.construct_calls] = 1;
      CHECK(log_string::log == log_string_state);

      xstd::tuple<log_int&, const log_int&, log_string&> y = x;

      CHECK(log_int::log == log_int_state);
      CHECK(log_string::log == log_string_state);

      CHECK(element<0>(x).value == 1);
      CHECK(element<1>(x).value == 2);
      CHECK(element<2>(x).value == string("Test"));

      CHECK(element<0>(y).value == 1);
      CHECK(element<1>(y).value == 2);
      CHECK(element<2>(y).value == string("Test"));

      ++element<0>(y).value;

      CHECK(element<0>(x).value == 2);
      CHECK(element<0>(y).value == 2);

      --element<1>(x).value;

      CHECK(element<1>(x).value == 1);
      CHECK(element<1>(y).value == 1);

      element<2>(x).value += " more";
      element<2>(y).value += " and even more";

      CHECK(element<2>(x).value == "Test more and even more");
      CHECK(element<2>(y).value == "Test more and even more");

      // tuple<log_int, log_int, log_string> z{
      //     move(element<0>(y)), move(element<1>(y)), move(element<2>(y))};
      tuple<log_int, log_int, log_string> z = std::move(y);
      // tuple<log_int, log_int, log_string> z{};
      // z = std::move(y);

      log_int_state.counters[log_int_state.move_construct_calls] += 1;
      log_int_state.counters[log_int_state.copy_construct_calls] += 1;
      CHECK(log_int::log == log_int_state);

      log_string_state.counters[log_string_state.move_construct_calls] += 1;
      CHECK(log_string::log == log_string_state);

      CHECK(element<0>(z).value == 2);
      CHECK(element<1>(z).value == 1);
      CHECK(element<2>(z).value == string("Test more and even more"));

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

      z = w;

      log_int_state.counters[log_int_state.copy_assign_calls] += 2;
      CHECK(log_int::log == log_int_state);
      log_string_state.counters[log_string_state.copy_assign_calls] += 1;
      CHECK(log_string::log == log_string_state);

      {
        const auto& [a, b, c] = z;
        CHECK(a.value == 10);
        CHECK(b.value == 20);
        CHECK(c.value == string("test"));
      }

      z = std::move(w);

      log_int_state.counters[log_int_state.move_assign_calls] += 1;
      log_int_state.counters[log_int_state.copy_assign_calls] += 1;
      CHECK(log_int::log == log_int_state);
      log_string_state.counters[log_string_state.move_assign_calls] += 1;
      CHECK(log_string::log == log_string_state);

      {
        const auto& [a, b, c] = z;
        CHECK(a.value == 10);
        CHECK(b.value == 20);
        CHECK(c.value == string("test"));
      }
    }

    // log_int_state.counters[log_int_state.destruct_calls] = 4;
    // CHECK(log_int::log == log_int_state);
    // log_string_state.counters[log_string_state.destruct_calls] = 2;
    // CHECK(log_string::log == log_string_state);
  }
}