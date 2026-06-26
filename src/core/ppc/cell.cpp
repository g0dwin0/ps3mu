#include "ppc/cell.hpp"
#include "ppc/parser.hpp"


void Cell::step() {
// PPC::decode(this->ppe.nia, InstructionInfo &)
    ppe.step();
}