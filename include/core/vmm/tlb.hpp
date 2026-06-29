#pragma once
#include "common.hpp"
#include "vmm.hpp"

// identifier for entry in the TLB
using Tag = u32;
// physical page number (not the address!)
using phys_page_t = u32;

struct TLB {
  static const u16 MAX_TLB_SIZE = 0x1000;

  std::unordered_map<Tag, phys_page_t> tlb;

  phys_page_t read(u64 address);

  void evict(Tag tag);
  void map();
};

static TLB tlb;