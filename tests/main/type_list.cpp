#include <doctest/doctest.h>
//
#include <cstdint>
//
#include <lyrahgames/xstd/meta.hpp>
#include <lyrahgames/xstd/type_list/type_list.hpp>

// using namespace std;
using namespace lyrahgames::xstd;

template <typename T, size_t index>
struct test {
  using type = T;
};
template <typename T>
struct test<T, 0> {};

template <typename U, typename... T>
struct my_struct {
  using type = typename test<my_struct<U, T...>, sizeof...(T)>::type;
  using type2 = typename test<U, 0>::type;
};

using my_test = my_struct<int>;

// template <typename T, typename U>
// constexpr U my_function() noexcept;
// template <typename T, auto x>
// constexpr decltype(x) my_function() noexcept;
// static_assert(meta::equal<decltype(my_function<int, int>()), int>);
// static_assert(meta::equal<decltype(my_function<int, 10>()), int>);

// template <typename T, auto x>
// using my_alias = decltype(my_function<T, x>());
// template <typename T, typename x>
// using my_alias = decltype(my_function<T, x>());

// Using declarations cannot be overloaded or specialized.
// Concerning template parameters, only functions are allowed
// to overload them by using concepts or values.
// The problem is that each function is only able to return a value and not a
// type. This means that in either case the interface cannot be uniform and
// consistent.

// namespace my_struct {
// template <typename T>
// struct my_struct {};
// }  // namespace my_struct
// // template <typename T>
// // struct my_struct : my_struct::my_struct<T> {};
// namespace lyrahgames::xstd::meta {
// template <typename T>
// using my_struct = my_struct::my_struct<T>;
// }

// Type Lists should be enclosed by the namepace of the library.
// Problem: Helper functions need to be provided by separate namespace.
// struct type_list{};
// scope, core, meta, traits,
// namespace type_list_scope{}
// namespace detail::type_list{}
// namespace traits::type_list{}
// namespace scope::type_list{}
// namespace meta::type_list{}

using namespace meta::type_list;

// Check for emptiness
//
// Member Type Function
static_assert(type_list<>::empty);
static_assert(!type_list<int>::empty);
static_assert(!type_list<char, int>::empty);
static_assert(!type_list<char, int, double>::empty);
// Non-Member Type Function
static_assert(empty<type_list<>>);
static_assert(!empty<type_list<int>>);
static_assert(!empty<type_list<int, char>>);
static_assert(!empty<type_list<char, int, double>>);

// Get the number of types
//
// Member Type Function
static_assert(type_list<>::size == 0);
static_assert(type_list<int>::size == 1);
static_assert(type_list<char>::size == 1);
static_assert(type_list<int, char>::size == 2);
static_assert(type_list<double, int, char>::size == 3);
// Non-Member Type Function
static_assert(size<type_list<>> == 0);
static_assert(size<type_list<int>> == 1);
static_assert(size<type_list<char>> == 1);
static_assert(size<type_list<int, char>> == 2);
static_assert(size<type_list<int, char, double>> == 3);

// Byte Size, Alignment, Member Offsets, and Member Padding
//
namespace {
struct empty_type {};
}  // namespace
// Member and Non-Member Type Functions
static_assert(type_list<int>::alignment == alignof(int));
static_assert(alignment<type_list<int>> == alignof(int));
//
static_assert(type_list<int>::struct_byte_size == sizeof(int));
static_assert(struct_byte_size<type_list<int>> == sizeof(int));
//
static_assert(type_list<int>::struct_offset<0> == 0);
static_assert(struct_offset<type_list<int>, 0> == 0);
//
static_assert(type_list<int>::struct_padding<0> == 0);
static_assert(struct_padding<type_list<int>, 0> == 0);
//
//
namespace {
struct A {
  int a;
  char b;
};
using AT = type_list<int, char>;
}  // namespace
static_assert(AT::alignment == alignof(A));
static_assert(alignment<AT> == alignof(A));
//
static_assert(AT::struct_byte_size == sizeof(A));
static_assert(struct_byte_size<AT> == sizeof(A));
//
static_assert(AT::struct_offset<0> == offsetof(A, a));
static_assert(AT::struct_offset<1> == offsetof(A, b));
static_assert(struct_offset<AT, 0> == offsetof(A, a));
static_assert(struct_offset<AT, 1> == offsetof(A, b));
//
static_assert(AT::tuple_offset<0> == offsetof(A, a));
static_assert(AT::tuple_offset<1> == offsetof(A, b));
static_assert(tuple_offset<AT, 0> == offsetof(A, a));
static_assert(tuple_offset<AT, 1> == offsetof(A, b));
//
static_assert(AT::struct_padding<0> ==
              offsetof(A, b) - offsetof(A, a) - sizeof(A::a));
