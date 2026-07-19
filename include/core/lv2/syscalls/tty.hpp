#pragma once
#include "common.hpp"
#include "ppe.hpp"

int sys_tty_write(PPE &ppe, uint32_t ch, const void *buf, uint32_t len, uint32_t *pwritelen);