#include <doctest/doctest.h>
//
#include <iomanip>
#include <iostream>
//
#include <lyrahgames/xstd/static_radix_tree.hpp>

using namespace std;
using namespace lyrahgames::xstd;
using namespace meta::type_list;

// static_assert(is_static_radix_node<static_radix_node<"", true>>);

// using type = detail::insertion<static_radix_node<"", true>>;

template <static_radix_tree::instance::node root, static_zstring prefix>
constexpr void print() {
  using namespace std;
  constexpr auto str = prefix + '|' + root::string;
  for_each<typename root::children>(
      [&]<typename child> { print<child, str>(); });
  if constexpr (root::is_leaf) cout << '"' << str << '"' << endl;
}

template <typename root>
constexpr void print() {
  using namespace std;
  for_each<typename root::children>(
      [&]<typename child> { print<child, root::string>(); });
  if constexpr (root::is_leaf) cout << '"' << root::string << '"' << endl;
}

namespace {
namespace srt = static_radix_tree;
using srt::leaf;
using srt::node;
static_assert(srt::instance::node_list<type_list<>>);
static_assert(!srt::instance::node_list<type_list<float>>);
static_assert(srt::instance::node_list<type_list<node<"test">>>);
static_assert(srt::instance::node_list<type_list<leaf<"">>>);
static_assert(!srt::instance::node_list<type_list<node<"test">, float>>);
}  // namespace

SCENARIO("") {
  // using tree = static_radix_tree<"help", "version", "test", "hell">;
  using tree = static_radix_tree::construction<
      "help", "version", "helo", "hel", "verbose", "help-me", "abc", "key",
      "check", "make", "", "input", "output", "man", "cheat", "in", "out",
      "help", "help">;
  print<tree>();
}
