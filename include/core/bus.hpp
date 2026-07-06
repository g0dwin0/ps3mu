#pragma once
#include <arpa/inet.h>

#include <vector>
#include "common.hpp"
#include "common/defs.hpp"
#include "common/types.hpp"
#include "vmm/vmm.hpp"

struct Bus {
  Bus(): memory(512_MiB) {}



  
  template <AccessWidth Width>
  [[nodiscard]] Width read(u32 _address) {
    fmt::println("{:#016x}", vmm.virt_to_phys(_address));
    Width r = *(Width*)&memory.at(vmm.virt_to_phys(_address));

    if constexpr (sizeof(Width) == 2) {
      r = htons(r);
    }
    if constexpr (sizeof(Width) == 4) {
      r = htonl(r);
    }
    if constexpr (sizeof(Width) == 8) {
      r = htonll(r);
    }

    return r;
  }

  template <AccessWidth Width>
  void write(u64 _address, u32 value) {
    fmt::println("<W{}> {:#016x} -> {:#016x}", sizeof(Width) * 8, _address, value);
    *(Width*)&memory.at(vmm.virt_to_phys(_address)) = value;
  }

  std::vector<u8> memory;
};
