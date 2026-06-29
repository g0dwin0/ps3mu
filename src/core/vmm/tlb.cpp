#include "vmm/tlb.hpp"

void TLB::evict(Tag tag) { tlb.erase(tag); };

void TLB::map() {
  if (tlb.size() == TLB::MAX_TLB_SIZE) evict(0);
}
phys_page_t TLB::read(u64 address) {
  Tag tag               = address / PPC64_PAGE_SIZE;
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