#include <doctest/doctest.h>
//
#include <lyrahgames/xstd/meta.hpp>
#include <lyrahgames/xstd/type_list.hpp>

using namespace std;
using namespace lyrahgames::xstd;

static_assert(type_list<>::empty);
static_assert(type_list<>::size == 0);
static_assert(!type_list<int>::empty);
static_assert(type_list<int>::size == 1);
static_assert(type_list<char>::size == 1);
static_assert(type_list<int, char>::size == 2);

static_assert(meta::empty<type_list<>>);
static_assert(meta::size<type_list<>> == 0);
static_assert(!meta::empty<type_list<int>>);
static_assert(meta::size<type_list<int>> == 1);
static_assert(meta::size<type_list<char>> == 1);
static_assert(meta::size<type_list<int, char>> == 2);

static_assert(meta::equal<meta::push_back<type_list<>, int>, type_list<int>>);
static_assert(
    meta::equal<meta::push_back<type_list<int>, char>, type_list<int, char>>);

static_assert(meta::equal<type_list<>::push_back<int>, type_list<int>>);
static_assert(
    meta::equal<type_list<int>::push_back<char>, type_list<int, char>>);

static_assert(meta::equal<type_list<>::push_front<int>, type_list<int>>);
static_assert(
    meta::equal<type_list<int>::push_front<char>, type_list<char, int>>);

static_assert(meta::equal<meta::push_front<type_list<>, int>, type_list<int>>);
static_assert(
    meta::equal<meta::push_front<type_list<int>, char>, type_list<char, int>>);

static_assert(!type_list<>::contains<int>);
static_assert(!type_list<>::contains<char>);
static_assert(type_list<int>::contains<int>);
static_assert(!type_list<int>::contains<char>);
static_assert(type_list<int, char>::contains<char>);
static_assert(type_list<int, char>::contains<int>);
static_assert(!type_list<int, char>::contains<float>);

static_assert(!meta::contains<type_list<>, int>);
static_assert(!meta::contains<type_list<>, char>);
static_assert(meta::contains<type_list<int>, int>);
static_assert(!meta::contains<type_list<int>, char>);
static_assert(meta::contains<type_list<int, char>, char>);
static_assert(meta::contains<type_list<int, char>, int>);
static_assert(!meta::contains<type_list<int, char>, float>);

static_assert(meta::equal<type_list<int, char>::element<0>, int>);
static_assert(meta::equal<type_list<int, char>::element<1>, char>);

static_assert(meta::equal<meta::element<type_list<int, char>, 0>, int>);
static_assert(meta::equal<meta::element<type_list<int, char>, 1>, char>);

static_assert(meta::equal<type_list<int>::front, int>);
static_assert(meta::equal<type_list<int>::back, int>);
static_assert(meta::equal<type_list<int, char>::front, int>);
static_assert(meta::equal<type_list<int, char>::back, char>);