static_assert(AT::struct_padding<1> ==
              sizeof(A) - offsetof(A, b) - sizeof(A::b));
static_assert(struct_padding<AT, 0> == AT::struct_padding<0>);
static_assert(struct_padding<AT, 1> == AT::struct_padding<1>);
//
//
namespace {
struct B {
  char a;
  int b;
};
using BT = type_list<char, int>;
}  // namespace
static_assert(BT::alignment == alignof(B));
static_assert(alignment<BT> == alignof(B));
//
static_assert(BT::struct_byte_size == sizeof(B));
static_assert(struct_byte_size<BT> == sizeof(B));
//
static_assert(BT::struct_offset<0> == offsetof(B, a));
static_assert(BT::struct_offset<1> == offsetof(B, b));
static_assert(struct_offset<BT, 0> == offsetof(B, a));
static_assert(struct_offset<BT, 1> == offsetof(B, b));
//
static_assert(BT::tuple_offset<0> == offsetof(B, a));
static_assert(BT::tuple_offset<1> == offsetof(B, b));
static_assert(tuple_offset<BT, 0> == offsetof(B, a));
static_assert(tuple_offset<BT, 1> == offsetof(B, b));
//
static_assert(BT::struct_padding<0> ==
              offsetof(B, b) - offsetof(B, a) - sizeof(B::a));
static_assert(BT::struct_padding<1> ==
              sizeof(B) - offsetof(B, b) - sizeof(B::b));
static_assert(struct_padding<BT, 0> == BT::struct_padding<0>);
static_assert(struct_padding<BT, 1> == BT::struct_padding<1>);
//
//
namespace {
struct C {
  char a;
  int b;
  double c;
};
using CT = type_list<char, int, double>;
}  // namespace
static_assert(CT::alignment == alignof(C));
static_assert(alignment<CT> == alignof(C));
//
static_assert(CT::struct_byte_size == sizeof(C));
static_assert(struct_byte_size<CT> == sizeof(C));
//
static_assert(CT::struct_offset<0> == offsetof(C, a));
static_assert(CT::struct_offset<1> == offsetof(C, b));
static_assert(CT::struct_offset<2> == offsetof(C, c));
static_assert(struct_offset<CT, 0> == offsetof(C, a));
static_assert(struct_offset<CT, 1> == offsetof(C, b));
static_assert(struct_offset<CT, 2> == offsetof(C, c));
//
static_assert(CT::tuple_offset<0> == offsetof(C, a));
static_assert(CT::tuple_offset<1> == offsetof(C, b));
static_assert(CT::tuple_offset<2> == offsetof(C, c));
static_assert(tuple_offset<CT, 0> == offsetof(C, a));
static_assert(tuple_offset<CT, 1> == offsetof(C, b));
static_assert(tuple_offset<CT, 2> == offsetof(C, c));
//
static_assert(CT::struct_padding<0> ==
              offsetof(C, b) - offsetof(C, a) - sizeof(C::a));
static_assert(CT::struct_padding<1> ==
              offsetof(C, c) - offsetof(C, b) - sizeof(C::b));
static_assert(CT::struct_padding<2> ==
              sizeof(C) - offsetof(C, c) - sizeof(C::c));
static_assert(struct_padding<CT, 0> == CT::struct_padding<0>);
static_assert(struct_padding<CT, 1> == CT::struct_padding<1>);
static_assert(struct_padding<CT, 2> == CT::struct_padding<2>);
//
//
namespace {
struct D {
  char a;
  double b;
  int c;
};
using DT = type_list<char, double, int>;
}  // namespace
static_assert(DT::alignment == alignof(D));
static_assert(alignment<DT> == alignof(D));
//
static_assert(DT::struct_byte_size == sizeof(D));
static_assert(struct_byte_size<DT> == sizeof(D));
//
static_assert(DT::struct_offset<0> == offsetof(D, a));
static_assert(DT::struct_offset<1> == offsetof(D, b));
static_assert(DT::struct_offset<2> == offsetof(D, c));
static_assert(struct_offset<DT, 0> == offsetof(D, a));
static_assert(struct_offset<DT, 1> == offsetof(D, b));
static_assert(struct_offset<DT, 2> == offsetof(D, c));
//
static_assert(DT::tuple_offset<0> == offsetof(D, a));
static_assert(DT::tuple_offset<1> == offsetof(D, b));
static_assert(DT::tuple_offset<2> == offsetof(D, c));
static_assert(tuple_offset<DT, 0> == offsetof(D, a));
static_assert(tuple_offset<DT, 1> == offsetof(D, b));
static_assert(tuple_offset<DT, 2> == offsetof(D, c));
//
static_assert(DT::struct_padding<0> ==
              offsetof(D, b) - offsetof(D, a) - sizeof(D::a));
static_assert(DT::struct_padding<1> ==
              offsetof(D, c) - offsetof(D, b) - sizeof(D::b));
