#include <doctest/doctest.h>
//
#include <lyrahgames/xstd/static.hpp>
#include <lyrahgames/xstd/value_list.hpp>

using namespace lyrahgames::xstd;
using namespace meta::value_list;

static_assert(is_value_list<value_list<>>);
static_assert(is_value_list<value_list<'a'>>);
static_assert(is_value_list<value_list<nullptr, 'a'>>);
static_assert(is_value_list<value_list<nullptr, 'a', -10>>);
static_assert(!is_value_list<int>);
static_assert(!is_value_list<float>);

// Check for emptiness
//
// Member Type Function
static_assert(value_list<>::empty);
static_assert(!value_list<1>::empty);
static_assert(!value_list<'c', 1>::empty);
static_assert(!value_list<'c', 1, nullptr>::empty);
// Non-Member Type Function
static_assert(empty<value_list<>>);
static_assert(!empty<value_list<1>>);
static_assert(!empty<value_list<1, 'c'>>);
static_assert(!empty<value_list<'c', 1, nullptr>>);

// Get the number of values
//
// Member Type Function
static_assert(value_list<>::size == 0);
static_assert(value_list<1>::size == 1);
static_assert(value_list<'c'>::size == 1);
static_assert(value_list<1, 'c'>::size == 2);
static_assert(value_list<nullptr, 1, 'c'>::size == 3);
// Non-Member Type Function
static_assert(size<value_list<>> == 0);
static_assert(size<value_list<1>> == 1);
static_assert(size<value_list<'c'>> == 1);
static_assert(size<value_list<1, 'c'>> == 2);
static_assert(size<value_list<1, 'c', nullptr>> == 3);

// Get a list of types of all the values.
//
// Member Type Function
static_assert(meta::equal<value_list<>::types, type_list<>>);
static_assert(meta::equal<value_list<1>::types, type_list<int>>);
static_assert(meta::equal<value_list<'c'>::types, type_list<char>>);
static_assert(meta::equal<value_list<1, 'c'>::types, type_list<int, char>>);
static_assert(meta::equal<value_list<nullptr, 1, 'c'>::types,
                          type_list<std::nullptr_t, int, char>>);
// Non-Member Type Function
static_assert(meta::equal<types<value_list<>>, type_list<>>);
static_assert(meta::equal<types<value_list<1>>, type_list<int>>);
static_assert(meta::equal<types<value_list<'c'>>, type_list<char>>);
static_assert(meta::equal<types<value_list<1, 'c'>>, type_list<int, char>>);
static_assert(meta::equal<types<value_list<1, 'c', nullptr>>,
                          type_list<int, char, std::nullptr_t>>);

// Check if value list contains a specific value.
//
// Member Type Function
static_assert(!value_list<>::contains<1>);
static_assert(!value_list<>::contains<'c'>);
static_assert(value_list<1>::contains<1>);
static_assert(!value_list<1>::contains<'c'>);
static_assert(value_list<1, 'c'>::contains<'c'>);
static_assert(value_list<1, 'c'>::contains<1>);
static_assert(!value_list<1, 'c'>::contains<true>);
// Non-Member Type Function
static_assert(!contains<value_list<>, 1>);
static_assert(!contains<value_list<>, 'c'>);
static_assert(contains<value_list<1>, 1>);
static_assert(!contains<value_list<1>, 'c'>);
static_assert(contains<value_list<1, 'c'>, 'c'>);
static_assert(contains<value_list<1, 'c'>, 1>);
static_assert(!contains<value_list<1, 'c'>, true>);

// Check whether the given value list is a set
//
// Member Type Function
static_assert(value_list<>::is_set);
static_assert(value_list<-1>::is_set);
static_assert(value_list<'c'>::is_set);
//
static_assert(value_list<-1, 'c'>::is_set);
static_assert(value_list<'c', -1>::is_set);
static_assert(!value_list<'c', 'c'>::is_set);
static_assert(!value_list<-1, -1>::is_set);

