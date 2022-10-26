#include <doctest/doctest.h>
//
#include <lyrahgames/xstd/packed_tuple.hpp>

using namespace std;
using namespace lyrahgames::xstd;

SCENARIO("") {
  {
    using type = packed_tuple<>;
    static_assert(sizeof(type) == 1);
    static_assert(alignof(type) == 1);
    static_assert(is_empty_v<type>);
  }
  {
    using type = packed_tuple<int>;
    static_assert(sizeof(type) == sizeof(int));
    static_assert(alignof(type) == alignof(int));
  }

  using tuple_type = packed_tuple<uint8, uint64, uint8, uint32, uint16>;
  static_assert(sizeof(tuple_type) == 16);
  static_assert(alignof(tuple_type) == 8);
  static_assert(
      meta::equal<tuple_type::base,
                  regular_tuple<uint64, uint32, uint16, uint8, uint8>>);
  static_assert(meta::equal<typename tuple_type::template type<0>, uint8>);
  static_assert(meta::equal<typename tuple_type::template type<1>, uint64>);
  static_assert(meta::equal<typename tuple_type::template type<2>, uint8>);
  static_assert(meta::equal<typename tuple_type::template type<3>, uint32>);
  static_assert(meta::equal<typename tuple_type::template type<4>, uint16>);
  tuple_type x{1, 2, 3, 4, 5};
  meta::static_index_list::iota<tuple_type::size()>::for_each([&]<auto index> {
    MESSAGE(index << ' ' << (int)value<index>(x) << ' '
                  << (int)value<index>(x.tuple()));
  });
}