static_assert(DT::struct_padding<2> ==
              sizeof(D) - offsetof(D, c) - sizeof(D::c));
static_assert(struct_padding<DT, 0> == DT::struct_padding<0>);
static_assert(struct_padding<DT, 1> == DT::struct_padding<1>);
static_assert(struct_padding<DT, 2> == DT::struct_padding<2>);
//
//
namespace {
struct E {
  double a;
  char b;
  int c;
};
using ET = type_list<double, char, int>;
}  // namespace
static_assert(ET::alignment == alignof(E));
static_assert(alignment<ET> == alignof(E));
//
static_assert(ET::struct_byte_size == sizeof(E));
static_assert(struct_byte_size<ET> == sizeof(E));
//
static_assert(ET::struct_offset<0> == offsetof(E, a));
static_assert(ET::struct_offset<1> == offsetof(E, b));
static_assert(ET::struct_offset<2> == offsetof(E, c));
static_assert(struct_offset<ET, 0> == offsetof(E, a));
static_assert(struct_offset<ET, 1> == offsetof(E, b));
static_assert(struct_offset<ET, 2> == offsetof(E, c));
//
static_assert(ET::tuple_offset<0> == offsetof(E, a));
static_assert(ET::tuple_offset<1> == offsetof(E, b));
static_assert(ET::tuple_offset<2> == offsetof(E, c));
static_assert(tuple_offset<ET, 0> == offsetof(E, a));
static_assert(tuple_offset<ET, 1> == offsetof(E, b));
static_assert(tuple_offset<ET, 2> == offsetof(E, c));
//
static_assert(ET::struct_padding<0> ==
              offsetof(E, b) - offsetof(E, a) - sizeof(E::a));
static_assert(ET::struct_padding<1> ==
              offsetof(E, c) - offsetof(E, b) - sizeof(E::b));
static_assert(ET::struct_padding<2> ==
              sizeof(E) - offsetof(E, c) - sizeof(E::c));
static_assert(struct_padding<ET, 0> == ET::struct_padding<0>);
static_assert(struct_padding<ET, 1> == ET::struct_padding<1>);
static_assert(struct_padding<ET, 2> == ET::struct_padding<2>);
//
//
namespace {
struct F {
  char a;
  double b;
  empty_type c;
  int d;
};
// using FV = std::tuple<empty_type, double, char, int>;
using FV = std::tuple<char, double, empty_type, int>;
using FT = type_list<char, double, empty_type, int>;
}  // namespace
static_assert(FT::alignment == alignof(F));
static_assert(alignment<FT> == alignof(F));
//
static_assert(FT::struct_byte_size == sizeof(F));
static_assert(struct_byte_size<FT> == sizeof(F));
//
static_assert(FT::tuple_byte_size == sizeof(FV));
static_assert(tuple_byte_size<FT> == sizeof(FV));
//
static_assert(FT::struct_offset<0> == offsetof(F, a));
static_assert(FT::struct_offset<1> == offsetof(F, b));
static_assert(FT::struct_offset<2> == offsetof(F, c));
static_assert(FT::struct_offset<3> == offsetof(F, d));
static_assert(struct_offset<FT, 0> == offsetof(F, a));
static_assert(struct_offset<FT, 1> == offsetof(F, b));
static_assert(struct_offset<FT, 2> == offsetof(F, c));
static_assert(struct_offset<FT, 3> == offsetof(F, d));
//
// SCENARIO("") {
//   FV f{};

//   CHECK(FT::tuple_offset<0> ==
//         static_cast<size_t>(reinterpret_cast<uintptr_t>(&get<0>(f)) -
//                             reinterpret_cast<uintptr_t>(&f)));
//   CHECK(FT::tuple_offset<1> ==
//         static_cast<size_t>(reinterpret_cast<uintptr_t>(&get<1>(f)) -
//                             reinterpret_cast<uintptr_t>(&f)));
//   CHECK(FT::tuple_offset<2> ==
//         static_cast<size_t>(reinterpret_cast<uintptr_t>(&get<2>(f)) -
//                             reinterpret_cast<uintptr_t>(&f)));
//   CHECK(FT::tuple_offset<3> ==
//         static_cast<size_t>(reinterpret_cast<uintptr_t>(&get<3>(f)) -
//                             reinterpret_cast<uintptr_t>(&f)));
// }
static_assert(FT::tuple_offset<1> == offsetof(F, b));
static_assert(FT::tuple_offset<2> == offsetof(F, c));
static_assert(tuple_offset<FT, 0> == offsetof(F, a));
static_assert(tuple_offset<FT, 1> == offsetof(F, b));
static_assert(tuple_offset<FT, 2> == offsetof(F, c));
//
static_assert(FT::struct_padding<0> ==
              offsetof(F, b) - offsetof(F, a) - sizeof(F::a));