static_assert(meta::equal<type_list<>::reverse, type_list<>>);
static_assert(meta::equal<type_list<int>::reverse, type_list<int>>);
static_assert(meta::equal<type_list<int, char>::reverse, type_list<char, int>>);
static_assert(meta::equal<type_list<float, int, char>::reverse,
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

static_assert(meta::equal<type_list<float, double, int, char>::range<0, 0>,  //
                          type_list<>>);
static_assert(meta::equal<type_list<float, double, int, char>::range<0, 1>,  //
                          type_list<float>>);
static_assert(meta::equal<type_list<float, double, int, char>::range<0, 2>,  //
                          type_list<float, double>>);
static_assert(meta::equal<type_list<float, double, int, char>::range<0, 3>,  //
                          type_list<float, double, int>>);
static_assert(meta::equal<type_list<float, double, int, char>::range<0, 4>,  //
                          type_list<float, double, int, char>>);

static_assert(meta::equal<type_list<float, double, int, char>::range<1, 4>,  //
                          type_list<double, int, char>>);
static_assert(meta::equal<type_list<float, double, int, char>::range<2, 4>,  //
                          type_list<int, char>>);
static_assert(meta::equal<type_list<float, double, int, char>::range<3, 4>,  //
                          type_list<char>>);
static_assert(meta::equal<type_list<float, double, int, char>::range<4, 4>,  //
                          type_list<>>);

static_assert(meta::equal<type_list<float, double, int, char>::range<1, 3>,  //
                          type_list<double, int>>);

static_assert(meta::equal<type_list<float, double, int, char>::swap<0, 0>,  //
                          type_list<float, double, int, char>>);
static_assert(meta::equal<type_list<float, double, int, char>::swap<0, 1>,  //
                          type_list<double, float, int, char>>);
static_assert(meta::equal<type_list<float, double, int, char>::swap<0, 2>,  //
                          type_list<int, double, float, char>>);
static_assert(meta::equal<type_list<float, double, int, char>::swap<0, 3>,  //
                          type_list<char, double, int, float>>);

static_assert(meta::equal<type_list<float, double, int, char>::swap<1, 0>,  //
                          type_list<double, float, int, char>>);
static_assert(meta::equal<type_list<float, double, int, char>::swap<1, 1>,  //
                          type_list<float, double, int, char>>);
static_assert(meta::equal<type_list<float, double, int, char>::swap<1, 2>,  //
                          type_list<float, int, double, char>>);
static_assert(meta::equal<type_list<float, double, int, char>::swap<1, 3>,  //
                          type_list<float, char, int, double>>);

static_assert(meta::equal<type_list<float, double, int, char>::swap<2, 0>,  //
                          type_list<int, double, float, char>>);
static_assert(meta::equal<type_list<float, double, int, char>::swap<2, 1>,  //
                          type_list<float, int, double, char>>);
static_assert(meta::equal<type_list<float, double, int, char>::swap<2, 2>,  //
                          type_list<float, double, int, char>>);
static_assert(meta::equal<type_list<float, double, int, char>::swap<2, 3>,  //
                          type_list<float, double, char, int>>);

static_assert(meta::equal<type_list<float, double, int, char>::swap<3, 0>,  //
                          type_list<char, double, int, float>>);
static_assert(meta::equal<type_list<float, double, int, char>::swap<3, 1>,  //
                          type_list<float, char, int, double>>);
static_assert(meta::equal<type_list<float, double, int, char>::swap<3, 2>,  //
                          type_list<float, double, char, int>>);
static_assert(meta::equal<type_list<float, double, int, char>::swap<3, 3>,  //
                          type_list<float, double, int, char>>);

static_assert(meta::equal<type_list<>::insert<0, int>, type_list<int>>);

static_assert(meta::equal<type_list<char>::insert<0, int>,  //
                          type_list<int, char>>);
static_assert(meta::equal<type_list<char>::insert<1, int>,  //
                          type_list<char, int>>);

static_assert(meta::equal<type_list<int, char>::insert<0, float>,  //
                          type_list<float, int, char>>);
static_assert(meta::equal<type_list<int, char>::insert<1, float>,  //
                          type_list<int, float, char>>);
static_assert(meta::equal<type_list<int, char>::insert<2, float>,  //
                          type_list<int, char, float>>);

static_assert(meta::equal<type_list<int, char, float>::insert<0, double>,  //
                          type_list<double, int, char, float>>);
static_assert(meta::equal<type_list<int, char, float>::insert<1, double>,  //
                          type_list<int, double, char, float>>);
static_assert(meta::equal<type_list<int, char, float>::insert<2, double>,  //
                          type_list<int, char, double, float>>);
static_assert(meta::equal<type_list<int, char, float>::insert<3, double>,  //
                          type_list<int, char, float, double>>);

static_assert(meta::equal<type_list<int>::remove<0>, type_list<>>);

static_assert(meta::equal<type_list<int, char>::remove<0>, type_list<char>>);
static_assert(meta::equal<type_list<int, char>::remove<1>, type_list<int>>);

static_assert(meta::equal<type_list<int, char, float>::remove<0>,  //
                          type_list<char, float>>);
static_assert(meta::equal<type_list<int, char, float>::remove<1>,  //
                          type_list<int, float>>);
static_assert(meta::equal<type_list<int, char, float>::remove<2>,  //
                          type_list<int, char>>);

static_assert(meta::equal<type_list<int, char, float, double>::remove<0>,  //
                          type_list<char, float, double>>);
static_assert(meta::equal<type_list<int, char, float, double>::remove<1>,  //
                          type_list<int, float, double>>);
static_assert(meta::equal<type_list<int, char, float, double>::remove<2>,  //
                          type_list<int, char, double>>);
static_assert(meta::equal<type_list<int, char, float, double>::remove<3>,  //
                          type_list<int, char, float>>);

template <typename T, typename U>
struct inc_size {
  static constexpr bool value = (sizeof(T) <= sizeof(U));
};
template <typename T, typename U>
struct dec_size {
  static constexpr bool value = (sizeof(T) >= sizeof(U));
};

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

static_assert(meta::equal<                              //
              type_list<>::sort_insert<int, inc_size>,  //
              type_list<int>>);

static_assert(meta::equal<                                  //
              type_list<char>::sort_insert<int, inc_size>,  //
              type_list<char, int>>);
static_assert(meta::equal<                                    //
              type_list<double>::sort_insert<int, inc_size>,  //
              type_list<int, double>>);
static_assert(meta::equal<                                          //
              type_list<unsigned int>::sort_insert<int, inc_size>,  //
              type_list<int, unsigned int>>);

static_assert(meta::equal<                                          //
              type_list<char, double>::sort_insert<int, inc_size>,  //
              type_list<char, int, double>>);
static_assert(meta::equal<                                                 //
              type_list<char, short, int>::sort_insert<double, inc_size>,  //
              type_list<char, short, int, double>>);
