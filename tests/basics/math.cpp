#include <random>
//
#include <doctest/doctest.h>
//
#include <lyrahgames/xstd/math.hpp>

using namespace std;
using namespace lyrahgames::xstd;

TEST_CASE("32-Bit Unsigned Integer Binary Logarithm") {
  CHECK(log2(uint32_t{0}) == uint32_t{0});
  CHECK(log2(uint32_t{1}) == uint32_t{0});
  CHECK(log2(uint32_t{2}) == uint32_t{1});

  for (uint32_t i = 0; i < 20; ++i) {
    const auto start = uint32_t{1} << i;
    const auto end = start << 1;
    for (uint32_t j = start; j < end; ++j)
      CHECK(log2(j) == i);
  }
}

TEST_CASE("64-Bit Unsigned Integer Binary Logarithm") {
  CHECK(log2(uint64_t{0}) == uint64_t{0});
  CHECK(log2(uint64_t{1}) == uint64_t{0});
  CHECK(log2(uint64_t{2}) == uint64_t{1});

  for (uint64_t i = 0; i < 20; ++i) {
    const auto start = uint64_t{1} << i;
    const auto end = start << 1;
    for (uint64_t j = start; j < end; ++j)
      CHECK(log2(j) == i);
  }
}

TEST_CASE("32-Bit Ceiling to Power of Two") {
  CHECK(ceil_pow2(uint32_t{0}) == uint32_t{0});
  CHECK(ceil_pow2(uint32_t{1}) == uint32_t{2});
  CHECK(ceil_pow2(uint32_t{2}) == uint32_t{2});
  CHECK(ceil_pow2(uint32_t{3}) == uint32_t{4});
  CHECK(ceil_pow2(uint32_t{4}) == uint32_t{4});
  CHECK(ceil_pow2(uint32_t{5}) == uint32_t{8});

  for (uint32_t i = 2; i < (uint32_t{1} << 20); i <<= 1) {
    CHECK(ceil_pow2(i) == i);
    for (uint32_t j = i + 1; j < (i << 1); ++j)
      CHECK(ceil_pow2(j) == (i << 1));
  }
}

TEST_CASE("32-Bit Ceiling to Power of Two") {
  CHECK(ceil_pow2(uint64_t{1}) == uint64_t{2});
  CHECK(ceil_pow2(uint64_t{2}) == uint64_t{2});
  CHECK(ceil_pow2(uint64_t{3}) == uint64_t{4});
  CHECK(ceil_pow2(uint64_t{4}) == uint64_t{4});
  CHECK(ceil_pow2(uint64_t{5}) == uint64_t{8});

  for (uint64_t i = 2; i < (uint64_t{1} << 20); i <<= 1) {
    CHECK(ceil_pow2(i) == i);
    for (uint64_t j = i + 1; j < (i << 1); ++j)
      CHECK(ceil_pow2(j) == (i << 1));
  }
}

namespace {

constexpr auto sq(auto x) noexcept {
  return x * x;
}

constexpr auto cb(auto x) noexcept {
  return sq(x) * x;
}

constexpr auto qd(auto x) noexcept {
  return sq(sq(x));
}

constexpr auto pt(auto x) noexcept {
  return sq(sq(x)) * x;
}

constexpr auto hx(auto x) noexcept {
  return sq(cb(x));
}

constexpr auto hp(auto x) noexcept {
  return sq(cb(x)) * x;
}

constexpr auto oc(auto x) noexcept {
  return sq(qd(x));
}

}  // namespace

SCENARIO("Power Function") {
  using namespace std;
  using lyrahgames::xstd::pow;
  using real = float;

  mt19937 rng{random_device{}()};
  normal_distribution<real> dist{};
  const auto random = [&]() { return dist(rng); };

  const size_t n = 1'000'000;
  for (size_t i = 0; i < n; ++i) {
    const auto x = random();
    CHECK(1 == pow<0>(x));
    CHECK(x == pow<1>(x));
    CHECK(sq(x) == pow<2>(x));
    CHECK(cb(x) == pow<3>(x));
    CHECK(qd(x) == pow<4>(x));
    CHECK(pt(x) == pow<5>(x));
    CHECK(hx(x) == pow<6>(x));
    CHECK(hp(x) == pow<7>(x));
    CHECK(oc(x) == pow<8>(x));
  }
}