static_assert(FT::struct_padding<1> ==
              offsetof(F, c) - offsetof(F, b) - sizeof(F::b));
static_assert(FT::struct_padding<2> ==
              offsetof(F, d) - offsetof(F, c) - sizeof(F::c));
static_assert(FT::struct_padding<3> ==
              sizeof(F) - offsetof(F, d) - sizeof(F::d));
static_assert(struct_padding<FT, 0> == FT::struct_padding<0>);
static_assert(struct_padding<FT, 1> == FT::struct_padding<1>);
static_assert(struct_padding<FT, 2> == FT::struct_padding<2>);

// Check if type list contains a specific type.
//
// Member Type Function
static_assert(!type_list<>::contains<int>);
static_assert(!type_list<>::contains<char>);
static_assert(type_list<int>::contains<int>);
static_assert(!type_list<int>::contains<char>);
static_assert(type_list<int, char>::contains<char>);
static_assert(type_list<int, char>::contains<int>);
static_assert(!type_list<int, char>::contains<float>);
// Non-Member Type Function
static_assert(!contains<type_list<>, int>);
static_assert(!contains<type_list<>, char>);
static_assert(contains<type_list<int>, int>);
static_assert(!contains<type_list<int>, char>);
static_assert(contains<type_list<int, char>, char>);
static_assert(contains<type_list<int, char>, int>);
static_assert(!contains<type_list<int, char>, float>);

// Accessing types by index
//
// Member Type Function
static_assert(meta::equal<type_list<int, char>::element<0>, int>);
static_assert(meta::equal<type_list<int, char>::element<1>, char>);
static_assert(meta::equal<type_list<double, int, char>::element<0>, double>);
static_assert(meta::equal<type_list<double, int, char>::element<1>, int>);
static_assert(meta::equal<type_list<double, int, char>::element<2>, char>);
// Non-Member Type Function
static_assert(meta::equal<element<type_list<int>, 0>, int>);
static_assert(meta::equal<element<type_list<int, char>, 0>, int>);
static_assert(meta::equal<element<type_list<int, char>, 1>, char>);
static_assert(meta::equal<element<type_list<double, int, char>, 0>, double>);
static_assert(meta::equal<element<type_list<double, int, char>, 1>, int>);
static_assert(meta::equal<element<type_list<double, int, char>, 2>, char>);

// Get the index of a contained type
//
// Member Type Function
static_assert(type_list<int>::index<int> == 0);
static_assert(type_list<char, int>::index<char> == 0);
static_assert(type_list<char, int>::index<int> == 1);
static_assert(type_list<double, char, int>::index<double> == 0);
static_assert(type_list<double, char, int>::index<char> == 1);
static_assert(type_list<double, char, int>::index<int> == 2);
// Non-Member Type Function
static_assert(index<type_list<int>, int> == 0);
static_assert(index<type_list<char, int>, char> == 0);
static_assert(index<type_list<char, int>, int> == 1);
static_assert(index<type_list<double, char, int>, double> == 0);
static_assert(index<type_list<double, char, int>, char> == 1);
static_assert(index<type_list<double, char, int>, int> == 2);

// Accessing the front
//
// Member Type Function
static_assert(meta::equal<type_list<int>::front, int>);
static_assert(meta::equal<type_list<int, char>::front, int>);
static_assert(meta::equal<type_list<double, int, char>::front, double>);
// Non-Member Type Function
static_assert(meta::equal<front<type_list<int>>, int>);
static_assert(meta::equal<front<type_list<char, int>>, char>);
static_assert(meta::equal<front<type_list<double, char, int>>, double>);

// Accessing the back
//
// Member Type Function
static_assert(meta::equal<type_list<int>::back, int>);
static_assert(meta::equal<type_list<int, char>::back, char>);
static_assert(meta::equal<type_list<int, char, double>::back, double>);
// Non-Member Type Function
static_assert(meta::equal<back<type_list<int>>, int>);
static_assert(meta::equal<back<type_list<char, int>>, int>);
static_assert(meta::equal<back<type_list<double, char, int>>, int>);

static_assert(meta::equal<push_back<type_list<>, int>,  //
                          type_list<int>>);
static_assert(meta::equal<push_back<type_list<int>, char>,  //
                          type_list<int, char>>);

static_assert(meta::equal<type_list<>::push_back<int>, type_list<int>>);
static_assert(meta::equal<type_list<int>::push_back<char>,  //
                          type_list<int, char>>);

static_assert(meta::equal<type_list<>::push_front<int>, type_list<int>>);
static_assert(meta::equal<type_list<int>::push_front<char>,  //
                          type_list<char, int>>);

static_assert(meta::equal<push_front<type_list<>, int>, type_list<int>>);
static_assert(meta::equal<push_front<type_list<int>, char>,  //
                          type_list<char, int>>);

