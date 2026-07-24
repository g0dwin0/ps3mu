#include "lv2/syscalls/tty.hpp"

#include "vmm.hpp"
int sys_tty_write(PPE& ppe, uint32_t ch, const void* buf, uint32_t len, uint32_t* written) {
  spdlog::info("ch: {}", ch);
  spdlog::info("buf: {}", fmt::ptr(buf));
  spdlog::info("len: {}", len);
  spdlog::info("written: {}", fmt::ptr(written));

  spdlog::info("{}(ch={}, buf={}, len={}, written={})", __func__, ch, fmt::ptr(buf), len, fmt::ptr(written));

  u64 phys_addr = vmm.virt_to_phys((u64)buf);

  const char* ptr = (const char*)ppe.bus->memory.data() + phys_addr;

  u32* _written = (u32*)ppe.bus->memory.data() + vmm.virt_to_phys((u64)written);

  spdlog::info("r: {}", ptr);
  *_written = len;
  ppe.logs += ptr;

  return 0;
}