// Accessing types by index
//
// Member Type Function
static_assert(meta::strict_equal<value_list<1, 'c'>::element<0>, 1>);
static_assert(meta::strict_equal<value_list<1, 'c'>::element<1>, 'c'>);
static_assert(
    meta::strict_equal<value_list<nullptr, 1, 'c'>::element<0>, nullptr>);
static_assert(meta::strict_equal<value_list<nullptr, 1, 'c'>::element<1>, 1>);
static_assert(meta::strict_equal<value_list<nullptr, 1, 'c'>::element<2>, 'c'>);
// Non-Member Type Function
static_assert(meta::strict_equal<element<value_list<1>, 0>, 1>);
static_assert(meta::strict_equal<element<value_list<1, 'c'>, 0>, 1>);
static_assert(meta::strict_equal<element<value_list<1, 'c'>, 1>, 'c'>);
static_assert(
    meta::strict_equal<element<value_list<nullptr, 1, 'c'>, 0>, nullptr>);
static_assert(meta::strict_equal<element<value_list<nullptr, 1, 'c'>, 1>, 1>);
static_assert(meta::strict_equal<element<value_list<nullptr, 1, 'c'>, 2>, 'c'>);

// Get the index of a contained value
//
// Member Type Function
static_assert(value_list<1>::index<1> == 0);
static_assert(value_list<'c', 1>::index<'c'> == 0);
static_assert(value_list<'c', 1>::index<1> == 1);
static_assert(value_list<nullptr, 'c', 1>::index<nullptr> == 0);
static_assert(value_list<nullptr, 'c', 1>::index<'c'> == 1);
static_assert(value_list<nullptr, 'c', 1>::index<1> == 2);
// Non-Member Type Function
static_assert(index<value_list<1>, 1> == 0);
static_assert(index<value_list<'c', 1>, 'c'> == 0);
static_assert(index<value_list<'c', 1>, 1> == 1);
static_assert(index<value_list<nullptr, 'c', 1>, nullptr> == 0);
static_assert(index<value_list<nullptr, 'c', 1>, 'c'> == 1);
static_assert(index<value_list<nullptr, 'c', 1>, 1> == 2);

// Accessing the front
//
// Member Type Function
static_assert(meta::strict_equal<value_list<-1>::front, -1>);
static_assert(meta::strict_equal<value_list<-1, 'c'>::front, -1>);
static_assert(meta::strict_equal<value_list<nullptr, -1, 'c'>::front, nullptr>);
// Non-Member Type Function
static_assert(meta::strict_equal<front<value_list<-1>>, -1>);
static_assert(meta::strict_equal<front<value_list<'c', -1>>, 'c'>);
static_assert(meta::strict_equal<front<value_list<nullptr, 'c', -1>>, nullptr>);

// Accessing the back
//
// Member Type Function
static_assert(meta::strict_equal<value_list<-1>::back, -1>);
static_assert(meta::strict_equal<value_list<-1, 'c'>::back, 'c'>);
static_assert(meta::strict_equal<value_list<-1, 'c', nullptr>::back, nullptr>);
// Non-Member Type Function
static_assert(meta::strict_equal<back<value_list<-1>>, -1>);
static_assert(meta::strict_equal<back<value_list<'c', -1>>, -1>);
static_assert(meta::strict_equal<back<value_list<nullptr, 'c', -1>>, -1>);

// Push back values
//
// Member Type Functions
static_assert(meta::equal<value_list<>::push_back<-1>, value_list<-1>>);
static_assert(meta::equal<value_list<-1>::push_back<'c'>,  //
                          value_list<-1, 'c'>>);
// Non-Member Type Function
static_assert(meta::equal<push_back<value_list<>, -1>,  //
                          value_list<-1>>);
static_assert(meta::equal<push_back<value_list<-1>, 'c'>,  //
                          value_list<-1, 'c'>>);