// Reverse the order of types inside a type list.
//
// Member Type Function
static_assert(meta::equal<type_list<>::reverse, type_list<>>);
static_assert(meta::equal<type_list<int>::reverse, type_list<int>>);
static_assert(meta::equal<type_list<int, char>::reverse, type_list<char, int>>);
static_assert(meta::equal<type_list<float, int, char>::reverse,
                          type_list<char, int, float>>);
// Non-Member Type Function
static_assert(meta::equal<reverse<type_list<>>, type_list<>>);
static_assert(meta::equal<reverse<type_list<int>>, type_list<int>>);
static_assert(meta::equal<reverse<type_list<int, char>>, type_list<char, int>>);
static_assert(meta::equal<reverse<type_list<float, int, char>>,
                          type_list<char, int, float>>);

static_assert(meta::equal<type_list<int>::pop_back, type_list<>>);
static_assert(meta::equal<type_list<int>::pop_front, type_list<>>);

static_assert(meta::equal<type_list<int, char>::pop_back, type_list<int>>);
static_assert(meta::equal<type_list<int, char>::pop_front, type_list<char>>);

static_assert(meta::equal<type_list<float, int, char>::trim_front<0>,
                          type_list<float, int, char>>);
static_assert(meta::equal<type_list<float, int, char>::trim_front<1>,
                          type_list<int, char>>);
static_assert(meta::equal<type_list<float, int, char>::trim_front<2>,  //
                          type_list<char>>);
static_assert(meta::equal<type_list<float, int, char>::trim_front<3>,  //
                          type_list<>>);

static_assert(meta::equal<type_list<float, int, char>::trim_back<0>,
                          type_list<float, int, char>>);
static_assert(meta::equal<type_list<float, int, char>::trim_back<1>,
                          type_list<float, int>>);
static_assert(meta::equal<type_list<float, int, char>::trim_back<2>,  //
                          type_list<float>>);
static_assert(meta::equal<type_list<float, int, char>::trim_back<3>,  //
                          type_list<>>);

// Get the sub type list of type list by providing range indices.
//
// Member Type Function
static_assert(meta::equal<                                       //
              type_list<float, double, int, char>::range<0, 0>,  //
              type_list<>>);
static_assert(meta::equal<                                       //
              type_list<float, double, int, char>::range<0, 1>,  //
              type_list<float>>);
static_assert(meta::equal<                                       //
              type_list<float, double, int, char>::range<0, 2>,  //
              type_list<float, double>>);
static_assert(meta::equal<                                       //
              type_list<float, double, int, char>::range<0, 3>,  //
              type_list<float, double, int>>);
static_assert(meta::equal<                                       //
              type_list<float, double, int, char>::range<0, 4>,  //
              type_list<float, double, int, char>>);
static_assert(meta::equal<                                       //
              type_list<float, double, int, char>::range<1, 4>,  //
              type_list<double, int, char>>);
static_assert(meta::equal<                                       //
              type_list<float, double, int, char>::range<2, 4>,  //
              type_list<int, char>>);
static_assert(meta::equal<                                       //
              type_list<float, double, int, char>::range<3, 4>,  //
              type_list<char>>);
static_assert(meta::equal<                                       //
              type_list<float, double, int, char>::range<4, 4>,  //
              type_list<>>);
static_assert(meta::equal<                                       //
              type_list<float, double, int, char>::range<1, 3>,  //
              type_list<double, int>>);
// Non-Member Type Function
static_assert(meta::equal<                                       //
              range<type_list<float, double, int, char>, 0, 0>,  //
              type_list<>>);
static_assert(meta::equal<                                       //
              range<type_list<float, double, int, char>, 0, 1>,  //
              type_list<float>>);
static_assert(meta::equal<                                       //
              range<type_list<float, double, int, char>, 0, 2>,  //
              type_list<float, double>>);
static_assert(meta::equal<                                       //
              range<type_list<float, double, int, char>, 0, 3>,  //
              type_list<float, double, int>>);
static_assert(meta::equal<                                       //
              range<type_list<float, double, int, char>, 0, 4>,  //
              type_list<float, double, int, char>>);
static_assert(meta::equal<                                       //
              range<type_list<float, double, int, char>, 1, 4>,  //
              type_list<double, int, char>>);
static_assert(meta::equal<                                       //
              range<type_list<float, double, int, char>, 2, 4>,  //
              type_list<int, char>>);
static_assert(meta::equal<                                       //
              range<type_list<float, double, int, char>, 3, 4>,  //
              type_list<char>>);
static_assert(meta::equal<                                       //
              range<type_list<float, double, int, char>, 4, 4>,  //
              type_list<>>);
static_assert(meta::equal<                                       //
              range<type_list<float, double, int, char>, 1, 3>,  //
              type_list<double, int>>);

