#pragma once
#include "ppe.hpp"

static constexpr u16 SYS_MEMORY_GET_USER_MEMORY_SIZE = 0x160;
static constexpr u16 SYS_TTY_WRITE                   = 0x193;
static constexpr u16 SYS_PROCESS_GETPPID             = 0x012;
static constexpr u16 SYS_TIME_GET_TIMEBASE_FREQUENCY = 0x093;

void lv2_handle_syscall(PPE&);