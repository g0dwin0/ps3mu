#pragma once
#include "../../core/ppe.hpp"
// for module-level emulation... probably won't use this.
void sys_initialize_tls(PPE&);
void sys_time_get_system_time(PPE&);
void sys_initialize_tls(PPE& ppe);
void sys_ppu_thread_get_id(PPE& ppe);
void sys_prx_register_library(PPE& ppe);
void sys_process_atexitspawn(PPE& ppe);
void sys_process_at_Exitspawn(PPE& ppe);
void sys_prx_exitspawn_with_level(PPE& ppe);
void cellSysutilRegisterCallback(PPE& ppe);
void cellSysutilCheckCallback(PPE& ppe);
void sys_lwmutex_lock(PPE& ppe);
void sys_lwmutex_unlock(PPE& ppe);
void sys_lwmutex_create(PPE& ppe);
void sys_lwmutex_destroy(PPE& ppe);
void sys_process_exit(PPE& ppe);