// Swap the order of two types given their indices.
//
// Member Type Function
static_assert(meta::equal<type_list<float, double, int, char>::swap<0, 0>,  //
                          type_list<float, double, int, char>>);
static_assert(meta::equal<type_list<float, double, int, char>::swap<0, 1>,  //
                          type_list<double, float, int, char>>);
static_assert(meta::equal<type_list<float, double, int, char>::swap<0, 2>,  //
                          type_list<int, double, float, char>>);
static_assert(meta::equal<type_list<float, double, int, char>::swap<0, 3>,  //
                          type_list<char, double, int, float>>);
//
static_assert(meta::equal<type_list<float, double, int, char>::swap<1, 0>,  //
                          type_list<double, float, int, char>>);
static_assert(meta::equal<type_list<float, double, int, char>::swap<1, 1>,  //
                          type_list<float, double, int, char>>);
static_assert(meta::equal<type_list<float, double, int, char>::swap<1, 2>,  //
                          type_list<float, int, double, char>>);
static_assert(meta::equal<type_list<float, double, int, char>::swap<1, 3>,  //
                          type_list<float, char, int, double>>);
//
static_assert(meta::equal<type_list<float, double, int, char>::swap<2, 0>,  //
                          type_list<int, double, float, char>>);
static_assert(meta::equal<type_list<float, double, int, char>::swap<2, 1>,  //
                          type_list<float, int, double, char>>);
static_assert(meta::equal<type_list<float, double, int, char>::swap<2, 2>,  //
                          type_list<float, double, int, char>>);
static_assert(meta::equal<type_list<float, double, int, char>::swap<2, 3>,  //
                          type_list<float, double, char, int>>);
//
static_assert(meta::equal<type_list<float, double, int, char>::swap<3, 0>,  //
                          type_list<char, double, int, float>>);
static_assert(meta::equal<type_list<float, double, int, char>::swap<3, 1>,  //
                          type_list<float, char, int, double>>);
static_assert(meta::equal<type_list<float, double, int, char>::swap<3, 2>,  //
                          type_list<float, double, char, int>>);
static_assert(meta::equal<type_list<float, double, int, char>::swap<3, 3>,  //
                          type_list<float, double, int, char>>);
//
// Non-Member Type Function
static_assert(meta::equal<swap<type_list<float, double, int, char>, 0, 0>,  //
                          type_list<float, double, int, char>>);
static_assert(meta::equal<swap<type_list<float, double, int, char>, 0, 1>,  //
                          type_list<double, float, int, char>>);
static_assert(meta::equal<swap<type_list<float, double, int, char>, 0, 2>,  //
                          type_list<int, double, float, char>>);
static_assert(meta::equal<swap<type_list<float, double, int, char>, 0, 3>,  //
                          type_list<char, double, int, float>>);
//
static_assert(meta::equal<swap<type_list<float, double, int, char>, 1, 0>,  //
                          type_list<double, float, int, char>>);
static_assert(meta::equal<swap<type_list<float, double, int, char>, 1, 1>,  //
                          type_list<float, double, int, char>>);
static_assert(meta::equal<swap<type_list<float, double, int, char>, 1, 2>,  //
                          type_list<float, int, double, char>>);
static_assert(meta::equal<swap<type_list<float, double, int, char>, 1, 3>,  //
                          type_list<float, char, int, double>>);
//
static_assert(meta::equal<swap<type_list<float, double, int, char>, 2, 0>,  //
                          type_list<int, double, float, char>>);
static_assert(meta::equal<swap<type_list<float, double, int, char>, 2, 1>,  //
                          type_list<float, int, double, char>>);
static_assert(meta::equal<swap<type_list<float, double, int, char>, 2, 2>,  //
                          type_list<float, double, int, char>>);
static_assert(meta::equal<swap<type_list<float, double, int, char>, 2, 3>,  //
                          type_list<float, double, char, int>>);
//
static_assert(meta::equal<swap<type_list<float, double, int, char>, 3, 0>,  //
                          type_list<char, double, int, float>>);
static_assert(meta::equal<swap<type_list<float, double, int, char>, 3, 1>,  //
                          type_list<float, char, int, double>>);
static_assert(meta::equal<swap<type_list<float, double, int, char>, 3, 2>,  //
                          type_list<float, double, char, int>>);
static_assert(meta::equal<swap<type_list<float, double, int, char>, 3, 3>,  //
                          type_list<float, double, int, char>>);

static_assert(meta::equal<type_list<>::insert<0, int>, type_list<int>>);
//
static_assert(meta::equal<type_list<char>::insert<0, int>,  //
                          type_list<int, char>>);
static_assert(meta::equal<type_list<char>::insert<1, int>,  //
                          type_list<char, int>>);
