#pragma once
#include <chrono>
#include <concepts>
#include <functional>
//
#include <lyrahgames/xstd/builtin_types.hpp>
#include <lyrahgames/xstd/meta.hpp>

namespace lyrahgames::xstd {

template <typename base = float64, typename functor, typename... args>
requires std::invocable<functor, args...>  //
    inline auto duration(functor&& f, args&&... v) {
  const auto start = std::chrono::high_resolution_clock::now();
  if constexpr (!std::same_as<meta::result<functor>, void>) {
    volatile auto result =
        std::invoke(std::forward<functor>(f), std::forward<args>(v)...);
  } else {
    std::invoke(std::forward<functor>(f), std::forward<args>(v)...);
  }
  const auto end = std::chrono::high_resolution_clock::now();
  return std::chrono::duration<base>(end - start);
}

}  // namespace lyrahgames::xstd