// Push values to the front
//
// Member Type Functions
static_assert(meta::equal<value_list<>::push_front<-1>, value_list<-1>>);
static_assert(meta::equal<value_list<-1>::push_front<'c'>,  //
                          value_list<'c', -1>>);
// Non-Member Type Functions
static_assert(meta::equal<push_front<value_list<>, -1>, value_list<-1>>);
static_assert(meta::equal<push_front<value_list<-1>, 'c'>,  //
                          value_list<'c', -1>>);

// Reverse the order of values inside a value list.
//
// Member Type Function
static_assert(meta::equal<value_list<>::reverse, value_list<>>);
static_assert(meta::equal<value_list<-1>::reverse, value_list<-1>>);
static_assert(meta::equal<value_list<-1, 'c'>::reverse, value_list<'c', -1>>);
static_assert(
    meta::equal<value_list<true, -1, 'c'>::reverse, value_list<'c', -1, true>>);
// Non-Member Type Function
static_assert(meta::equal<reverse<value_list<>>, value_list<>>);
static_assert(meta::equal<reverse<value_list<-1>>, value_list<-1>>);
static_assert(meta::equal<reverse<value_list<-1, 'c'>>, value_list<'c', -1>>);
static_assert(
    meta::equal<reverse<value_list<true, -1, 'c'>>, value_list<'c', -1, true>>);

// Pop elements from the front and back
//
static_assert(meta::equal<value_list<-1>::pop_back, value_list<>>);
static_assert(meta::equal<value_list<-1>::pop_front, value_list<>>);
//
static_assert(meta::equal<value_list<-1, 'c'>::pop_back, value_list<-1>>);
static_assert(meta::equal<value_list<-1, 'c'>::pop_front, value_list<'c'>>);

// Trim the front
static_assert(meta::equal<value_list<true, -1, 'c'>::trim_front<0>,
                          value_list<true, -1, 'c'>>);
static_assert(
    meta::equal<value_list<true, -1, 'c'>::trim_front<1>, value_list<-1, 'c'>>);
static_assert(meta::equal<value_list<true, -1, 'c'>::trim_front<2>,  //
                          value_list<'c'>>);
static_assert(meta::equal<value_list<true, -1, 'c'>::trim_front<3>,  //
                          value_list<>>);
// Trim the back
static_assert(meta::equal<value_list<true, -1, 'c'>::trim_back<0>,
                          value_list<true, -1, 'c'>>);
static_assert(
    meta::equal<value_list<true, -1, 'c'>::trim_back<1>, value_list<true, -1>>);
static_assert(meta::equal<value_list<true, -1, 'c'>::trim_back<2>,  //
                          value_list<true>>);
static_assert(meta::equal<value_list<true, -1, 'c'>::trim_back<3>,  //
                          value_list<>>);

// Get the sub value list of value list by providing range indices.
//
// Member Type Function
static_assert(meta::equal<                                      //
              value_list<true, nullptr, -1, 'c'>::range<0, 0>,  //
              value_list<>>);
static_assert(meta::equal<                                      //
              value_list<true, nullptr, -1, 'c'>::range<0, 1>,  //
              value_list<true>>);
static_assert(meta::equal<                                      //
              value_list<true, nullptr, -1, 'c'>::range<0, 2>,  //
              value_list<true, nullptr>>);
static_assert(meta::equal<                                      //
              value_list<true, nullptr, -1, 'c'>::range<0, 3>,  //
              value_list<true, nullptr, -1>>);
static_assert(meta::equal<                                      //
              value_list<true, nullptr, -1, 'c'>::range<0, 4>,  //
              value_list<true, nullptr, -1, 'c'>>);
static_assert(meta::equal<                                      //
              value_list<true, nullptr, -1, 'c'>::range<1, 4>,  //
              value_list<nullptr, -1, 'c'>>);
