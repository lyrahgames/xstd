#include <iomanip>
#include <iostream>
#include <thread>
//
#include <lyrahgames/xstd/chrono.hpp>
#include <lyrahgames/xstd/string.hpp>

using namespace std;
using namespace chrono_literals;
using namespace lyrahgames;
using namespace xstd;

int sum(int n) {
  int result = 0;
  for (int i = 0; i < n; ++i) result += i;
  return result;
}

int main() {
  {
    const auto time = duration(sum, 1'000'000'000);
    cout << setw(15) << "time = " << time.count() << '\n';
  }
  {
    const auto time = duration([] { return xstd::string("Hello!"); });
    cout << setw(15) << "time = " << time.count() << '\n';
  }
  {
    const auto time = duration([] { this_thread::sleep_for(200ms); });
    cout << setw(15) << "time = " << time.count() << '\n';
  }
  {
    const auto time = duration<float32>([] { this_thread::sleep_for(200ms); });
    cout << setw(15) << "time = " << time.count() << '\n';
  }
}