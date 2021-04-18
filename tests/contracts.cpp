#include <iomanip>
#include <iostream>
//
#include <lyrahgames/xstd/contracts.hpp>

using namespace std;
using namespace lyrahgames::xstd;

non_negative<float> f(non_negative<float> x) { return x; }
positive<float> g(positive<double> x) { return -x; }

int main() {
  try {
    g(f(1));
  } catch (contract_violation) {
    cout << "Contract Violation!\n";
  }
}