static_assert(meta::equal<                                      //
              value_list<true, nullptr, -1, 'c'>::range<2, 4>,  //
              value_list<-1, 'c'>>);
static_assert(meta::equal<                                      //
              value_list<true, nullptr, -1, 'c'>::range<3, 4>,  //
              value_list<'c'>>);
static_assert(meta::equal<                                      //
              value_list<true, nullptr, -1, 'c'>::range<4, 4>,  //
              value_list<>>);
static_assert(meta::equal<                                      //
              value_list<true, nullptr, -1, 'c'>::range<1, 3>,  //
              value_list<nullptr, -1>>);
// Non-Member Type Function
static_assert(meta::equal<                                      //
              range<value_list<true, nullptr, -1, 'c'>, 0, 0>,  //
              value_list<>>);
static_assert(meta::equal<                                      //
              range<value_list<true, nullptr, -1, 'c'>, 0, 1>,  //
              value_list<true>>);
static_assert(meta::equal<                                      //
              range<value_list<true, nullptr, -1, 'c'>, 0, 2>,  //
              value_list<true, nullptr>>);
static_assert(meta::equal<                                      //
              range<value_list<true, nullptr, -1, 'c'>, 0, 3>,  //
              value_list<true, nullptr, -1>>);
static_assert(meta::equal<                                      //
              range<value_list<true, nullptr, -1, 'c'>, 0, 4>,  //
              value_list<true, nullptr, -1, 'c'>>);
static_assert(meta::equal<                                      //
              range<value_list<true, nullptr, -1, 'c'>, 1, 4>,  //
              value_list<nullptr, -1, 'c'>>);
static_assert(meta::equal<                                      //
              range<value_list<true, nullptr, -1, 'c'>, 2, 4>,  //
              value_list<-1, 'c'>>);
static_assert(meta::equal<                                      //
              range<value_list<true, nullptr, -1, 'c'>, 3, 4>,  //
              value_list<'c'>>);
static_assert(meta::equal<                                      //
              range<value_list<true, nullptr, -1, 'c'>, 4, 4>,  //
              value_list<>>);
static_assert(meta::equal<                                      //
              range<value_list<true, nullptr, -1, 'c'>, 1, 3>,  //
              value_list<nullptr, -1>>);

// Swap the order of two values given their indices.
//
// Member Type Function
static_assert(meta::equal<value_list<true, nullptr, -1, 'c'>::swap<0, 0>,  //
                          value_list<true, nullptr, -1, 'c'>>);
static_assert(meta::equal<value_list<true, nullptr, -1, 'c'>::swap<0, 1>,  //
                          value_list<nullptr, true, -1, 'c'>>);
static_assert(meta::equal<value_list<true, nullptr, -1, 'c'>::swap<0, 2>,  //
                          value_list<-1, nullptr, true, 'c'>>);
static_assert(meta::equal<value_list<true, nullptr, -1, 'c'>::swap<0, 3>,  //
                          value_list<'c', nullptr, -1, true>>);
//
static_assert(meta::equal<value_list<true, nullptr, -1, 'c'>::swap<1, 0>,  //
                          value_list<nullptr, true, -1, 'c'>>);
static_assert(meta::equal<value_list<true, nullptr, -1, 'c'>::swap<1, 1>,  //
                          value_list<true, nullptr, -1, 'c'>>);
static_assert(meta::equal<value_list<true, nullptr, -1, 'c'>::swap<1, 2>,  //
                          value_list<true, -1, nullptr, 'c'>>);
static_assert(meta::equal<value_list<true, nullptr, -1, 'c'>::swap<1, 3>,  //
                          value_list<true, 'c', -1, nullptr>>);
//
static_assert(meta::equal<value_list<true, nullptr, -1, 'c'>::swap<2, 0>,  //
                          value_list<-1, nullptr, true, 'c'>>);
static_assert(meta::equal<value_list<true, nullptr, -1, 'c'>::swap<2, 1>,  //
                          value_list<true, -1, nullptr, 'c'>>);
