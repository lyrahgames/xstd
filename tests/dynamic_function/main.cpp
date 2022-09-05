#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
//
#include <lyrahgames/xstd/dynamic_function.hpp>

using namespace std;
using namespace lyrahgames::xstd;

int main() {
  serializer<istream&, ostream&,
             decltype([](istream& in, auto& x) { in >> x; }),
             decltype([](const auto& x, ostream& out) { out << x << endl; })>
      s{};
  using function_type = decltype(s)::dynamic_function;

  unordered_map<string, function_type> calls{};
  calls["exit"] = s.create([]() { exit(0); });
  calls["print"] = s.create([](string x) { cout << x << endl; });
  calls["add"] = s.create([](float x, float y) { return x + y; });
  int i = 0;
  calls["state"] = s.create([&i]() { return i++; });
  calls["help"] = s.create([&calls]() {
    for (const auto& [name, _] : calls)
      cout << name << endl;
  });

  string line;
  while (getline(cin, line)) {
    stringstream input{line};

    string cmd;
    input >> cmd;

    auto it = calls.find(cmd);
    if (it == calls.end()) {
      cerr << "Unknown function." << endl;
      continue;
    }
    auto [name, call] = *it;
    call(input, cout);
  }
}
