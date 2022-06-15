#pragma once
#include <lyrahgames/xstd/static_zstring.hpp>
#include <lyrahgames/xstd/type_list.hpp>

namespace lyrahgames::xstd {

template <static_zstring str,
          bool leaf = false,
          instance::type_list nodes = type_list<>>
struct static_radix_node {
  static constexpr static_zstring string = str;
  static constexpr bool is_leaf = leaf;
  using children = nodes;
};

namespace detail {
template <typename T>
struct is_static_radix_node : std::false_type {};
template <static_zstring str, bool leaf, instance::type_list children>
struct is_static_radix_node<static_radix_node<str, leaf, children>>
    : std::true_type {};
}  // namespace detail
template <typename T>
constexpr bool is_static_radix_node = detail::is_static_radix_node<T>::value;

namespace instance {
template <typename T>
concept static_radix_node = is_static_radix_node<T>;
}  // namespace instance

namespace detail::static_radix_tree {

using xstd::type_list;
using namespace meta::type_list;

template <static_zstring str,
          bool is_leaf = false,
          instance::type_list children = type_list<>>
using node = static_radix_node<str, is_leaf, children>;

template <static_zstring str, instance::type_list children = type_list<>>
using leaf = node<str, true, children>;

template <typename T>
constexpr bool is_node = xstd::is_static_radix_node<T>;

namespace instance {
template <typename T>
concept node = xstd::instance::static_radix_node<T>;
}  // namespace instance

// namespace detail {
// template <typename T>
// struct is_node_list : std::false_type {};
// template <generic::node... nodes>
// struct is_node_list<type_list<nodes...>> : std::true_type {};
// }  // namespace detail
// template <typename T>
// constexpr bool is_node_list = detail::is_node_list<T>::value;

// namespace generic {
// template <typename T>
// concept node_list = is_node_list<T>;
// }

// template <static_zstring str, bool l, generic::node_list nodes>
// struct node<str, l, nodes> {
//   static constexpr static_zstring string = str;
//   static constexpr bool leaf = l;
//   using children = nodes;
// };

template <instance::node root, static_zstring str, size_t index>
struct insertion_helper;

template <instance::node root, static_zstring str>
struct insertion {
  using type =
      typename insertion_helper<root,
                                str,
                                prefix_match_index(root::string, str)>::type;
};

template <instance::node root, static_zstring str, size_t index>
struct insertion_helper;

// No Match
template <instance::node root, static_zstring str>
requires(!root::string.empty())  //
    struct insertion_helper<root, str, 0> {
  using type = root;
};

// Full Match
// The full match is a projection and
// makes sure that a given string can only be inserted once.
template <instance::node root, static_zstring str, size_t index>
requires(index == root::string.size()) && (index == str.size())  //
    struct insertion_helper<root, str, index> {
  using type = node<root::string, true, typename root::children>;
};

// Full String Match
template <instance::node root, static_zstring str, size_t index>
requires(index < root::string.size()) && (index == str.size())  //
    struct insertion_helper<root, str, index> {
  using type = node<str,
                    true,
                    type_list<node<tail<index>(root::string),
                                   root::is_leaf,
                                   typename root::children>>>;
};

template <instance::node root,
          static_zstring str,
          size_t index,
          bool match = false>
struct full_prefix_insertion_helper {
  using type = node<root::string,
                    root::is_leaf,
                    push_back<typename root::children,
                              node<tail<index>(str), true, type_list<>>>>;
};

template <instance::node root, static_zstring str, size_t index>
struct full_prefix_insertion_helper<root, str, index, true> {
  template <instance::node _>
  using inserter = insertion<_, tail<index>(str)>;
  using type = node<root::string,
                    root::is_leaf,
                    transformation<typename root::children, inserter>>;
};

template <static_zstring str, xstd::instance::type_list nodes>
struct first_character_match;
template <static_zstring str, typename... types>
struct first_character_match<str, type_list<types...>> {
  static constexpr bool value = ((str[0] == types::string[0]) || ...);
};

// Full Prefix Match
template <instance::node root, static_zstring str, size_t index>
requires(index == root::string.size()) && (index < str.size())  //
    struct insertion_helper<root, str, index> {
  static constexpr bool matched =
      first_character_match<tail<index>(str), typename root::children>::value;
  using type =
      typename full_prefix_insertion_helper<root, str, index, matched>::type;
};

// Partial Match
template <instance::node root, static_zstring str, size_t index>
requires(index < root::string.size()) && (index < str.size())  //
    struct insertion_helper<root, str, index> {
  using type = node<prefix<index>(str),
                    false,
                    type_list<node<tail<index>(root::string),
                                   root::is_leaf,
                                   typename root::children>,
                              node<tail<index>(str), true, type_list<>>>>;
};

template <instance::node root, static_zstring... str>
struct constructor {
  using type = root;
};
template <instance::node root, static_zstring str, static_zstring... tail>
struct constructor<root, str, tail...> {
  using type =
      typename constructor<typename insertion<root, str>::type, tail...>::type;
};

// template <typename root, static_zstring prefix = "">
// struct visit;
// template <static_zstring prefix,
//           static_zstring name,
//           bool is_leaf,
//           typename... children>
// struct visit<basic_node<name, is_leaf, children...>, prefix> {
//   constexpr bool operator()(czstring str, auto&& f) const noexcept {
//     constexpr auto new_prefix = prefix + name;
//     const auto tail = prefix_match<name>(str);
//     if (!tail) return false;

//     if constexpr (is_leaf) {
//       if (!*tail) {
//         std::forward<decltype(f)>(f).template operator()<new_prefix>();
//         return true;
//       }
//     } else {
//       if (!*tail) return false;
//     }

//     return static_for_each_until<children...>(
//         [&]<typename root> { return visit<root, new_prefix>{}(tail, f); });
//   }
// };

}  // namespace detail::static_radix_tree

template <static_zstring... str>
using static_radix_tree = typename detail::static_radix_tree::constructor<
    detail::static_radix_tree::node<"", false, type_list<>>,
    str...>::type;

}  // namespace lyrahgames::xstd
