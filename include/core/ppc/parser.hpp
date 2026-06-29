#pragma once
#include <string>
#include <unordered_map>

#include "common/defs.hpp"

namespace PPC {

  enum class InstructionFormat { I_FORM, B_FORM, SC_FORM, D_FORM, DS_FORM, A_FORM, M_FORM, MD_FORM, MDS_FORM, X_FORM, XL_FORM, XFX_FORM, XFL_FORM, XS_FORM, XO_FORM, UNDEFINED };

  const std::unordered_map<InstructionFormat, std::string> instruction_format_str_map = {

      {   InstructionFormat::I_FORM,    "I_FORM"},
      {   InstructionFormat::B_FORM,    "B_FORM"},
      {  InstructionFormat::SC_FORM,   "SC_FORM"},
      {   InstructionFormat::D_FORM,    "D_FORM"},
      {  InstructionFormat::DS_FORM,   "DS_FORM"},
      {   InstructionFormat::A_FORM,    "A_FORM"},
      {   InstructionFormat::M_FORM,    "M_FORM"},
      {  InstructionFormat::MD_FORM,   "MD_FORM"},
      { InstructionFormat::MDS_FORM,  "MDS_FORM"},
      {   InstructionFormat::X_FORM,    "X_FORM"},
      {  InstructionFormat::XL_FORM,   "XL_FORM"},
      { InstructionFormat::XFX_FORM,  "XFX_FORM"},
      { InstructionFormat::XFL_FORM,  "XFL_FORM"},
      {  InstructionFormat::XS_FORM,   "XS_FORM"},
      {  InstructionFormat::XO_FORM,   "XO_FORM"},
      {InstructionFormat::UNDEFINED, "UNDEFINED"},
  };
  struct InstructionInfo {
    u8 opcode;
    u8 bo, bi, bd;
    bool aa, lk, lev, rc;
    u8 rt, ra, rb, oe, rs, xo, bf, l, frt, frs, to;
    u16 d, si, ui;
    i16 ds;
    u64 li;
    std::string mnemnonic;
  };

  void decode(u32 op, InstructionInfo&);
}  // namespace PPC
