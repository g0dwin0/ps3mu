#include "common/defs.hpp"
#include "lv2/mutex.h"
#include "ppe.hpp"

void module_prologue(PPE& ppe) { ppe.bus->write<u64>(ppe.gpr[1] + 0x28, ppe.gpr[2]); }
void sys_initialize_tls(PPE& ppe) {
  module_prologue(ppe);

  spdlog::info("sys_initialize_tls() = 0: stubbed");

  ppe.gpr[3] = 0;
}

void sys_ppu_thread_get_id(PPE& ppe) {
  module_prologue(ppe);
  spdlog::info("[module] sys_ppu_thread_get_id() = 0x1000500: stubbed");
  ppe.bus->write<u64>(ppe.gpr[3], 0x1000500);

  ppe.gpr[3] = 0;
}

void sys_prx_register_library(PPE& ppe) {
  module_prologue(ppe);
  spdlog::info("[module] sys_prx_register_library() = 0: stubbed");
  ppe.gpr[3] = 0;
}

void sys_process_atexitspawn(PPE& ppe) {
  module_prologue(ppe);
  spdlog::info("[module] sys_process_atexitspawn() = {}", std::time(0));
  ppe.gpr[3] = 0;
}

void sys_process_at_Exitspawn(PPE& ppe) {
  module_prologue(ppe);
  spdlog::info("[module] sys_process_at_Exitspawn() = {}", std::time(0));
  ppe.gpr[3] = 0;
}

void sys_prx_exitspawn_with_level(PPE& ppe) {
  module_prologue(ppe);
  spdlog::info("[module] sys_prx_exitspawn_with_level() = {}", std::time(0));
  ppe.gpr[3] = 0;
}

void sys_time_get_system_time(PPE& ppe) {
  module_prologue(ppe);
  spdlog::info("[module] sys_time_get_system_time() = {}", std::time(0));
  ppe.gpr[3] = std::time(0);
}

void sys_lwmutex_lock(PPE& ppe) {
  module_prologue(ppe);
  spdlog::info("[module] sys_lwmutex_lock(): = {} -- stubbed", 0);
  ppe.gpr[3] = 0;
};
void sys_lwmutex_unlock(PPE& ppe) {
  module_prologue(ppe);
  spdlog::info("[module] sys_lwmutex_unlock(): = {} -- stubbed", 0);
  ppe.gpr[3] = 0;
};
void sys_lwmutex_create(PPE& ppe) {
  module_prologue(ppe);
  int retval = 0;

  sys_lwmutex_t* sys_lwmutex           = reinterpret_cast<sys_lwmutex_t*>(ppe.bus->memory.data() + vmm.virt_to_phys(ppe.gpr[3]));
  sys_lwmutex_attr_t* sys_lwmutex_attr = reinterpret_cast<sys_lwmutex_attr_t*>(ppe.bus->memory.data() + vmm.virt_to_phys(ppe.gpr[4]));

  sys_lwmutex->attribute = sys_lwmutex_attr->attr_protocol;

  spdlog::info("[module] sys_lwmutex_create(*mutex={:#016x}, *attr={:#016x}) = {}", vmm.virt_to_phys(ppe.gpr[3]), vmm.virt_to_phys(ppe.gpr[4]), retval);
  ppe.gpr[3] = retval;
};
void sys_lwmutex_destroy(PPE& ppe) {
  module_prologue(ppe);
  spdlog::info("[module] sys_lwmutex_destroy(): = {} -- stubbed", 0);
  ppe.gpr[3] = 0;
};
void sys_process_exit(PPE& ppe) {
  module_prologue(ppe);
  spdlog::info("[module] sys_process_exit(): = {} -- stubbed", 0);
  ppe.gpr[3] = 0;
};

void cellSysutilRegisterCallback(PPE& ppe) {
  module_prologue(ppe);
  spdlog::info("[module] cellSysutilRegisterCallback(): = {} -- stubbed", 0);
  ppe.gpr[3] = 0;
}
void cellSysutilCheckCallback(PPE& ppe) {
  module_prologue(ppe);
  spdlog::info("[module] cellSysutilCheckCallback(): = {} -- stubbed", 0);
  ppe.gpr[3] = 0;
}
