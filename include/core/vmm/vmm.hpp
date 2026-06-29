#pragma once
#include "common.hpp"

using phys_page                      = u64;
using virt_page                      = u64;
using virt_addr                      = u64;
static constexpr u64 UNMAPPED_PAGE   = -0x1;
static constexpr u64 PPC64_PAGE_SIZE = 0x10000;

// 0x(FFFF)(FFFF)(FFFF)[FFFF]

struct OuterPageTable {
  std::unordered_map<u16, u16> page_table;
};

using PageTableEntry = std::unordered_map<u16, u16>;
struct VMM {
  u32 page_size{};
  using ThirdLevelTable  = std::unordered_map<u16, i32>;
  using SecondLevelTable = std::unordered_map<u16, ThirdLevelTable*>;

  std::unordered_map<u16, SecondLevelTable*> first_level_table_base;  // holds pointers to the bases of a 2nd-level page tables, indexed into using first level
  std::unordered_map<u16, ThirdLevelTable*> second_level_table_base;  // holds pointers to the bases of a 3rd-level page tables, indexed into second level
  std::unordered_map<u16, u16> third_level_page_table;                // holds page entries, indexed into using third level

  VMM(u32 page_sz) {
    assert((page_size & (page_size - 1)) == 0);
    page_size = page_sz;
  };

  void map();
  phys_page get_page_number(u64 virt_addr);

  std::unordered_map<virt_page, phys_page> page_table{};
  u64 virt_to_phys(u64 virt_addr);
};

static VMM vmm{PPC64_PAGE_SIZE};
