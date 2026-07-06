#include "hle/sysPrxForUser.hpp"

void sys_initialize_tls(PPE& ppe) {
  fmt::println("sys_initialize_tls() = 0: stubbed");
  ppe.gpr[3] = 0;
}

void sys_ppu_thread_get_id(PPE& ppe) {
  fmt::println("sys_ppu_thread_get_id() = 0x0B00B135: stubbed");
  ppe.gpr[3] = 0x0B00B135;
}

void sys_prx_register_library(PPE& ppe) {
  fmt::println("sys_prx_register_library() = 0: stubbed");
  ppe.gpr[3] = 0;
}

void sys_process_atexitspawn(PPE& ppe) {
  fmt::println("sys_process_atexitspawn() = {}", std::time(0));
  ppe.gpr[3] = 0;
}

void sys_process_at_Exitspawn(PPE& ppe) {
  fmt::println("sys_process_at_Exitspawn() = {}", std::time(0));
  ppe.gpr[3] = 0;
}

void sys_prx_exitspawn_with_level(PPE& ppe) {
  fmt::println("sys_prx_exitspawn_with_level() = {}", std::time(0));
  ppe.gpr[3] = 0;
}

void sys_time_get_system_time(PPE& ppe) {
  fmt::println("sys_time_get_system_time() = {}", std::time(0));
  ppe.gpr[3] = std::time(0);
}

void sys_lwmutex_lock(PPE& ppe) {
    fmt::println("sys_lwmutex_lock(): = {} -- stubbed", 0);
    ppe.gpr[3] = 0;
};
void sys_lwmutex_unlock(PPE& ppe) {
    fmt::println("sys_lwmutex_unlock(): = {} -- stubbed", 0);
    ppe.gpr[3] = 0;
};
void sys_lwmutex_create(PPE& ppe) {
    fmt::println("sys_lwmutex_create(): = {} -- stubbed", 0);
    ppe.gpr[3] = 0;
};
void sys_lwmutex_destroy(PPE& ppe) {
    fmt::println("sys_lwmutex_destroy(): = {} -- stubbed", 0);
    ppe.gpr[3] = 0;
};
void sys_process_exit(PPE& ppe) {
    fmt::println("sys_process_exit(): = {} -- stubbed", 0);
    ppe.gpr[3] = 0;
};

