#pragma once

#include <array>

#include "ppc/parser.hpp"
#include "ppe.hpp"
#include "spe.hpp"

struct Cell {
  PPE ppe{};
  std::array<SPE, 8> spe{};

  PPC::InstructionInfo current_instruction;
  void step();
};