static_assert(meta::equal<value_list<true, nullptr, -1, 'c'>::swap<2, 2>,  //
                          value_list<true, nullptr, -1, 'c'>>);
static_assert(meta::equal<value_list<true, nullptr, -1, 'c'>::swap<2, 3>,  //
                          value_list<true, nullptr, 'c', -1>>);
//
static_assert(meta::equal<value_list<true, nullptr, -1, 'c'>::swap<3, 0>,  //
                          value_list<'c', nullptr, -1, true>>);
static_assert(meta::equal<value_list<true, nullptr, -1, 'c'>::swap<3, 1>,  //
                          value_list<true, 'c', -1, nullptr>>);
static_assert(meta::equal<value_list<true, nullptr, -1, 'c'>::swap<3, 2>,  //
                          value_list<true, nullptr, 'c', -1>>);
static_assert(meta::equal<value_list<true, nullptr, -1, 'c'>::swap<3, 3>,  //
                          value_list<true, nullptr, -1, 'c'>>);
//
// Non-Member Type Function
static_assert(meta::equal<swap<value_list<true, nullptr, -1, 'c'>, 0, 0>,  //
                          value_list<true, nullptr, -1, 'c'>>);
static_assert(meta::equal<swap<value_list<true, nullptr, -1, 'c'>, 0, 1>,  //
                          value_list<nullptr, true, -1, 'c'>>);
static_assert(meta::equal<swap<value_list<true, nullptr, -1, 'c'>, 0, 2>,  //
                          value_list<-1, nullptr, true, 'c'>>);
static_assert(meta::equal<swap<value_list<true, nullptr, -1, 'c'>, 0, 3>,  //
                          value_list<'c', nullptr, -1, true>>);
//
static_assert(meta::equal<swap<value_list<true, nullptr, -1, 'c'>, 1, 0>,  //
                          value_list<nullptr, true, -1, 'c'>>);
static_assert(meta::equal<swap<value_list<true, nullptr, -1, 'c'>, 1, 1>,  //
                          value_list<true, nullptr, -1, 'c'>>);
static_assert(meta::equal<swap<value_list<true, nullptr, -1, 'c'>, 1, 2>,  //
                          value_list<true, -1, nullptr, 'c'>>);
static_assert(meta::equal<swap<value_list<true, nullptr, -1, 'c'>, 1, 3>,  //
                          value_list<true, 'c', -1, nullptr>>);
//
static_assert(meta::equal<swap<value_list<true, nullptr, -1, 'c'>, 2, 0>,  //
                          value_list<-1, nullptr, true, 'c'>>);
static_assert(meta::equal<swap<value_list<true, nullptr, -1, 'c'>, 2, 1>,  //
                          value_list<true, -1, nullptr, 'c'>>);
static_assert(meta::equal<swap<value_list<true, nullptr, -1, 'c'>, 2, 2>,  //
                          value_list<true, nullptr, -1, 'c'>>);
static_assert(meta::equal<swap<value_list<true, nullptr, -1, 'c'>, 2, 3>,  //
                          value_list<true, nullptr, 'c', -1>>);
//
static_assert(meta::equal<swap<value_list<true, nullptr, -1, 'c'>, 3, 0>,  //
                          value_list<'c', nullptr, -1, true>>);
static_assert(meta::equal<swap<value_list<true, nullptr, -1, 'c'>, 3, 1>,  //
                          value_list<true, 'c', -1, nullptr>>);
static_assert(meta::equal<swap<value_list<true, nullptr, -1, 'c'>, 3, 2>,  //
                          value_list<true, nullptr, 'c', -1>>);
static_assert(meta::equal<swap<value_list<true, nullptr, -1, 'c'>, 3, 3>,  //
                          value_list<true, nullptr, -1, 'c'>>);

