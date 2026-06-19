#pragma once
#include <string>
#include <unordered_map>

#include "common/defs.hpp"
#include "ppc/parser.hpp"
namespace PPC {

  struct OpcodeInfo {
    std::string mnemonic;
    InstructionFormat instruction_format;
  };

  const std::unordered_map<u8, OpcodeInfo> opcode_info_map = {


      {0x02,           {"tdi", InstructionFormat::D_FORM}},
      {0x03,           {"twi", InstructionFormat::D_FORM}},

      {0x07,         {"mulli", InstructionFormat::D_FORM}},
      {0x08,        {"subfic", InstructionFormat::D_FORM}},
      {0x09,          {"dozi", InstructionFormat::D_FORM}},
      {0x0A,         {"cmpli", InstructionFormat::D_FORM}},
      {0x0B,          {"cmpi", InstructionFormat::D_FORM}},
      {0x0C,         {"addic", InstructionFormat::D_FORM}},
      {0x0D,        {"addic.", InstructionFormat::D_FORM}},
      {0x0E,          {"addi", InstructionFormat::D_FORM}},
      {0x0F,         {"addis", InstructionFormat::D_FORM}},
      {0x10,            {"bc", InstructionFormat::B_FORM}},
      {0x11,           {"sc", InstructionFormat::SC_FORM}},
      {0x12,             {"b", InstructionFormat::I_FORM}},
      {0x13, {"CR ops, etc.", InstructionFormat::XL_FORM}},

      {0x14,        {"rlwimi", InstructionFormat::M_FORM}},
      {0x15,        {"rlwinm", InstructionFormat::M_FORM}},
      {0x16,          {"rlmi", InstructionFormat::M_FORM}},
      {0x17,         {"rlwnm", InstructionFormat::M_FORM}},
      {0x18,           {"ori", InstructionFormat::D_FORM}},
      {0x19,          {"oris", InstructionFormat::D_FORM}},
      {0x1A,          {"xori", InstructionFormat::D_FORM}},
      {0x1B,         {"xoris", InstructionFormat::D_FORM}},
      {0x1C,         {"andi.", InstructionFormat::D_FORM}},
      {0x1D,        {"andis.", InstructionFormat::D_FORM}},
      {0x1E,  {"FX_DWD_ROT", InstructionFormat::MDS_FORM}},
      {0x1F,    {"FX_EXT_OPS", InstructionFormat::M_FORM}},

      {0x20,           {"lwz", InstructionFormat::D_FORM}},
      {0x21,       {"lwzu", InstructionFormat::UNDEFINED}},
      {0x22,           {"lbz", InstructionFormat::D_FORM}},
      {0x23,          {"lbzu", InstructionFormat::D_FORM}},
      {0x24,        {"stw", InstructionFormat::UNDEFINED}},
      {0x25,       {"stwu", InstructionFormat::UNDEFINED}},
      {0x26,        {"stb", InstructionFormat::UNDEFINED}},
      {0x27,          {"stbu", InstructionFormat::D_FORM}},
      {0x28,           {"lhz", InstructionFormat::D_FORM}},
      {0x29,          {"lhzu", InstructionFormat::D_FORM}},
      {0x2A,           {"lha", InstructionFormat::D_FORM}},
      {0x2B,          {"lhau", InstructionFormat::D_FORM}},
      {0x2C,           {"sth", InstructionFormat::D_FORM}},
      {0x2D,          {"sthu", InstructionFormat::D_FORM}},
      {0x2E,           {"lmw", InstructionFormat::D_FORM}},
      {0x2F,          {"stmw", InstructionFormat::D_FORM}},

      {0x30,           {"lfs", InstructionFormat::D_FORM}},
      {0x31,          {"lfsu", InstructionFormat::D_FORM}},
      {0x32,           {"lfd", InstructionFormat::D_FORM}},
      {0x33,          {"lfdu", InstructionFormat::D_FORM}},
      {0x34,          {"stfs", InstructionFormat::D_FORM}},
      {0x35,         {"stfsu", InstructionFormat::D_FORM}},
      {0x36,          {"stfd", InstructionFormat::D_FORM}},
      {0x37,         {"stfdu", InstructionFormat::D_FORM}},

      {0x3A,          {"FX_DS_FORM_LOADS", InstructionFormat::DS_FORM}},
      {0x3B,         {"FP_SINGLE_EXT_OPS", InstructionFormat::A_FORM}},
      {0x3E,          {"FX_DS_FORM_STORES", InstructionFormat::DS_FORM}},
      {0x3F,         {"FP_DOUBLE_EXT_OPS", InstructionFormat::D_FORM}},     
  };
}  // namespace PPC