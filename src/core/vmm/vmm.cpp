#include "vmm/vmm.hpp"

#include <memory>

#include "common.hpp"

phys_page VMM::get_page_number(u64 virt_addr) {
  // u16 page_offset      = virt_addr & 0xFFFF;
  vmm_logger->info("[gpn] addrof: {:#016x}", (u64)std::addressof(first_level_table));
  u16 first_level_idx            = (virt_addr >> 48) & 0xFFFF;  // index used in the 1st-level table to see if it points to a 2nd-level page table base
  u16 second_level_idx           = (virt_addr >> 32) & 0xFFFF;  // index used in the 2nd-level table to see if it points to a 3rd-level page table base
  u16 inner_page_table_entry_idx = (virt_addr >> 16) & 0xFFFF;  // index used in the 3rd-level table to see if it points to a physical page number

  vmm_logger->info("[gpn] first level entry idx: {:#010x}", first_level_idx);
  vmm_logger->info("[gpn] second_level level entry idx: {:#010x}", second_level_idx);
  vmm_logger->info("[gpn] inner_page_table_entry_idx: {:#010x}", inner_page_table_entry_idx);
  // vmm_logger->info("page offset: {:#010x}", page_offset);

  auto ptr_to_second_level_base_exists = first_level_table.contains(first_level_idx);  // 0x(FFFF)FFFFFFFF[FFFF]
  if (!ptr_to_second_level_base_exists) {
    vmm_logger->info("1st-level root does not have entry to 2nd-level inner ({})", first_level_idx);
    return UNMAPPED_PAGE;
  }
  auto _second_level_table = first_level_table.at(first_level_idx);

  auto ptr_to_second_level_page_table_exists = _second_level_table->contains(second_level_idx);  // 0xFFFF(FFFF)FFFF[FFFF]
  if (!ptr_to_second_level_page_table_exists) {
    vmm_logger->info("[gpn] 2nd-level inner {} does not have entry to 3rd-level leaf ({})", first_level_idx, second_level_idx);
    return UNMAPPED_PAGE;
  };
  // vmm_logger->info("HEREE?");
  auto inner_page_table = _second_level_table->at(second_level_idx);

  // vmm_logger->info("THERE???");
  auto is_mapped = inner_page_table->contains(inner_page_table_entry_idx);  // 0xFFFFFFFF(FFFF)[FFFF]

  if (!is_mapped) {
    return UNMAPPED_PAGE;
  };

  vmm_logger->info("premap");
  u64 phys_page_num = inner_page_table->at(inner_page_table_entry_idx);

  return phys_page_num;
}
phys_page VMM::get_next_free_page() {
  for (size_t page_n = 0; page_n < all_pages.size(); page_n++) {
    if (!all_pages.at(page_n)) {
      all_pages.at(page_n) = IN_USE;

      vmm_logger->info("found free page: {}", page_n);
      return page_n;
    }
  }

  FATAL("ran out of physical pages");

  // assert(0 && )
  return -1;
};
phys_page VMM::walk_and_create(u64 virt_addr) {
  // u16 page_offset                = virt_addr & 0xFFFF;
  vmm_logger->info("addrof: {:#016x}", (u64)std::addressof(first_level_table));

  u16 first_level_idx            = (virt_addr >> 48) & 0xFFFF;  // index used in the 1st-level table to see if it points to a 2nd-level page table base
  u16 second_level_idx           = (virt_addr >> 32) & 0xFFFF;  // index used in the 2nd-level table to see if it points to a 3rd-level page table base
  u16 inner_page_table_entry_idx = (virt_addr >> 16) & 0xFFFF;  // index used in the 3rd-level table to see if it points to a physical page number

  vmm_logger->info("first level entry idx: {:#010x}", first_level_idx);
  vmm_logger->info("second_level level entry idx: {:#010x}", second_level_idx);
  vmm_logger->info("third_level level entry idx: {:#010x}", inner_page_table_entry_idx);
  // vmm_logger->info("page offset: {:#010x}", page_offset);

  vmm_logger->info("sizeof: {}", first_level_table.size());

  auto ptr_to_second_level_table_exists = first_level_table.contains(first_level_idx);  // 0x(FFFF)FFFFFFFF[FFFF]
  if (!ptr_to_second_level_table_exists) {
    auto slt = new SecondLevelTable{};
    first_level_table.insert({first_level_idx, slt});
    vmm_logger->info("first_level_table x a: {}", first_level_table.size());
    vmm_logger->info("1st-level root does not have entry to 2nd-level inner ({}) -- creating", first_level_idx);
    // return UNMAPPED_PAGE;
  }
  auto second_level_table = first_level_table.at(first_level_idx);

  // vmm_logger->info("GOOD! second_level_table size: {}", second_level_table->size());
  // vmm_logger->info("second_level_idx: {:#010x}", second_level_idx);

  auto ptr_to_second_level_page_table_exists = second_level_table->contains(second_level_idx);  // 0xFFFF(FFFF)FFFF[FFFF]
  if (!ptr_to_second_level_page_table_exists) {
    auto tlt = new ThirdLevelTable{};
    // vmm_logger->info("2222lvlidx: {:#016x}", second_level_idx);
    second_level_table->insert({second_level_idx, tlt});
    vmm_logger->info("2nd-level inner {} does not have entry to inner page table ({}) -- creating", second_level_idx, inner_page_table_entry_idx);
    // return UNMAPPED_PAGE;
  };

  // auto inner_page_table = second_level_table->at(second_level_idx);

  vmm_logger->info("wac: {}", first_level_table.size());
  vmm_logger->info("walked and created");
  return 0x0;
};

