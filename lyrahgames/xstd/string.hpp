#pragma once
#include <concepts>
#include <string>

namespace lyrahgames::xstd {

namespace type {
using string = std::string;
}
using string_t = type::string;

// Function Overloads for Constructors
template <typename... types>
requires std::constructible_from<type::string, types...>  //
    auto string(types&&... values) {
  return type::string(std::forward<types>(values)...);
}

// Function Overloads for std::to_string
template <typename... types>
requires(!std::constructible_from<type::string, types...>)  //
    auto string(types&&... values)
        -> decltype(std::to_string(std::forward<types>(values)...)) {
  return std::to_string(std::forward<types>(values)...);
}

}  // namespace lyrahgames::xstd