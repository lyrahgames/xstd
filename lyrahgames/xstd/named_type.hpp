#pragma once
#include <lyrahgames/xstd/static_zstring.hpp>

namespace lyrahgames::xstd {

template <static_zstring N, typename T>
struct named_type : T {
  static constexpr auto name = N;
  using type = T;
  using type::type;
};

template <static_zstring N, typename T>
requires std::is_final_v<T> ||(!std::is_class_v<T>)  //
    struct named_type<N, T> {
  using type = T;

  // constexpr named_type() = default;

  template <typename... arguments>
  constexpr named_type(arguments&&... args)
      : value(std::forward<arguments>(args)...) {}

  constexpr operator type() noexcept { return value; }

  type value{};
};

}  // namespace lyrahgames::xstd
