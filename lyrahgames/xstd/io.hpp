#pragma once
// This header module provides some standards for I/O.

// Get all the basics.
//
#include <lyrahgames/xstd/utility.hpp>

// We want to be able to work with files,
// the console, and other string formatting.
//
#include <fstream>
#include <iostream>
#include <sstream>

namespace lyrahgames::xstd {

// 'czstring' is used because the given file path is only read once.
//
inline auto string_from_file(czstring file_path) -> std::string {
  using namespace std;
  // We will read all characters as block.
  // Hence, open the file in binary mode.
  // Make sure to jump to its end for directly reading its size.
  ifstream file{file_path, ios::binary | ios::ate};
  if (!file)
    throw runtime_error("Failed to open the file '"s + file_path + "'.");
  // Read the file size.
  auto size = file.tellg();
  // Prepare the result string with a sufficiently large buffer.
  string result(size, '\0');
  // Go back to the start and read all characters at once.
  file.seekg(0);
  file.read(result.data(), size);
  return result;
};

}  // namespace lyrahgames::xstd
