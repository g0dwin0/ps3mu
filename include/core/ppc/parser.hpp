#pragma once
#include "boost/endian/arithmetic.hpp"
#include "common/defs.hpp"

namespace PPC {

  enum class InstructionFormat {
    I_FORM,
    B_FORM,
    SC_FORM,
    D_FORM,
    DS_FORM,
    A_FORM,
    M_FORM,
    MD_FORM,
    MDS_FORM,
    X_FORM,
    XL_FORM,
    XFX_FORM,
    XFL_FORM,
    XS_FORM,
    XO_FORM,
    UNDEFINED
  };
  struct InstructionInfo {
    u8 opcode;
    u8 bo, bi, bd;
    bool aa, lk, lev;
    u8 rt, ra, ds, xo;
    u16 d, si, ui;
  };

  InstructionInfo decode(u32 op);
}  // namespace PPC
