#pragma once

#include <arpa/inet.h>

#include <climits>
#include <source_location>
#include <string>

#include "common/types.hpp"
#define CONCAT(x, y) x##y
#define EXPAND(x, y) CONCAT(x, y)
#define RESERVED EXPAND(reserved, __LINE__)
#ifdef __unix
#define htonll(x) ((((long long)htonl(x)) << 32) + (htonl((x) >> 32)))
#endif
#include <cassert>
#include <fstream>
#include <iterator>
#include <vector>

#include "common/defs.hpp"
#include "spdlog/spdlog.h"
inline u64 global_step_count = 0;
inline std::vector<u8> read_file(const std::string filename) {
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

    u8 value = src.at(base + i);
    fmt::print("{:#04x}", value);
    fmt::print(" ");
  }
}

inline static void FATAL(const std::string x = "unhandled", const std::source_location location = std::source_location::current()) {
  fmt::println("FATAL: {} -- {}:{} gs: {}", x, location.file_name(), location.line(), global_step_count);
  exit(-1);
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

template <typename T>
struct BE {
  T value;

  BE() = default;

  BE(T v) { *this = v; }

  operator T() const {
    if constexpr (std::endian::native == std::endian::big) return value;
    else return std::byteswap(value);
  }

  constexpr T get() const noexcept { return *this; }

  BE& operator=(T v) {
    if constexpr (std::endian::native == std::endian::big) value = v;
    else value = std::byteswap(v);

    return *this;
  }
};

template <typename T>
struct fmt::formatter<BE<T>> : fmt::formatter<T> {
  auto format(const BE<T>& v, format_context& ctx) const { return fmt::formatter<T>::format(static_cast<T>(v), ctx); }
};

inline u64 MASK(u8 x, u8 y) {
  fmt::println("X: {}", x);
  fmt::println("X: {}", x);
  
  assert(x <= y);
  u64 mask = 0;

  for (size_t idx = x; idx <= y; idx++) {
    mask |= (1ULL << idx);
    // fmt::println("MASK: {:#066b}", mask);
    // fmt::println("MASK: {}", mask);
  }
  return mask;
}

inline u64 EXTS(const u64 msb, const u64 value) {
  u64 masked_retval = value & (1ULL << msb) - 1;
  bool msb_set      = (value & (1ULL << msb)) != 0;

  if (msb_set) {
    const u64 set_bits = U64_MAX << msb;
    masked_retval |= set_bits;
  }

  return masked_retval;
}
