#pragma once
#include "common/defs.hpp"
#include "types.hpp"

struct Bus {
  template <AccessWidth Width>
  [[nodiscard]] Width read(u32 _address) {
    (void)_address;
  }

  template <AccessWidth Width>
  [[nodiscard]] Width write(u32 _address, u32 value) {
    (void)_address;
    (void)value;
  }
};
