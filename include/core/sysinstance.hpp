#pragma once
#include "bus.hpp"
#include "ppc/cell.hpp"

struct SysInstance {
  SysInstance() { cell_be.ppe.bus = &bus; };
  Cell cell_be;
  Bus bus;
};
