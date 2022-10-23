#include <lyrahgames/xstd/type_list/detail/functions.hpp>

namespace lyrahgames::xstd {

namespace detail::type_list {

template <typename... types>
struct base {
  using self = type_list<types...>;

  static constexpr size_t size = detail::type_list::size<self>::value;

  static constexpr size_t alignment = detail::type_list::alignment<self>::value;
  static constexpr size_t struct_byte_size =
      detail::type_list::struct_byte_size<self>::value;
  static constexpr size_t tuple_byte_size =
      detail::type_list::tuple_byte_size<self>::value;

  template <size_t index>
  requires(index <= size - 1)  //
      static constexpr size_t struct_offset =
          detail::type_list::struct_offset<self, index>::value;

  template <size_t index>
  requires(index <= size - 1)  //
      static constexpr size_t tuple_offset =
          detail::type_list::tuple_offset<self, index>::value;

  template <size_t index>
  requires(index <= size - 1)  //
      static constexpr size_t struct_padding =
          detail::type_list::struct_padding<self, index>::value;

  static constexpr bool empty = detail::type_list::empty<self>::value;

  template <typename type>
  static constexpr bool contains = (meta::equal<type, types> || ...);

  template <size_t index>
  // requires(index <= sizeof...(types))  //
  // using element = typename detail::type_list::element<index, T,
  // types...>::type;
  // using element = typename detail::type_list::element<index, types...>::type;
  using element = typename detail::type_list::element<self, index>::type;

  template <typename type>
  requires(contains<type>)  //
      static constexpr size_t index =
          detail::type_list::index<self, type>::value;

  template <typename type>
  using push_front = typename detail::type_list::push_front<self, type>::type;

  template <typename type>
  using push_back = typename detail::type_list::push_back<self, type>::type;

  template <size_t index, typename type>
  using insert = typename detail::type_list::insert<self, index, type>::type;

  template <size_t index>
  using remove = typename detail::type_list::remove<self, index>::type;

  template <size_t index, typename type>
  using assignment =
      typename detail::type_list::assignment<self, index, type>::type;

  template <size_t first, size_t last>
  using range = typename detail::type_list::range<self, first, last>::type;

  template <size_t count>
  using trim_front = typename detail::type_list::trim_front<self, count>::type;

  template <size_t count>
  using trim_back = typename detail::type_list::trim_back<self, count>::type;

  template <typename list>
  using concat = typename detail::type_list::concatenation<self, list>::type;

  using reverse = typename detail::type_list::reverse<self>::type;

  template <size_t i, size_t j>
  using swap = typename detail::type_list::swap<self, i, j>::type;

  template <template <typename, typename> typename less>
  using sort = typename detail::type_list::sort<self, less>::type;

  template <typename type, template <typename, typename> typename condition>
  using insert_when =
      typename detail::type_list::insert_when<self, type, condition>::type;

  template <template <typename> typename f>
  using transformation =
      typename detail::type_list::transformation<self, f>::type;

  static constexpr auto for_each(auto&& f) {
    (f.template operator()<types>(), ...);
  }

  static constexpr bool for_each_until(auto&& f) {
    return detail::type_list::for_each_until<self>(
        std::forward<decltype(f)>(f));
  }
};

}  // namespace detail::type_list

/// The type specialization for an empty type list.
/// An empty type list does not provide access to the front or back.
/// Also, to pop from the front or back makes no sense.
///
template <>
struct type_list<> : detail::type_list::base<> {};

/// The type specialization for a non-empty type list.
///
template <typename T, typename... types>
struct type_list<T, types...> : detail::type_list::base<T, types...> {
  using base = detail::type_list::base<T, types...>;
  using base::size;

  using self = type_list<T, types...>;

  using front = typename detail::type_list::front<self>::type;
  using back = typename detail::type_list::back<self>::type;

  using pop_front = typename detail::type_list::pop_front<self>::type;
  using pop_back = typename detail::type_list::pop_back<self>::type;
};

}  // namespace lyrahgames::xstd
