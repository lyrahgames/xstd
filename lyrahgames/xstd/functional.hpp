#pragma once
#include <lyrahgames/xstd/static_index_list.hpp>
#include <lyrahgames/xstd/type_list/type_list.hpp>

namespace lyrahgames::xstd {

namespace detail {

//
template <typename F, typename indices, typename... Args>
struct permuted_invocable_traits {};
//
template <typename F, size_t... indices, typename... Args>
struct permuted_invocable_traits<F, static_index_list<indices...>, Args...> {
  using arg_list = xstd::type_list<Args...>;

  using result_type = std::  //
      invoke_result_t<F, meta::type_list::element<arg_list, indices>...>;

  static constexpr bool invocable = std::  //
      invocable<F, typename arg_list::template element<indices>...>;

  static constexpr bool nothrow_invocable = std::  //
      is_nothrow_invocable_v<F, meta::type_list::element<arg_list, indices>...>;
};

}  // namespace detail

namespace generic {

/// The concept checks whether 'F' is invocable with
/// the types 'Args...' after applying a permutation given by 'indices.'
///
template <typename F, typename indices, typename... Args>
concept permuted_invocable =
    detail::permuted_invocable_traits<F, indices, Args...>::invocable;

/// The concept checks whether 'F' is invocable without throwing and with
/// the types 'Args...' after applying a permutation given by 'indices.'
///
template <typename F, typename indices, typename... Args>
concept permuted_nothrow_invocable =
    detail::permuted_invocable_traits<F, indices, Args...>::nothrow_invocable;

}  // namespace generic

namespace meta {

/// Returns the result type of a permuted invocation of a callable.
///
template <typename F, typename indices, typename... Args>
using permuted_invocable_result = typename detail::
    permuted_invocable_traits<F, indices, Args...>::result_type;

}  // namespace meta

///
namespace detail {
template <size_t... indices, typename F, typename... Args>
constexpr auto permuted_invoke(static_index_list<indices...> list,
                               F&& f,
                               Args&&... args)  //
    noexcept(generic::permuted_nothrow_invocable<F, decltype(list), Args...>)
        -> meta::permuted_invocable_result<F, decltype(list), Args...>
requires generic::permuted_invocable<F, decltype(list), Args...> {
  return std::invoke(std::forward<F>(f),
                     forward_element<indices>(std::forward<Args>(args)...)...);
}
}  // namespace detail

///
///
template <instance::static_index_list indices, typename F, typename... Args>
constexpr auto permuted_invoke(F&& f, Args&&... args) noexcept(
    generic::permuted_nothrow_invocable<F, indices, Args...>)
    -> meta::permuted_invocable_result<F, indices, Args...>
requires generic::permuted_invocable<F, indices, Args...> {
  return detail::permuted_invoke(indices{}, std::forward<F>(f),
                                 std::forward<Args>(args)...);
}

}  // namespace lyrahgames::xstd
