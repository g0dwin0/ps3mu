#pragma once
#include <vector>

#include "common/defs.hpp"
#include "types.hpp"

struct Bus {
  Bus() { memory.resize(512 * 1024 * 1024, 0); }

  template <AccessWidth Width>
  [[nodiscard]] Width read(u32 _address) {
    (void)_address;

    return *(Width*)&memory.at(_address);
  }

  template <AccessWidth Width>
  [[nodiscard]] Width write(u32 _address, u32 value) {
    *(Width)&memory.at(_address) = value;
  }

  std::vector<u8> memory;
};
