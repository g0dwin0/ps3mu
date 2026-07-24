#pragma once
#include <arpa/inet.h>

#include <vector>

#include "common/defs.hpp"
#include "common/types.hpp"
#include "vmm/vmm.hpp"

struct Bus {
  Bus() : memory(512_MiB) {}

  std::vector<u8> memory;

  template <AccessWidth Width>
  [[nodiscard]] Width read(u64 _address) {
    spdlog::info("{:#016x}", _address);
    Width r = *(Width*)&memory.at(vmm.virt_to_phys(_address));

    if constexpr (sizeof(Width) == 2) {
      r = htons(r);
    }
    if constexpr (sizeof(Width) == 4) {
      r = htonl(r);
    }
    if constexpr (sizeof(Width) == 8) {
      const auto& mem = memory.data() + vmm.virt_to_phys(_address);
      r               = (u64)mem[0] << 56 | (u64)mem[1] << 48 | (u64)mem[2] << 40 | (u64)mem[3] << 32 | (u64)mem[4] << 24 | (u64)mem[5] << 16 | (u64)mem[6] << 8 | (u64)mem[7] << 0;
    }

    spdlog::info("<R{}> {:#016x} -> {:#016x}", sizeof(Width) * 8, _address, r);

    return r;
  }

  template <AccessWidth Width>
  void write(u64 _address, u64 value) {
    const auto& mem = memory.data() + (vmm.virt_to_phys(_address));

    if constexpr (sizeof(Width) == 1) {
      mem[0] = value & 0xff;
    }
    if constexpr (sizeof(Width) == 2) {
      mem[0] = (value >> 8) & 0xff;
      mem[1] = (value >> 0) & 0xff;
    }
    if constexpr (sizeof(Width) == 4) {
      mem[0] = (value >> 24) & 0xff;
      mem[1] = (value >> 16) & 0xff;
      mem[2] = (value >> 8) & 0xff;
      mem[3] = (value >> 0) & 0xff;
    }
    if constexpr (sizeof(Width) == 8) {
      mem[0] = (value >> 56) & 0xff;
      mem[1] = (value >> 48) & 0xff;
      mem[2] = (value >> 40) & 0xff;
      mem[3] = (value >> 32) & 0xff;
      mem[4] = (value >> 24) & 0xff;
      mem[5] = (value >> 16) & 0xff;
      mem[6] = (value >> 8) & 0xff;
      mem[7] = (value >> 0) & 0xff;
    }

    spdlog::info("<W{}> {:#016x} -> {:#016x}", sizeof(Width) * 8, _address, value);
  }
};
