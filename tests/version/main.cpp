#include <iomanip>
#include <iostream>
#include <sstream>

#include <lyrahgames/xstd/version.hpp>

using namespace std;
using namespace lyrahgames::xstd;

int main() {
  stringstream stream;
  stream << left << setw(20) << "version::number"
         << " = " << right << setw(40) << version::number << '\n'
         << left << setw(20) << "version::string"
         << " = " << right << setw(40) << version::string << '\n'
         << left << setw(20) << "version::id"
         << " = " << right << setw(40) << version::id << '\n'
         << left << setw(20) << "version::full"
         << " = " << right << setw(40) << version::full << '\n'
         << left << setw(20) << "version::major"
         << " = " << right << setw(40) << version::major << '\n'
         << left << setw(20) << "version::minor"
         << " = " << right << setw(40) << version::minor << '\n'
         << left << setw(20) << "version::patch"
         << " = " << right << setw(40) << version::patch << '\n'
         << left << setw(20) << "version::pre_release"
         << " = " << right << setw(40) << version::pre_release << '\n'
         << left << setw(20) << "version::snapshot_sn"
         << " = " << right << setw(40) << version::snapshot_sn << '\n'
         << left << setw(20) << "version::snapshot_id"
         << " = " << right << setw(40) << version::snapshot_id << '\n';
  cout << stream.str();
}
