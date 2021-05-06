#include <cmath>
#include <iomanip>
#include <iostream>
//
#include <lyrahgames/xstd/xstd.hpp>

using namespace std;
using namespace lyrahgames;

int main() {
  using namespace xstd;
  using xstd::string;
  cout << string(20, 'c') << '\n'
       << string(5.0f) << '\n'
       << string(-3) << '\n'
       << string('a') << '\n'
       << string("Hello!") << '\n'
       << setprecision(16)  //
       << float32(M_PI) << '\n';
}