#include "lv2/syscalls/syscall.hpp"
#include "syscalls/tty.hpp"
#include "vmm.hpp"
struct sys_memory_info_t
{
	BE<u32> total_user_memory;
	BE<u32> available_user_memory;
};

void lv2_handle_syscall(PPE & ppe) {
    u16 syscall_id = ppe.gpr[11];


    auto& retval = ppe.gpr[3];


    switch(syscall_id) {


        case 0x160: {
            fmt::println("R3: {:#016x}", ppe.gpr[3]);
            fmt::println("R4: {:#016x}", ppe.gpr[4]);
            fmt::println("R5: {:#016x}", ppe.gpr[5]);
            fmt::println("R6: {:#016x}", ppe.gpr[6]);
            fmt::println("R7: {:#016x}", ppe.gpr[7]);
            fmt::println("R8: {:#016x}", ppe.gpr[8]);
            fmt::println("R9: {:#016x}", ppe.gpr[9]);

            // fmt::println("sys_mem_info_t @ {:#016x}", ppe.gpr[3]);
            auto mem = (sys_memory_info_t*)ppe.bus->memory.data() + vmm.virt_to_phys(ppe.gpr[3]);

            mem->total_user_memory = PPC64_PAGE_SIZE * 1024;
            mem->available_user_memory = PPC64_PAGE_SIZE * 512;
            
            retval = 0;
            break;
        }

        case 0x193: {
            sys_tty_write(ppe, ppe.gpr[3], (void*)ppe.gpr[4], (u32)ppe.gpr[5], (uint32_t*)ppe.gpr[6]);
            break;
        }


        default: {
            FATAL(fmt::format("syscall {:#06x} unimplemented", syscall_id));
            break;
        }
    }
}