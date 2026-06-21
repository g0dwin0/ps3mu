#pragma once

#include <array>

#include "ppe.hpp"
#include "spe.hpp"

struct Cell {
  PPE ppe;
  std::array<SPE, 8> spe;

  void step();
};
