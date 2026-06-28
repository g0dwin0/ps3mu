#pragma once
#include "common.hpp"

using phys_page = u16;
using virt_page = u16;
using phys_addr = u64;
using virt_addr = u64;

static constexpr u64 PPC64_PAGE_SIZE = 0x10000;


struct PageTableEntry {};

struct VMM {

  VMM() = delete;

  VMM(u32 page_sz) {
    assert((page_size & (page_size - 1)) == 0);
    page_size = page_sz;
  };

  u32 page_size{};
  std::unordered_map<virt_page, phys_page> page_table{};
};

struct TLB {
  // identifier for entry in the TLB
  using Tag      = u32;

  // physical page number (not the address!)
  using phys_page_t = u32;

  std::unordered_map<Tag, phys_page_t> tlb;

  void evict(Tag tag) { tlb.erase(tag); };
  phys_page_t read(u64 address) {
    Tag tag            = address / PPC64_PAGE_SIZE;
    phys_page_t phys_page = 0;

    if (tlb.contains(tag)) {
      return phys_page;
    } else {
      // walk page table

      // insert tag & phys into TLB
      u32 phys = 0x0;
      tlb.insert({tag, phys});

      return phys_page;
    }
  }
  u8 MAX_TLB_SIZE = 0xFF;
};
