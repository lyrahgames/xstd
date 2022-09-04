#include <iomanip>
#include <iostream>
//
#include <lyrahgames/xstd/contracts.hpp>

using namespace std;
using namespace lyrahgames::xstd;

non_negative<float> f(non_negative<float> x) {
  return x;
}
positive<float> g(negative<double> x) {
  return -x;
}

int main() {
  try {
    g(f(1));
  } catch (contract_violation& error) {
    cout << "Contract Violation at " << error.what() << '\n';
  }
}
