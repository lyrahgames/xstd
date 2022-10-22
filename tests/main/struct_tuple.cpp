#include <doctest/doctest.h>
//
#include <iomanip>
#include <iostream>
//
#include <string>
#include <tuple>
//
#include <lyrahgames/xstd/meta.hpp>
#include <lyrahgames/xstd/struct_tuple.hpp>

using namespace std;
using namespace lyrahgames;
using namespace xstd;

template <size_t... indices>
constexpr void static_for(index_sequence<indices...>, auto&& f) {
  (f.template operator()<indices>(), ...);
}

namespace {

struct empty_type {};

}  // namespace

SCENARIO("") {
  using type = struct_tuple<>;
  static_assert(sizeof(type) == 1);
  static_assert(alignof(type) == 1);
  static_assert(is_empty_v<type>);

  type_list<                     //
      struct_tuple<char>,        //
      struct_tuple<short>,       //
      struct_tuple<int>,         //
      struct_tuple<float>,       //
      struct_tuple<double>,      //
      struct_tuple<empty_type>,  //
      //
      struct_tuple<int, int>,                //
      struct_tuple<char, int>,               //
      struct_tuple<empty_type, empty_type>,  //
      //
      struct_tuple<empty_type, double, char, int>,  //
      //
      struct_tuple<>  //
      >::for_each([]<typename tuple_type> {
    using types = typename tuple_type::types;
    constexpr auto size = types::size;

    static_assert(alignof(tuple_type) == types::alignment);
    static_assert(sizeof(tuple_type) == types::struct_byte_size);

    tuple_type data;
    static_for(make_index_sequence<size>{}, [&data]<size_t i> {
      const auto offset =
          static_cast<size_t>(reinterpret_cast<uintptr_t>(&value<i>(data)) -
                              reinterpret_cast<uintptr_t>(&data));
      CHECK(offset == types::template struct_offset<i>);
    });
  });
}

SCENARIO("") {
  // struct A {
  //   A() noexcept {}
  // };

  // static_assert(is_default_constructible_v<A>);
  // static_assert(!is_trivially_default_constructible_v<A>);
  // static_assert(is_nothrow_default_constructible_v<A>);

  {
    using type = int&;
    static_assert(!is_default_constructible_v<type>);
    static_assert(!is_trivially_default_constructible_v<type>);
    static_assert(!is_nothrow_default_constructible_v<type>);
    //
    static_assert(is_copy_constructible_v<type>);
    static_assert(is_trivially_copy_constructible_v<type>);
    static_assert(is_nothrow_copy_constructible_v<type>);
    //
    static_assert(is_move_constructible_v<type>);
    static_assert(is_trivially_move_constructible_v<type>);
    static_assert(is_nothrow_move_constructible_v<type>);
    //
    static_assert(is_copy_assignable_v<type>);
    static_assert(is_trivially_copy_assignable_v<type>);
    static_assert(is_nothrow_copy_assignable_v<type>);
    //
    static_assert(is_move_assignable_v<type>);
    static_assert(is_trivially_move_assignable_v<type>);
    static_assert(is_nothrow_move_assignable_v<type>);
    //
    static_assert(is_destructible_v<type>);
    static_assert(is_trivially_destructible_v<type>);
    static_assert(is_nothrow_destructible_v<type>);
    static_assert(!has_virtual_destructor_v<type>);
    //
    static_assert(!is_trivial_v<type>);
    static_assert(!is_standard_layout_v<type>);

    struct A {
      type x;
      // reference_wrapper<int> x;
    };
    static_assert(!is_default_constructible_v<A>);
    static_assert(!is_trivially_default_constructible_v<A>);
    static_assert(!is_nothrow_default_constructible_v<A>);
    //
    static_assert(is_copy_constructible_v<A>);
    static_assert(is_trivially_copy_constructible_v<A>);
    static_assert(is_nothrow_copy_constructible_v<A>);
    //
    static_assert(is_move_constructible_v<A>);
    static_assert(is_trivially_move_constructible_v<A>);
    static_assert(is_nothrow_move_constructible_v<A>);
    //
    static_assert(!is_copy_assignable_v<A>);
    static_assert(!is_trivially_copy_assignable_v<A>);
    static_assert(!is_nothrow_copy_assignable_v<A>);
    //
    static_assert(!is_move_assignable_v<A>);
    static_assert(!is_trivially_move_assignable_v<A>);
    static_assert(!is_nothrow_move_assignable_v<A>);
    //
    static_assert(is_destructible_v<A>);
    static_assert(is_trivially_destructible_v<A>);
    static_assert(is_nothrow_destructible_v<A>);
    static_assert(!has_virtual_destructor_v<A>);
  }

  type_list<                     //
      struct_tuple<char>,        //
      struct_tuple<short>,       //
      struct_tuple<int>,         //
      struct_tuple<float>,       //
      struct_tuple<double>,      //
      struct_tuple<empty_type>,  //
      //
      struct_tuple<int, int>,                //
      struct_tuple<char, int>,               //
      struct_tuple<empty_type, empty_type>,  //
      //
      struct_tuple<empty_type, double, char, int>,  //
      //
      struct_tuple<>  //
      >::for_each([]<typename type> {
    static_assert(is_default_constructible_v<type>);
    static_assert(is_trivially_default_constructible_v<type>);
    static_assert(is_nothrow_default_constructible_v<type>);
  });
}

// SCENARIO("") {
//   struct_tuple<int, int> x{1, 2};

//   CHECK(value<0>(x) == 1);
//   CHECK(value<1>(x) == 2);

//   ++value<0>(x);
//   ++value<1>(x);

//   CHECK(value<0>(x) == 2);
//   CHECK(value<1>(x) == 3);

//   // struct_tuple<int&, int&> y = x;
// }

// SCENARIO("") {
//   auto x = struct_tuple{1, 2};

//   auto& [a, b] = x;

//   CHECK(value<0>(x) == 1);
//   CHECK(value<1>(x) == 2);
//   CHECK(a == 1);
//   CHECK(b == 2);

//   ++a;
//   ++b;

//   CHECK(value<0>(x) == 2);
//   CHECK(value<1>(x) == 3);
// }
