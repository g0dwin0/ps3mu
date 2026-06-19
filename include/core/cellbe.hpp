#pragma once

#include <array>

#include "ppe.hpp"
#include "spe.hpp"

struct CellBE {
  PPE ppe;
  std::array<SPE, 8> spe;
};
