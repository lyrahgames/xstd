#pragma once
#include <lyrahgames/xstd/type_list/common.hpp>
#include <lyrahgames/xstd/value_list/common.hpp>

namespace lyrahgames::xstd {

/// Forward declarations for type function details.
// This is needed for member type functions.
// Also, for every using declaration providing a non-member type function,
// an implementation given by struct specializations has to be declared.
//
// Every type function implementation is provided as its own template.
namespace detail::type_list {

using xstd::type_list;

//
//
template <instance::type_list list>
struct size;
//
template <typename... types>
struct size<type_list<types...>> {
  static constexpr size_t value = sizeof...(types);
};

//
//
template <instance::type_list list>
struct empty {
  static constexpr bool value = size<list>::value == 0;
};

// Empty lists have no type access.
//
template <instance::type_list list, size_t index>
struct element {};
//
// The first type can be accessed by index zero.
//
template <typename front, typename... tail>
struct element<type_list<front, tail...>, 0> {
  using type = front;
};
//
// For all other indices, we use a recursive call
// to elements with a reduced pack extension and index.
//
template <size_t index, typename front, typename... tail>
struct element<type_list<front, tail...>, index> {
  using type = typename element<type_list<tail...>, index - 1>::type;
};

//
//
template <instance::type_list list, typename type>
struct index {};
//
template <typename t, typename front, typename... tail>
struct index<type_list<front, tail...>, t> {
  static constexpr size_t value = 1 + index<type_list<tail...>, t>::value;
};
//
template <typename t, typename front, typename... tail>
requires(meta::equal<t, front>)  //
    struct index<type_list<front, tail...>, t> {
  static constexpr size_t value = 0;
};

//
//
template <instance::type_list list>
struct front {};
//
template <typename t, typename... types>
struct front<type_list<t, types...>> {
  using type = t;
};

//
//
template <instance::type_list list>
struct back {};
//
template <typename t>
struct back<type_list<t>> {
  using type = t;
};
//
template <typename t, typename... types>
struct back<type_list<t, types...>> {
  using type = typename back<type_list<types...>>::type;
};

//
constexpr auto aligned_offset_padding(size_t offset, size_t alignment) noexcept
    -> size_t {
  return alignment - 1 - ((offset + alignment - 1) % alignment);
}

//
constexpr auto aligned_offset(size_t offset, size_t alignment) noexcept
    -> size_t {
  return offset + aligned_offset_padding(offset, alignment);
}

// For empty structs or tuples, the alignment is 1.
//
template <instance::type_list list>
struct alignment {
  static constexpr size_t value = 1;
};
//
template <typename front, typename... tail>
struct alignment<type_list<front, tail...>> {
  static constexpr size_t value =
      std::max(alignof(front), alignment<type_list<tail...>>::value);
};

//
template <instance::type_list list, size_t index>  //
struct struct_offset {
  static constexpr size_t value =
      aligned_offset(struct_offset<list, index - 1>::value +
                         sizeof(typename element<list, index - 1>::type),
                     alignof(typename element<list, index>::type));
};
template <instance::type_list list>  //
struct struct_offset<list, 0> {
  static constexpr size_t value = 0;
};

//
template <instance::type_list list, size_t index>
struct tuple_offset {
  using type = typename element<list, index - 1>::type;
  static constexpr size_t byte_size = std::is_empty_v<type> ? 0 : sizeof(type);
  static constexpr size_t value =
      aligned_offset(tuple_offset<list, index - 1>::value + byte_size,
                     alignof(typename element<list, index>::type));
  ;
};
template <instance::type_list list>  //
struct tuple_offset<list, 0> {
  static constexpr size_t value = 0;
};

//
template <instance::type_list list>
struct struct_byte_size {
  static constexpr size_t value =
      aligned_offset(struct_offset<list, size<list>::value - 1>::value +
                         sizeof(typename back<list>::type),
                     alignment<list>::value);
};
template <>
struct struct_byte_size<type_list<>> {
  static constexpr size_t value = 1;
};

//
template <instance::type_list list>
struct tuple_byte_size {
  using type = typename back<list>::type;
  static constexpr size_t byte_size = std::is_empty_v<type> ? 0 : sizeof(type);
  static constexpr size_t value =
      std::max(size_t{1},
               aligned_offset(
                   tuple_offset<list, size<list>::value - 1>::value + byte_size,
                   alignment<list>::value));
};
template <>
struct tuple_byte_size<type_list<>> {
  static constexpr size_t value = 1;
};

//
template <instance::type_list list, size_t index>
struct struct_padding {
  static constexpr size_t value = struct_offset<list, index + 1>::value -
                                  struct_offset<list, index>::value -
                                  sizeof(typename element<list, index>::type);
};
template <instance::type_list list, size_t index>
requires(index == list::size - 1)  //
    struct struct_padding<list, index> {
  static constexpr size_t value = struct_byte_size<list>::value -
                                  struct_offset<list, index>::value -
                                  sizeof(typename back<list>::type);
};

//
template <instance::type_list x, instance::type_list y>
struct concatenation {};
template <typename... t, typename... u>
struct concatenation<type_list<t...>, type_list<u...>> {
  using type = type_list<t..., u...>;
};

//
template <instance::type_list list, typename type>
struct push_back {};
template <typename t, typename... types>
struct push_back<type_list<types...>, t> {
  using type = type_list<types..., t>;
};

//
template <instance::type_list list, typename type>
struct push_front {};
template <typename t, typename... types>
struct push_front<type_list<types...>, t> {
  using type = type_list<t, types...>;
};

//
template <instance::type_list list>
struct pop_front {};
template <typename t, typename... types>
struct pop_front<type_list<t, types...>> {
  using type = type_list<types...>;
};

//
template <instance::type_list list>
struct pop_back {};
template <typename t>
struct pop_back<type_list<t>> {
  using type = type_list<>;
};
template <typename t, typename... types>
struct pop_back<type_list<t, types...>> {
  using type = typename push_front<typename pop_back<type_list<types...>>::type,
                                   t>::type;
};

//
template <instance::type_list list>
struct reverse {
  using type = list;
};
template <typename t, typename... types>
struct reverse<type_list<t, types...>> {
  using type =
      typename push_back<typename reverse<type_list<types...>>::type, t>::type;
};

//
template <instance::type_list list, size_t count>
requires(count <= list::size)  //
    struct trim_front {
  using type =
      typename trim_front<typename pop_front<list>::type, count - 1>::type;
};
template <instance::type_list list>
struct trim_front<list, 0> {
  using type = list;
};

//
template <instance::type_list list, size_t count>
requires(count <= list::size)  //
    struct trim_back {
  using type =
      typename trim_back<typename pop_back<list>::type, count - 1>::type;
};
template <instance::type_list list>
struct trim_back<list, 0> {
  using type = list;
};

//
template <instance::type_list list, size_t first, size_t last>
requires(first <= last) && (last <= list::size)  //
    struct range {
  using type =
      typename trim_front<typename trim_back<list, list::size - last>::type,
                          first>::type;
};

template <instance::type_list list, size_t i, size_t j>
struct swap;
template <instance::type_list list, size_t i, size_t j>
requires(i < j) && (j < list::size)  //
    struct swap<list, i, j> {
  using left = typename range<list, 0, i>::type;
  using middle = typename range<list, i + 1, j>::type;
  using right = typename range<list, j + 1, list::size>::type;
  using xi = typename element<list, i>::type;
  using xj = typename element<list, j>::type;
  using new_left = typename push_back<left, xj>::type;
  using new_right = typename push_front<right, xi>::type;
  using tmp = typename concatenation<new_left, middle>::type;
  using type = typename concatenation<tmp, new_right>::type;
};
template <instance::type_list list, size_t i, size_t j>
requires(j < i) && (i < list::size)  //
    struct swap<list, i, j> {
  using type = typename swap<list, j, i>::type;
};
template <instance::type_list list, size_t i>
struct swap<list, i, i> {
  using type = list;
};

//
template <instance::type_list list, size_t index, typename t>
requires(index <= list::size)  //
    struct insert {
  using left = typename range<list, 0, index>::type;
  using right = typename range<list, index, list::size>::type;
  using tmp = typename push_back<left, t>::type;
  using type = typename concatenation<tmp, right>::type;
};

//
template <typename list, size_t index>
requires(index < list::size)  //
    struct remove {
  using left = typename range<list, 0, index>::type;
  using right = typename range<list, index + 1, list::size>::type;
  using type = typename concatenation<left, right>::type;
};

//
template <instance::type_list list, size_t index, typename t>
requires(index < list::size)  //
    struct assignment {
  using left = typename range<list, 0, index>::type;
  using right = typename range<list, index + 1, list::size>::type;
  using new_right = typename push_front<right, t>::type;
  using type = typename concatenation<left, new_right>::type;
};

//
template <instance::type_list left,
          instance::type_list right,
          template <typename, typename>
          typename less>
struct merge;
template <template <typename, typename> typename less>
struct merge<type_list<>, type_list<>, less> {
  using type = type_list<>;
};
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
  using type = typename push_front<
      typename merge<type_list<t...>, type_list<U, u...>, less>::type,
      T>::type;
};
template <template <typename, typename> typename less,
          typename T,
          typename U,
          typename... t,
          typename... u>
requires(!less<T, U>::value)  //
    struct merge<type_list<T, t...>, type_list<U, u...>, less> {
  using type = typename push_front<
      typename merge<type_list<T, t...>, type_list<u...>, less>::type,
      U>::type;
};

//
template <instance::type_list list, template <typename, typename> typename less>
struct sort {
  static constexpr size_t half = list::size / 2;
  using type = typename merge<
      typename sort<typename range<list, 0, half>::type, less>::type,
      typename sort<typename range<list, half, list::size>::type, less>::type,
      less>::type;
};
template <instance::type_list list, template <typename, typename> typename less>
requires(list::size <= 1)  //
    struct sort<list, less> {
  using type = list;
};

//
template <instance::type_list list,
          typename t,
          template <typename, typename>
          typename less>
struct insert_when;
template <typename t, template <typename, typename> typename less>
struct insert_when<type_list<>, t, less> {
  using type = type_list<t>;
};
template <typename t,
          typename front,
          template <typename, typename>
          typename less,
          typename... types>
struct insert_when<type_list<front, types...>, t, less> {
  using type = type_list<t, front, types...>;
};
template <typename t,
          typename front,
          template <typename, typename>
          typename less,
          typename... types>
requires(!less<t, front>::value)  //
    struct insert_when<type_list<front, types...>, t, less> {
  using type = typename push_front<
      typename insert_when<type_list<types...>, t, less>::type,
      front>::type;
};

//
template <instance::type_list list, template <typename> typename f>
struct transformation;
//
template <template <typename> typename f, typename... types>
struct transformation<type_list<types...>, f> {
  using type = type_list<typename f<types>::type...>;
};

//
template <instance::type_list list, instance::index_list indices>
struct permutation;
//
template <typename... types, size_t... indices>
struct permutation<type_list<types...>, xstd::value_list<indices...>> {
  using list = type_list<types...>;
  using type = type_list<typename list::template element<indices>...>;
};

template <instance::type_list list>
constexpr bool for_each_until(auto&& f) {
  if constexpr (list::empty)
    return false;
  else {
    if (f.template operator()<typename front<list>::type>()) return true;
    return for_each_until<typename pop_front<list>::type>(
        std::forward<decltype(f)>(f));
  }
}

}  // namespace detail::type_list

}  // namespace lyrahgames::xstd
