#pragma once
#include <future>
#include <mutex>
//
#include <string>
#include <string_view>
//
#include <iostream>

namespace lyrahgames::xstd {

struct async_cio_state : std::future<std::string> {
  async_cio_state() = default;
  async_cio_state(std::future<std::string>&& f)
      : std::future<std::string>{std::move(f)} {}

  bool available() const {
    using namespace std::chrono_literals;
    return std::future_status::ready == wait_for(0s);
  }
};

inline std::mutex async_cio_mutex{};

inline auto async_line_read(std::string_view prompt = "\n>>> ")
    -> async_cio_state {
  using namespace std;
  string s{prompt};
  return async(launch::async, [prompt = move(s)]() {
    scoped_lock lock{async_cio_mutex};
    cout << prompt << flush;
    string line;
    getline(cin, line);
    return line;
  });
}

}  // namespace lyrahgames::xstd
