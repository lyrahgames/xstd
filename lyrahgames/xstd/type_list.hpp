#pragma once
#include <cstddef>
#include <type_traits>
//
#include <lyrahgames/xstd/meta.hpp>

namespace lyrahgames::xstd {

// namespace detail {

// // Access to elements of pack extension by using indices.
// template <size_t index, typename... types>
// struct element;
// template <size_t index>
// struct element<index> {};
// template <typename t, typename... types>
// struct element<0, t, types...> {
//   using type = t;
// };
// template <size_t index, typename t, typename... types>
// struct element<index, t, types...> {
//   using type = typename element<index - 1, types...>::type;
// };

// }  // namespace detail

namespace detail {
template <typename T, size_t count>
struct trim_front {
  using type = typename trim_front<typename T::pop_front, count - 1>::type;
};
template <typename T>
struct trim_front<T, 0> {
  using type = T;
};

template <typename T, size_t count>
struct trim_back {
  using type = typename trim_back<typename T::pop_back, count - 1>::type;
};
template <typename T>
struct trim_back<T, 0> {
  using type = T;
};
}  // namespace detail

template <typename... types>
struct type_list;

namespace detail {
template <typename t, typename u>
struct concatenate;
template <typename... t, typename... u>
struct concatenate<type_list<t...>, type_list<u...>> {
  using type = type_list<t..., u...>;
};

template <typename T, size_t i, size_t j>
struct swap {
  using type = typename T::template range<0, i>::
      template push_back<typename T::template element<j>>::template concat<
          typename T::template range<i + 1, j>>::
          template push_back<typename T::template element<i>>::template concat<
              typename T::template range<j + 1, T::size>>;
};
template <typename T, size_t i, size_t j>
requires(j < i)  //
    struct swap<T, i, j> {
  using type = typename swap<T, j, i>::type;
};
template <typename T, size_t i>
struct swap<T, i, i> {
  using type = T;
};

template <typename T, size_t index, typename U>
struct insert {
  using type = typename T::template range<0, index>::template push_back<
      U>::template concat<typename T::template range<index, T::size>>;
};
// template <typename T, size_t index, typename U>
// requires(index == 0)  //
//     struct insert<T, index, U> {
//   using type = typename T::template push_front<U>;
// };
// template <typename T, size_t index, typename U>
// requires(index == T::size)  //
//     struct insert<T, index, U> {
//   using type = typename T::template push_back<U>;
// };

template <typename T, size_t index>
struct remove {
  using type = typename T::template range<0, index>::template concat<
      typename T::template range<index + 1, T::size>>;
};

template <typename T, typename U, template <typename, typename> typename less>
struct merge;
template <template <typename, typename> typename less, typename... T>
struct merge<type_list<T...>, type_list<>, less> {
  using type = type_list<T...>;
};
template <template <typename, typename> typename less, typename... T>
struct merge<type_list<>, type_list<T...>, less> {
  using type = type_list<T...>;
};
template <template <typename, typename> typename less,
          typename T,
          typename U,
          typename... t,
          typename... u>
requires less<T, U>::value  //
    struct merge<type_list<T, t...>, type_list<U, u...>, less> {
  using type = typename merge<type_list<t...>, type_list<U, u...>, less>::type::
      template push_front<T>;
};
template <template <typename, typename> typename less,
          typename T,
          typename U,
          typename... t,
          typename... u>
requires(!less<T, U>::value)  //
    struct merge<type_list<T, t...>, type_list<U, u...>, less> {
  using type = typename merge<type_list<T, t...>, type_list<u...>, less>::type::
      template push_front<U>;
};

template <typename T, template <typename, typename> typename less>
struct sort {
  using type = typename merge<
      typename sort<typename T::template range<0, T::size / 2>, less>::type,
      typename sort<typename T::template range<T::size / 2, T::size>,
                    less>::type,
      less>::type;
};
template <template <typename, typename> typename less, typename T>
struct sort<type_list<T>, less> {
  using type = type_list<T>;
};

template <typename T, typename U, template <typename, typename> typename less>
struct sort_insert;
template <typename T, typename U, template <typename, typename> typename less>
requires T::empty  //
    struct sort_insert<T, U, less> {
  using type = typename T::template push_back<U>;
};
template <typename T, typename U, template <typename, typename> typename less>
requires less<U, typename T::front>::value  //
    struct sort_insert<T, U, less> {
  using type = typename T::template push_front<U>;
};
template <typename T, typename U, template <typename, typename> typename less>
requires(!less<U, typename T::front>::value)  //
    struct sort_insert<T, U, less> {
  using type = typename sort_insert<typename T::pop_front, U, less>::type::
      template push_front<typename T::front>;
};

}  // namespace detail

template <typename T, typename... types>
struct type_list<T, types...> {
  static constexpr size_t size = 1 + sizeof...(types);
  static constexpr bool empty = false;

