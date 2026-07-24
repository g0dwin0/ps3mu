#include "lv2/syscalls/vm.hpp"

int sys_memory_get_user_memory_size(sys_memory_info_t *mem_info) {
  mem_info->total_user_memory     = 0X100000;
  mem_info->available_user_memory = 0X100000;

  return 0;
}