// Insert values into the value list
//
// Member Type Function
static_assert(meta::equal<value_list<>::insert<0, -1>, value_list<-1>>);
//
static_assert(meta::equal<value_list<'c'>::insert<0, -1>,  //
                          value_list<-1, 'c'>>);
static_assert(meta::equal<value_list<'c'>::insert<1, -1>,  //
                          value_list<'c', -1>>);
//
static_assert(meta::equal<value_list<-1, 'c'>::insert<0, true>,  //
                          value_list<true, -1, 'c'>>);
static_assert(meta::equal<value_list<-1, 'c'>::insert<1, true>,  //
                          value_list<-1, true, 'c'>>);
static_assert(meta::equal<value_list<-1, 'c'>::insert<2, true>,  //
                          value_list<-1, 'c', true>>);
//
static_assert(meta::equal<value_list<-1, 'c', true>::insert<0, nullptr>,  //
                          value_list<nullptr, -1, 'c', true>>);
static_assert(meta::equal<value_list<-1, 'c', true>::insert<1, nullptr>,  //
                          value_list<-1, nullptr, 'c', true>>);
static_assert(meta::equal<value_list<-1, 'c', true>::insert<2, nullptr>,  //
                          value_list<-1, 'c', nullptr, true>>);
static_assert(meta::equal<value_list<-1, 'c', true>::insert<3, nullptr>,  //
                          value_list<-1, 'c', true, nullptr>>);
//
// Non-Member Type Function
static_assert(meta::equal<insert<value_list<>, 0, -1>, value_list<-1>>);
//
static_assert(meta::equal<insert<value_list<'c'>, 0, -1>,  //
                          value_list<-1, 'c'>>);
static_assert(meta::equal<insert<value_list<'c'>, 1, -1>,  //
                          value_list<'c', -1>>);
//
static_assert(meta::equal<insert<value_list<-1, 'c'>, 0, true>,  //
                          value_list<true, -1, 'c'>>);
static_assert(meta::equal<insert<value_list<-1, 'c'>, 1, true>,  //
                          value_list<-1, true, 'c'>>);
static_assert(meta::equal<insert<value_list<-1, 'c'>, 2, true>,  //
                          value_list<-1, 'c', true>>);
//
static_assert(meta::equal<insert<value_list<-1, 'c', true>, 0, nullptr>,  //
                          value_list<nullptr, -1, 'c', true>>);
static_assert(meta::equal<insert<value_list<-1, 'c', true>, 1, nullptr>,  //
                          value_list<-1, nullptr, 'c', true>>);
static_assert(meta::equal<insert<value_list<-1, 'c', true>, 2, nullptr>,  //
                          value_list<-1, 'c', nullptr, true>>);
static_assert(meta::equal<insert<value_list<-1, 'c', true>, 3, nullptr>,  //
                          value_list<-1, 'c', true, nullptr>>);

// Remove a value inside the value list given its index
//
// Member Type Function
static_assert(meta::equal<value_list<-1>::remove<0>, value_list<>>);
//
static_assert(meta::equal<value_list<-1, 'c'>::remove<0>, value_list<'c'>>);
static_assert(meta::equal<value_list<-1, 'c'>::remove<1>, value_list<-1>>);
//
static_assert(meta::equal<value_list<-1, 'c', true>::remove<0>,  //
                          value_list<'c', true>>);
static_assert(meta::equal<value_list<-1, 'c', true>::remove<1>,  //
                          value_list<-1, true>>);
static_assert(meta::equal<value_list<-1, 'c', true>::remove<2>,  //
                          value_list<-1, 'c'>>);
//
static_assert(meta::equal<value_list<-1, 'c', true, nullptr>::remove<0>,  //
                          value_list<'c', true, nullptr>>);
static_assert(meta::equal<value_list<-1, 'c', true, nullptr>::remove<1>,  //
                          value_list<-1, true, nullptr>>);
static_assert(meta::equal<value_list<-1, 'c', true, nullptr>::remove<2>,  //
                          value_list<-1, 'c', nullptr>>);