  template <typename U>
  static constexpr bool contains = meta::equal<U, T> ||
                                   (meta::equal<U, types> || ...);

  template <size_t index>
  // requires(index <= sizeof...(types))  //
  using element =
      typename meta::detail::variadic_projection<index, T, types...>::type;

  using front = element<0>;
  using back = element<size - 1>;

  using self = type_list<T, types...>;

  template <typename U>
  using push_back = type_list<T, types..., U>;

  template <typename U>
  using push_front = type_list<U, T, types...>;

  using pop_front = type_list<types...>;

  using reverse = typename type_list<types...>::reverse::template push_back<T>;

  using pop_back = typename reverse::pop_front::reverse;

  template <typename U>
  using concat = typename detail::concatenate<self, U>::type;

  template <size_t count>
  using trim_front = typename detail::trim_front<self, count>::type;

  template <size_t count>
  using trim_back = typename detail::trim_back<self, count>::type;

  template <size_t i, size_t j>
  using swap = typename detail::swap<self, i, j>::type;

  template <size_t first, size_t last>
  using range = typename trim_back<size - last>::template trim_front<first>;

  template <size_t index, typename U>
  using insert = typename detail::insert<self, index, U>::type;

  template <size_t index>
  // using remove = typename detail::remove<self, index>::type;
  using remove = typename detail::concatenate<range<0, index>,
                                              range<index + 1, size>>::type;

  template <template <typename, typename> typename less>
  using sort = typename detail::sort<self, less>::type;

  template <typename U, template <typename, typename> typename less>
  using sort_insert = typename detail::sort_insert<self, U, less>::type;
};

template <>
struct type_list<> {
  static constexpr size_t size = 0;
  static constexpr bool empty = true;
  template <typename T>
  static constexpr bool contains = false;
  using self = type_list<>;
  template <typename T>
  using push_back = type_list<T>;
  template <typename T>
  using push_front = type_list<T>;
  using reverse = type_list<>;
  template <typename U>
  using concat = typename detail::concatenate<self, U>::type;

  template <size_t index, typename T>
  requires(index == 0)  //
      using insert = type_list<T>;

  template <size_t count>
  requires(count == 0)  //
      using trim_front = self;

  template <size_t count>
  requires(count == 0)  //
      using trim_back = self;

  template <template <typename, typename> typename>
  using sort = self;

  template <typename U, template <typename, typename> typename less>
  using sort_insert = type_list<U>;
};

namespace detail {
template <typename T>
struct is_type_list : std::false_type {};
template <typename... types>
struct is_type_list<type_list<types...>> : std::true_type {};
}  // namespace detail
template <typename T>
constexpr bool is_type_list = detail::is_type_list<T>::value;

namespace instance {
template <typename T>
concept type_list = is_type_list<T>;
}  // namespace instance

// Non-Member Type Functions
// Need to be provided inside the meta namespace.
namespace meta {

template <instance::type_list list>
struct detail::size<list> {
  static constexpr size_t value = list::size;
};

template <instance::type_list list>
struct detail::empty<list> {
  static constexpr bool value = list::empty;
};

template <instance::type_list list, typename T>
struct detail::contains<list, T> {
  static constexpr bool value = list::template contains<T>;
};

template <instance::type_list list, std::integral auto index>
requires(0 <= index) && (index < list::size)  //
    struct detail::element<list, index> {
  using type = typename list::template element<index>;
};

template <instance::type_list list>
requires(!list::empty)  //
    struct detail::front<list> {
  using type = typename list::front;
};

template <instance::type_list list>
requires(!list::empty)  //
    struct detail::back<list> {
  using type = typename list::back;
};

template <instance::type_list list, typename T>
using push_back = typename list::template push_back<T>;

template <instance::type_list list, typename T>
using push_front = typename list::template push_front<T>;

}  // namespace meta

}  // namespace lyrahgames::xstd
