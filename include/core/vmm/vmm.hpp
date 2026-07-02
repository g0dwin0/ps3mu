#pragma once
#include <spdlog/fmt/ranges.h>

#include <unordered_map>

#include "common.hpp"
#include "common/literals.hpp"
#include "spdlog/sinks/stdout_color_sinks.h"

using phys_page                      = u64;
using virt_page                      = u64;
using virt_addr                      = u64;
static constexpr u64 UNMAPPED_PAGE   = -0x1;
static constexpr u64 PPC64_PAGE_SIZE = 0x10000;
using page_in_use                    = bool;
enum page_status : bool {
  IN_USE    = true,
  AVAILABLE = false,
};
// 0x(FFFF)(FFFF)(FFFF)[FFFF]

struct VmMapping {
  u64 virt_addr_start;
  size_t length;
  std::vector<phys_page> page_numbers;

  void print() {
    fmt::println("virt_addr_start: {:#016x}", virt_addr_start);
    fmt::println("length: {:#016x}", length);
    fmt::println("page_numbers: {}", fmt::join(page_numbers, ","));
  }
};

struct VMM {
  VMM() = delete;
  VMM(u32 page_sz) {
    assert((page_size & (page_size - 1)) == 0);
    page_size = page_sz;
    vmm_logger->set_pattern("vmm: %v");
    vmm_logger->set_level(spdlog::level::off);
    vmm_logger->info("hello from VMM :-)");
  };

  phys_page walk_and_create(u64 virt_addr);
  phys_page get_next_free_page();

  std::shared_ptr<spdlog::logger> vmm_logger = spdlog::stdout_color_mt("VMM");
  u32 page_size{};

  // we'll give the emulator about 4gigs
  std::array<page_in_use, 4_GiB / PPC64_PAGE_SIZE> all_pages{};
  using ThirdLevelTable  = std::unordered_map<u16, u16>;
  using SecondLevelTable = std::unordered_map<u16, ThirdLevelTable*>;

  std::unordered_map<u16, SecondLevelTable*> first_level_table;  // holds pointers to the bases of a 2nd-level page tables, indexed into using first level
  std::unordered_map<u16, ThirdLevelTable*> second_level_table;  // holds pointers to the bases of a 3rd-level page tables, indexed into second level

  std::optional<VmMapping> map(u64 vaddr, u64 size);
  phys_page get_page_number(u64 virt_addr);

  std::unordered_map<virt_page, phys_page> page_table{};
  u64 virt_to_phys(u64 virt_addr);

  bool do_page_table_entry(u64 addr, u64 phys_page);
  std::vector<VmMapping> mappings;
};

inline VMM vmm{PPC64_PAGE_SIZE};