u64 VMM::virt_to_phys(u64 virt_addr) {
  u64 page_aligned_virt_addr = virt_addr - (virt_addr % PPC64_PAGE_SIZE);

  u32 page_num = get_page_number(page_aligned_virt_addr);
  vmm_logger->info("pfn: {:#016x}", get_page_number(page_aligned_virt_addr));

  u64 phys_addr = (page_num * PPC64_PAGE_SIZE) + (virt_addr % PPC64_PAGE_SIZE);

  return phys_addr;
}

std::optional<VmMapping> VMM::map(u64 paddr, u64 size) {
  VmMapping mapping{};
  size_t diff = (paddr % PPC64_PAGE_SIZE);

  mapping.virt_addr_start = paddr - diff;

  vmm_logger->info("normalized address: {:#010x}", mapping.virt_addr_start);

  for (const auto& m : mappings) {
    if (m.virt_addr_start == mapping.virt_addr_start) {
      FATAL("overlapping memory mapping");
    }
  }

  mapping.length = size;

  u32 n_required_pages = (size / PPC64_PAGE_SIZE) + 1;

  for (size_t i = 0; i < n_required_pages; i++) {
    u16 page_num = get_next_free_page();
    mapping.page_numbers.push_back(page_num);
    do_page_table_entry(mapping.virt_addr_start + (PPC64_PAGE_SIZE * i), page_num);
  }

  mappings.push_back(mapping);

  return mapping;
}

bool VMM::do_page_table_entry(u64 virt_addr, u64 phys_page) {
  vmm_logger->info("\ndo_page_entry(virt_addr={:#016x}, phys_page={:#08x})", virt_addr, phys_page);
  u16 first_level_idx            = (virt_addr >> 48) & 0xFFFF;  // index used in the 1st-level table to see if it points to a 2nd-level page table base
  u16 second_level_idx           = (virt_addr >> 32) & 0xFFFF;  // index used in the 2nd-level table to see if it points to a 3rd-level page table base
  u16 inner_page_table_entry_idx = (virt_addr >> 16) & 0xFFFF;  // index used in the 3rd-level table to see if it points to a physical page number

  vmm_logger->info("first level index: {}", first_level_idx);
  vmm_logger->info("2nd level index: {}", second_level_idx);
  vmm_logger->info("ipt index: {}", inner_page_table_entry_idx);

  walk_and_create(virt_addr);

  auto second_lvl = first_level_table.at(first_level_idx);
  auto page_table = second_lvl->at(second_level_idx);
  // vmm_logger->info("first_lvl size: {}", first_lvl->size());
  // vmm_logger->info("second_lvl size: {}", second_lvl->size());

  vmm_logger->info("{:#016x} -> pfn: {}", virt_addr, phys_page);

  page_table->insert_or_assign(inner_page_table_entry_idx, phys_page);

  vmm_logger->info("finished page entry\n\n");

  return true;
}