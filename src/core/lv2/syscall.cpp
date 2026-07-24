#include "lv2/syscalls/syscall.hpp"

#include "lv2/syscalls/vm.hpp"
#include "syscalls/tty.hpp"
#include "syscalls/vm.hpp"
#include "vmm.hpp"

void lv2_handle_syscall(PPE& ppe) {
  u16 syscall_id = ppe.gpr[11];

  switch (syscall_id) {
    case 0:
    case 1:
    case 2: {
      spdlog::info("R11: {:#016x}", syscall_id);

      if (syscall_id == 1) {
        spdlog::info("case failed: group: {}, case_count: {} expected: {:#016x} actual: {:#016x}", ppe.gpr[3], ppe.gpr[4], ppe.gpr[5], ppe.gpr[6]);

        // ppe.gpr[3] = 0x1000500;
      }
      if (syscall_id == 2) {
        spdlog::info("all groups done failure count: {}", ppe.gpr[3]);
      }
      break;
    }
    case SYS_MEMORY_GET_USER_MEMORY_SIZE: {
      spdlog::info("sys_mem_info_t @ {:#016x}", ppe.gpr[3]);
      auto mem = reinterpret_cast<sys_memory_info_t*>(ppe.bus->memory.data() + vmm.virt_to_phys(ppe.gpr[3]));

      ppe.gpr[3] = sys_memory_get_user_memory_size(mem);
      break;
    }

    case SYS_TTY_WRITE: {
      // GuestPtr<sys_memory_info_t> guestPtr(0x0);
      sys_tty_write(ppe, ppe.gpr[3], (void*)ppe.gpr[4], (u32)ppe.gpr[5], (uint32_t*)ppe.gpr[6]);

      ppe.gpr[3] = 0;
      break;
    }

    case SYS_PROCESS_GETPPID: {
      ppe.gpr[3] = 0x1000300;
      break;
    }
    case SYS_TIME_GET_TIMEBASE_FREQUENCY: {
      ppe.gpr[3] = 0x989680;
      break;
    }
    default: {
      FATAL(fmt::format("syscall {:#06x} unimplemented", syscall_id));
      break;
    }
  }
}