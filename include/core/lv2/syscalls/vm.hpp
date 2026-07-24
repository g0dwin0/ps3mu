#pragma once
#include "common.hpp"

struct sys_memory_info_t {
  BE<u32> total_user_memory;
  BE<u32> available_user_memory;
};

int sys_memory_get_user_memory_size(sys_memory_info_t* mem_info);