static_assert(meta::equal<value_list<-1, 'c', true, nullptr>::remove<3>,  //
                          value_list<-1, 'c', true>>);
//
// Non-Member Type Function
static_assert(meta::equal<remove<value_list<-1>, 0>, value_list<>>);
//
static_assert(meta::equal<remove<value_list<-1, 'c'>, 0>, value_list<'c'>>);
static_assert(meta::equal<remove<value_list<-1, 'c'>, 1>, value_list<-1>>);
//
static_assert(meta::equal<remove<value_list<-1, 'c', true>, 0>,  //
                          value_list<'c', true>>);
static_assert(meta::equal<remove<value_list<-1, 'c', true>, 1>,  //
                          value_list<-1, true>>);
static_assert(meta::equal<remove<value_list<-1, 'c', true>, 2>,  //
                          value_list<-1, 'c'>>);
//
static_assert(meta::equal<remove<value_list<-1, 'c', true, nullptr>, 0>,  //
                          value_list<'c', true, nullptr>>);
static_assert(meta::equal<remove<value_list<-1, 'c', true, nullptr>, 1>,  //
                          value_list<-1, true, nullptr>>);
static_assert(meta::equal<remove<value_list<-1, 'c', true, nullptr>, 2>,  //
                          value_list<-1, 'c', nullptr>>);
static_assert(meta::equal<remove<value_list<-1, 'c', true, nullptr>, 3>,  //
                          value_list<-1, 'c', true>>);

// Remove a value inside the value list given its index
//
// Member Type Function
static_assert(meta::equal<value_list<-1>::remove_value<-1>, value_list<>>);
//
static_assert(
    meta::equal<value_list<-1, 'c'>::remove_value<-1>, value_list<'c'>>);
static_assert(
    meta::equal<value_list<-1, 'c'>::remove_value<'c'>, value_list<-1>>);
//
static_assert(meta::equal<value_list<-1, 'c', true>::remove_value<-1>,  //
                          value_list<'c', true>>);
static_assert(meta::equal<value_list<-1, 'c', true>::remove_value<'c'>,  //
                          value_list<-1, true>>);
static_assert(meta::equal<value_list<-1, 'c', true>::remove_value<true>,  //
                          value_list<-1, 'c'>>);
//
static_assert(
    meta::equal<value_list<-1, 'c', true, nullptr>::remove_value<-1>,  //
                value_list<'c', true, nullptr>>);
static_assert(
    meta::equal<value_list<-1, 'c', true, nullptr>::remove_value<'c'>,  //
                value_list<-1, true, nullptr>>);
static_assert(
    meta::equal<value_list<-1, 'c', true, nullptr>::remove_value<true>,  //
                value_list<-1, 'c', nullptr>>);
static_assert(
    meta::equal<value_list<-1, 'c', true, nullptr>::remove_value<nullptr>,  //
                value_list<-1, 'c', true>>);
//
// Non-Member Type Function
static_assert(meta::equal<remove_value<value_list<-1>, -1>, value_list<>>);
//
static_assert(
    meta::equal<remove_value<value_list<-1, 'c'>, -1>, value_list<'c'>>);
static_assert(
    meta::equal<remove_value<value_list<-1, 'c'>, 'c'>, value_list<-1>>);
//
static_assert(meta::equal<remove_value<value_list<-1, 'c', true>, -1>,  //
                          value_list<'c', true>>);
static_assert(meta::equal<remove_value<value_list<-1, 'c', true>, 'c'>,  //
                          value_list<-1, true>>);
static_assert(meta::equal<remove_value<value_list<-1, 'c', true>, true>,  //
                          value_list<-1, 'c'>>);
//
static_assert(
    meta::equal<remove_value<value_list<-1, 'c', true, nullptr>, -1>,  //
                value_list<'c', true, nullptr>>);