//
static_assert(meta::equal<type_list<int, char>::insert<0, float>,  //
                          type_list<float, int, char>>);
static_assert(meta::equal<type_list<int, char>::insert<1, float>,  //
                          type_list<int, float, char>>);
static_assert(meta::equal<type_list<int, char>::insert<2, float>,  //
                          type_list<int, char, float>>);
//
static_assert(meta::equal<type_list<int, char, float>::insert<0, double>,  //
                          type_list<double, int, char, float>>);
static_assert(meta::equal<type_list<int, char, float>::insert<1, double>,  //
                          type_list<int, double, char, float>>);
static_assert(meta::equal<type_list<int, char, float>::insert<2, double>,  //
                          type_list<int, char, double, float>>);
static_assert(meta::equal<type_list<int, char, float>::insert<3, double>,  //
                          type_list<int, char, float, double>>);
//
// Non-Member Type Function
static_assert(meta::equal<insert<type_list<>, 0, int>, type_list<int>>);
//
static_assert(meta::equal<insert<type_list<char>, 0, int>,  //
                          type_list<int, char>>);
static_assert(meta::equal<insert<type_list<char>, 1, int>,  //
                          type_list<char, int>>);
//
static_assert(meta::equal<insert<type_list<int, char>, 0, float>,  //
                          type_list<float, int, char>>);
static_assert(meta::equal<insert<type_list<int, char>, 1, float>,  //
                          type_list<int, float, char>>);
static_assert(meta::equal<insert<type_list<int, char>, 2, float>,  //
                          type_list<int, char, float>>);
//
static_assert(meta::equal<insert<type_list<int, char, float>, 0, double>,  //
                          type_list<double, int, char, float>>);
static_assert(meta::equal<insert<type_list<int, char, float>, 1, double>,  //
                          type_list<int, double, char, float>>);
static_assert(meta::equal<insert<type_list<int, char, float>, 2, double>,  //
                          type_list<int, char, double, float>>);
static_assert(meta::equal<insert<type_list<int, char, float>, 3, double>,  //
                          type_list<int, char, float, double>>);

// Remove a type inside the type list given its index
//
// Member Type Function
static_assert(meta::equal<type_list<int>::remove<0>, type_list<>>);
//
static_assert(meta::equal<type_list<int, char>::remove<0>, type_list<char>>);
static_assert(meta::equal<type_list<int, char>::remove<1>, type_list<int>>);
//
static_assert(meta::equal<type_list<int, char, float>::remove<0>,  //
                          type_list<char, float>>);
static_assert(meta::equal<type_list<int, char, float>::remove<1>,  //
                          type_list<int, float>>);
static_assert(meta::equal<type_list<int, char, float>::remove<2>,  //
                          type_list<int, char>>);
//
static_assert(meta::equal<type_list<int, char, float, double>::remove<0>,  //
                          type_list<char, float, double>>);
static_assert(meta::equal<type_list<int, char, float, double>::remove<1>,  //
                          type_list<int, float, double>>);
static_assert(meta::equal<type_list<int, char, float, double>::remove<2>,  //
                          type_list<int, char, double>>);
static_assert(meta::equal<type_list<int, char, float, double>::remove<3>,  //
                          type_list<int, char, float>>);
//
// Non-Member Type Function
static_assert(meta::equal<remove<type_list<int>, 0>, type_list<>>);
//
static_assert(meta::equal<remove<type_list<int, char>, 0>, type_list<char>>);
static_assert(meta::equal<remove<type_list<int, char>, 1>, type_list<int>>);
//
static_assert(meta::equal<remove<type_list<int, char, float>, 0>,  //
                          type_list<char, float>>);
static_assert(meta::equal<remove<type_list<int, char, float>, 1>,  //
                          type_list<int, float>>);
static_assert(meta::equal<remove<type_list<int, char, float>, 2>,  //
                          type_list<int, char>>);
//
static_assert(meta::equal<remove<type_list<int, char, float, double>, 0>,  //
                          type_list<char, float, double>>);
static_assert(meta::equal<remove<type_list<int, char, float, double>, 1>,  //
                          type_list<int, float, double>>);
static_assert(meta::equal<remove<type_list<int, char, float, double>, 2>,  //
                          type_list<int, char, double>>);
static_assert(meta::equal<remove<type_list<int, char, float, double>, 3>,  //
                          type_list<int, char, float>>);

// Assign values by their index
//
// Member Type Function
static_assert(
    meta::equal<type_list<int>::assignment<0, char>, type_list<char>>);
//
static_assert(meta::equal<type_list<int, char>::assignment<0, float>,
                          type_list<float, char>>);
static_assert(meta::equal<type_list<int, char>::assignment<1, float>,
                          type_list<int, float>>);
//
static_assert(meta::equal<type_list<int, char, float>::assignment<0, double>,
                          type_list<double, char, float>>);
