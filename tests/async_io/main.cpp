#include <lyrahgames/xstd/async_io.hpp>

using namespace std;
using namespace lyrahgames::xstd;

int main() {
  int counter = 0;
  auto line_read = async_line_read("$> ");
  while (!line_read.available()) {
    ++counter;
    this_thread::sleep_for(10ms);
  }
  const auto line = line_read.get();
  cout << line << endl << counter << endl;
}
