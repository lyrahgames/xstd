#pragma once
#include <chrono>
#include <concepts>
#include <functional>
//
#include <lyrahgames/xstd/meta.hpp>
#include <lyrahgames/xstd/utility.hpp>

namespace lyrahgames::xstd {

// Measure the duration of a given function by using perfect forwarding.
template <typename base = float64, typename functor, typename... arguments>
requires std::invocable<functor, arguments...>  //
inline auto duration(functor&& f, arguments&&... args) {
  using namespace std;
  using clock = chrono::high_resolution_clock;

  const auto start = clock::now();

  // If the function has a return value,
  // we make sure to get it such that
  // the compiler does not optimize it away.
  if constexpr (!meta::equal<void, meta::result<functor>>)
    volatile auto result =
        std::invoke(std::forward<functor>(f), std::forward<arguments>(args)...);
  else
    std::invoke(std::forward<functor>(f), std::forward<arguments>(args)...);

  const auto end = clock::now();
  return chrono::duration<base>(end - start);
}

}  // namespace lyrahgames::xstd
