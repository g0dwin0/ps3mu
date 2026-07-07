#include "lv2/syscalls/tty.hpp"

#include "vmm.hpp"
int sys_tty_write(PPE& ppe, uint32_t ch, const void* buf, uint32_t len, uint32_t* written) {
  fmt::println("ch: {}", ch);
  fmt::println("buf: {}", fmt::ptr(buf));
  fmt::println("len: {}", len);
  fmt::println("written: {}", fmt::ptr(written));

  fmt::println("{}(ch={}, buf={}, len={}, written={})", __func__, ch, fmt::ptr(buf), len, fmt::ptr(written));

  u64 phys_addr = vmm.virt_to_phys((u64)buf);

  const char* ptr = (const char*)ppe.bus->memory.data() + phys_addr;

//   std::fwrite(ptr, 1, len, stdout);


  for (size_t i = 0; i < len; ++i)
    printf("%02X ", (unsigned char)ptr[i]);

//  fmt::print("{}", fmt::string_view(ptr, len));
//   FATAL("check!");
  return 0;
}
