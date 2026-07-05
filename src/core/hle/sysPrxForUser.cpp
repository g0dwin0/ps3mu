#include "hle/sysPrxForUser.hpp"


void sys_initialize_tls(PPE& ppe) {
    fmt::println("sys_initialize_tls() = 0: stubbed");
    ppe.gpr[3] = 0;
}

void sys_time_get_system_time(PPE& ppe) {
    fmt::println("sys_time_get_system_time() = {}", std::time(0));
    ppe.gpr[3] = 0;
}


