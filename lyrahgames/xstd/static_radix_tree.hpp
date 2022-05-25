#pragma once
#include <lyrahgames/xstd/static_zstring.hpp>
#include <lyrahgames/xstd/type_list.hpp>

namespace lyrahgames::xstd {

namespace static_radix_tree {

template <static_zstring str, bool leaf, typename children>
struct node;

namespace detail {
template <typename T>
struct is_node : std::false_type {};
template <static_zstring str, bool leaf, typename... children>
struct is_node<node<str, leaf, children...>> : std::true_type {};
}  // namespace detail
template <typename T>
constexpr bool is_node = detail::is_node<T>::value;

namespace generic {
template <typename T>
concept node = is_node<T>;
}  // namespace generic

namespace detail {
template <typename T>
struct is_node_list : std::false_type {};
template <generic::node... nodes>
struct is_node_list<type_list<nodes...>> : std::true_type {};
}  // namespace detail
template <typename T>
constexpr bool is_node_list = detail::is_node_list<T>::value;

namespace generic {
template <typename T>
concept node_list = is_node_list<T>;
}

template <static_zstring str, bool l, generic::node_list nodes>
struct node<str, l, nodes> {
  static constexpr static_zstring string = str;
  static constexpr bool leaf = l;
  using children = nodes;
};

namespace detail {

// template <generic::node root, static_zstring str, size_t index>
// requires(root::string == str)  //
//     struct insertion {
//   using type = node<root::string, true, root::children>;
// };

// template <generic::node root, static_zstring str>
// struct insertion<root, str, 0> {
//   using type = root;
// };

// template <generic::node root, static_zstring str, size_t index>
// requires(index == root::string.size())  //
//     struct insertion<root, str, index> {
//   using type = node<root::string,
//                     root::leaf,
//                     append<root::children, node<tail<index>(str), true>>>;
// };

}  // namespace detail

}  // namespace static_radix_tree

}  // namespace lyrahgames::xstd