static_assert(
    meta::equal<remove_value<value_list<-1, 'c', true, nullptr>, 'c'>,  //
                value_list<-1, true, nullptr>>);
static_assert(
    meta::equal<remove_value<value_list<-1, 'c', true, nullptr>, true>,  //
                value_list<-1, 'c', nullptr>>);
static_assert(
    meta::equal<remove_value<value_list<-1, 'c', true, nullptr>, nullptr>,  //
                value_list<-1, 'c', true>>);

// Try to remove a value from the value list
//
// Member Type Function
static_assert(meta::equal<value_list<>::try_remove_value<-1>, value_list<>>);
static_assert(meta::equal<value_list<>::try_remove_value<'c'>, value_list<>>);
//
static_assert(meta::equal<value_list<-1>::try_remove_value<-1>, value_list<>>);
static_assert(
    meta::equal<value_list<-1>::try_remove_value<'c'>, value_list<-1>>);
//
// Non-Member Type Function
static_assert(meta::equal<try_remove_value<value_list<>, -1>, value_list<>>);
static_assert(meta::equal<try_remove_value<value_list<>, 'c'>, value_list<>>);
//
static_assert(meta::equal<try_remove_value<value_list<-1>, -1>, value_list<>>);
static_assert(
    meta::equal<try_remove_value<value_list<-1>, 'c'>, value_list<-1>>);

// Assign values by their index
//
// Member Type Function
static_assert(meta::equal<value_list<-1>::assignment<0, 'c'>, value_list<'c'>>);
//
static_assert(meta::equal<value_list<-1, 'c'>::assignment<0, true>,
                          value_list<true, 'c'>>);
static_assert(meta::equal<value_list<-1, 'c'>::assignment<1, true>,
                          value_list<-1, true>>);
//
static_assert(meta::equal<value_list<-1, 'c', true>::assignment<0, nullptr>,
                          value_list<nullptr, 'c', true>>);
static_assert(meta::equal<value_list<-1, 'c', true>::assignment<1, nullptr>,
                          value_list<-1, nullptr, true>>);
static_assert(meta::equal<value_list<-1, 'c', true>::assignment<2, nullptr>,
                          value_list<-1, 'c', nullptr>>);
//
// Non-Member Type Function
static_assert(meta::equal<assignment<value_list<-1>, 0, 'c'>, value_list<'c'>>);
//
static_assert(meta::equal<assignment<value_list<-1, 'c'>, 0, true>,
                          value_list<true, 'c'>>);
static_assert(meta::equal<assignment<value_list<-1, 'c'>, 1, true>,
                          value_list<-1, true>>);
//
static_assert(meta::equal<assignment<value_list<-1, 'c', true>, 0, nullptr>,
                          value_list<nullptr, 'c', true>>);
static_assert(meta::equal<assignment<value_list<-1, 'c', true>, 1, nullptr>,
                          value_list<-1, nullptr, true>>);
static_assert(meta::equal<assignment<value_list<-1, 'c', true>, 2, nullptr>,
                          value_list<-1, 'c', nullptr>>);

// Sort
// using namespace meta::type_list;
using package = type_list<int32_t, char, int64_t, float>;
using indices = meta::value_list::iota<package::size>;
constexpr auto alignment_less = []<auto x, auto y> {
  return alignof(typename package::template element<x>) >=
         alignof(typename package::template element<y>);
};
using sigma = sort<indices, alignment_less>;
constexpr auto inv_less = []<auto x, auto y> {
  return sigma::template element<x> <= sigma::template element<y>;
};
using inverse_sigma = sort<indices, inv_less>;
using tight_package = meta::type_list::permutation<package, sigma>;
static_assert(meta::equal<    //
              tight_package,  //
              type_list<int64_t, int32_t, float, char>>);

static_assert(
    meta::equal<
        meta::type_list::element<tight_package,
                                 meta::value_list::element<inverse_sigma, 0>>,
        meta::type_list::element<package, 0>>);
