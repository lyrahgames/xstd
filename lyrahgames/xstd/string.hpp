#pragma once
#include <concepts>
#include <string>

namespace lyrahgames::xstd {

// Function Overloads for Constructors
template <typename... types>
requires std::constructible_from<std::string, types...>  //
inline auto string(types&&... values) {
  return std::string(std::forward<types>(values)...);
}

// Function Overloads for std::to_string
template <typename... types>
requires(!std::constructible_from<std::string, types...>)  //
    inline auto string(types&&... values)
        -> decltype(std::to_string(std::forward<types>(values)...)) {
  return std::to_string(std::forward<types>(values)...);
}

}  // namespace lyrahgames::xstd
