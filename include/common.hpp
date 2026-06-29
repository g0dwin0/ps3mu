#pragma once

#include <climits>
#include <arpa/inet.h>
#include "types.hpp"
#define CONCAT(x, y) x##y
#define EXPAND(x, y) CONCAT(x, y)
#define RESERVED EXPAND(reserved, __LINE__)
// #define htonll(x) ((((long long)htonl(x)) << 32) + (htonl((x) >> 32)))


#include <cassert>
#include <fstream>
#include <iterator>
#include <vector>

#include "common/defs.hpp"
#include "spdlog/spdlog.h"

inline std::vector<u8> read_file(std::string filename) {
  std::ifstream file(filename.data(), std::ios::binary);

  if (!file.good()) {
    fmt::println("failed to load file: {}", filename);
    // assert(0 & "failed to load file");
    assert(0);
  }

  file.unsetf(std::ios::skipws);
  std::streampos fileSize;
  file.seekg(0, std::ios::end);
  fileSize = file.tellg();
  file.seekg(0, std::ios::beg);
  std::vector<u8> vec;
  vec.reserve(fileSize);

  vec.insert(vec.begin(), std::istream_iterator<u8>(file), std::istream_iterator<u8>());

  return vec;
}

inline void hexdump(std::vector<u8>& src, size_t len, size_t base) {
  // TODO: add ascii setting
  for (size_t i = 0; i < len; i++) {
    if (i != 0 && i % 16 == 0) {
      fmt::print("\n");
    }

    if (i % 16 == 0) fmt::print("{:#08x}: ", base + i);

    uint8_t value = src.at(base + i);
    fmt::print("{:#04x}", value);
    fmt::print(" ");
  }
}


template <AccessWidth width>
inline width read_p_be(width val) {
  width r = 0;

  if constexpr (sizeof(val) == 2) {
    r = htons(val);
  }
  if constexpr (sizeof(val) == 4) {
    r = htonl(val);
  }
  if constexpr (sizeof(val) == 8) {
    r = htonll(val);
  }


return r;
}