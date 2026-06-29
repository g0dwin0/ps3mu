#include "vmm/vmm.hpp"

phys_page VMM::get_page_number(u64 virt_addr) {
  u16 page_offset      = virt_addr & 0xFFFF;
  u16 first_level_idx  = (virt_addr >> 48) & 0xFFFF;  // index used in the 1st-level table to see if it points to a 2nd-level page table base
  u16 second_level_idx = (virt_addr >> 32) & 0xFFFF;  // index used in the 2nd-level table to see if it points to a 3rd-level page table base
  u16 third_level      = (virt_addr >> 16) & 0xFFFF;  // index used in the 3rd-level table to see if it points to a physical page number

  fmt::println("first level entry idx: {:#010x}", first_level_idx);
  fmt::println("second_level level entry idx: {:#010x}", second_level_idx);
  fmt::println("third_level level entry idx: {:#010x}", third_level);
  fmt::println("page offset: {:#010x}", page_offset);

  auto ptr_to_second_level_base_exists = first_level_table_base.contains(first_level_idx);  // 0x(FFFF)FFFFFFFF[FFFF]
  if (!ptr_to_second_level_base_exists) {
    fmt::println("[VMM] 1st-level root does not have entry to 2nd-level inner ({})", first_level_idx);
    return UNMAPPED_PAGE;
  }
  auto second_level_base = first_level_table_base[first_level_idx];

  auto ptr_to_second_level_page_table_exists = second_level_base->contains(second_level_idx);  // 0xFFFF(FFFF)FFFF[FFFF]
  if (!ptr_to_second_level_page_table_exists) {
    fmt::println("2nd-level inner {} does not have entry to 3rd-level leaf ({})", first_level_idx, second_level_idx);
    return UNMAPPED_PAGE;
  };
  auto third_level_leaf = second_level_table_base[second_level_idx];

  auto ptr_to_third_level_pt_exists = third_level_leaf->contains(third_level);  // 0xFFFFFFFF(FFFF)[FFFF]
  if (!ptr_to_third_level_pt_exists) {
    return UNMAPPED_PAGE;
  };
  auto third_level_pt = third_level_leaf[third_level];

  auto phys_page_num = third_level_pt[third_level];

  return phys_page_num;
}

u64 VMM::VIRT_TO_PHYS(u64 virt_addr) {
  // u64 effective_addr = virt_addr

  return virt_addr;
}