static_assert(meta::equal<type_list<int, char, float>::assignment<1, double>,
                          type_list<int, double, float>>);
static_assert(meta::equal<type_list<int, char, float>::assignment<2, double>,
                          type_list<int, char, double>>);
//
// Non-Member Type Function
static_assert(
    meta::equal<assignment<type_list<int>, 0, char>, type_list<char>>);
//
static_assert(meta::equal<assignment<type_list<int, char>, 0, float>,
                          type_list<float, char>>);
static_assert(meta::equal<assignment<type_list<int, char>, 1, float>,
                          type_list<int, float>>);
//
static_assert(meta::equal<assignment<type_list<int, char, float>, 0, double>,
                          type_list<double, char, float>>);
static_assert(meta::equal<assignment<type_list<int, char, float>, 1, double>,
                          type_list<int, double, float>>);
static_assert(meta::equal<assignment<type_list<int, char, float>, 2, double>,
                          type_list<int, char, double>>);

template <typename T, typename U>
struct inc_size {
  static constexpr bool value = (sizeof(T) <= sizeof(U));
};
template <typename T, typename U>
struct dec_size {
  static constexpr bool value = (sizeof(T) >= sizeof(U));
};

// Sort the contained types of a type list given an order relation predicate.
//
// Member Type Function
static_assert(meta::equal<type_list<>::sort<inc_size>, type_list<>>);
static_assert(meta::equal<type_list<int>::sort<inc_size>, type_list<int>>);
static_assert(meta::equal<type_list<int, char>::sort<inc_size>,  //
                          type_list<char, int>>);
static_assert(meta::equal<type_list<int, double>::sort<inc_size>,  //
                          type_list<int, double>>);
static_assert(meta::equal<type_list<int, double, short>::sort<inc_size>,  //
                          type_list<short, int, double>>);
static_assert(meta::equal<                                                //
              type_list<int32_t, char, int64_t, double>::sort<inc_size>,  //
              type_list<char, int32_t, int64_t, double>>);
static_assert(meta::equal<                                                //
              type_list<int32_t, char, int64_t, double>::sort<dec_size>,  //
              type_list<int64_t, double, int32_t, char>>);
//
// Non-Member Type Function
static_assert(meta::equal<sort<type_list<>, inc_size>, type_list<>>);
static_assert(meta::equal<sort<type_list<int>, inc_size>, type_list<int>>);
static_assert(meta::equal<sort<type_list<int, char>, inc_size>,  //
                          type_list<char, int>>);
static_assert(meta::equal<sort<type_list<int, double>, inc_size>,  //
                          type_list<int, double>>);
static_assert(meta::equal<sort<type_list<int, double, short>, inc_size>,  //
                          type_list<short, int, double>>);
static_assert(meta::equal<  //
              sort<type_list<int32_t, char, int64_t, double>, inc_size>,
              type_list<char, int32_t, int64_t, double>>);
static_assert(meta::equal<  //
              sort<type_list<int32_t, char, int64_t, double>, dec_size>,
              type_list<int64_t, double, int32_t, char>>);

// Insert a given type into a type list
// at the first position where the given condition is true.
//
// Member Type Function
static_assert(meta::equal<                              //
              type_list<>::insert_when<int, inc_size>,  //
              type_list<int>>);
//
static_assert(meta::equal<                                  //
              type_list<char>::insert_when<int, inc_size>,  //
              type_list<char, int>>);
static_assert(meta::equal<                                    //
              type_list<double>::insert_when<int, inc_size>,  //
              type_list<int, double>>);
static_assert(meta::equal<                                          //
              type_list<unsigned int>::insert_when<int, inc_size>,  //
              type_list<int, unsigned int>>);
//
static_assert(meta::equal<                                          //
              type_list<char, double>::insert_when<int, inc_size>,  //
              type_list<char, int, double>>);
static_assert(meta::equal<                                                 //
              type_list<char, short, int>::insert_when<double, inc_size>,  //
              type_list<char, short, int, double>>);
//
// Non-Member Type Function
static_assert(meta::equal<  //
              insert_when<type_list<>, int, inc_size>,
              type_list<int>>);
//
static_assert(meta::equal<  //
              insert_when<type_list<char>, int, inc_size>,
              type_list<char, int>>);
static_assert(meta::equal<  //
              insert_when<type_list<double>, int, inc_size>,
              type_list<int, double>>);
static_assert(meta::equal<  //
              insert_when<type_list<unsigned int>, int, inc_size>,
              type_list<int, unsigned int>>);
//
static_assert(meta::equal<  //
              insert_when<type_list<char, double>, int, inc_size>,
              type_list<char, int, double>>);
static_assert(meta::equal<  //
              insert_when<type_list<char, short, int>, double, inc_size>,
              type_list<char, short, int, double>>);
