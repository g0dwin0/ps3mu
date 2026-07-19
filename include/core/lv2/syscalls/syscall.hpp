#pragma once
#include "ppe.hpp"

static constexpr u16 SYS_MEMORY_GET_USER_MEMORY_SIZE = 0x160;
static constexpr u16 SYS_TTY_WRITE                   = 0x193;

void lv2_handle_syscall(PPE&);