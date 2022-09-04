#include <lyrahgames/xstd/io.hpp>

using namespace std;
using namespace lyrahgames::xstd;

int main(int argc, char* argv[]) {
  const auto text = string_from_file(argv[1]);
  cout << text << flush;
}
