#pragma once
#include <string>
#include <unordered_map>

#include "common/defs.hpp"
#include "ppc/parser.hpp"
#include "ppe.hpp"

static constexpr u8 TDI               = 0x02;  
static constexpr u8 TWI               = 0x03;  
static constexpr u8 MULLI             = 0x07;  
static constexpr u8 SUBFIC            = 0x08;  
static constexpr u8 DOZI              = 0x09;  
static constexpr u8 CMPLI             = 0x0A;  
static constexpr u8 CMPI              = 0x0B;  
static constexpr u8 ADDIC             = 0x0C;  
// static constexpr u8 ADDIC             = 0x0D;  
static constexpr u8 ADDI              = 0x0E;  
static constexpr u8 ADDIS             = 0x0F;  
static constexpr u8 BC                = 0x10;  
static constexpr u8 SC                = 0x11;  
static constexpr u8 B                 = 0x12;  
static constexpr u8 CR                = 0x13;  
static constexpr u8 RLWIMI            = 0x14;  
static constexpr u8 RLWINM            = 0x15;  
static constexpr u8 RLMI              = 0x16;  
static constexpr u8 RLWNM             = 0x17;  
static constexpr u8 ORI               = 0x18;  
static constexpr u8 ORIS              = 0x19;  
static constexpr u8 XORI              = 0x1A;  
static constexpr u8 XORIS             = 0x1B;  
static constexpr u8 ANDI              = 0x1C;  
static constexpr u8 ANDIS             = 0x1D;  
static constexpr u8 FX_DWD_ROT        = 0x1E;  
static constexpr u8 FX_EXT_OPS        = 0x1F;  
static constexpr u8 LWZ               = 0x20;  
static constexpr u8 LWZU              = 0x21;  
static constexpr u8 LBZ               = 0x22;  
static constexpr u8 LBZU              = 0x23;  
static constexpr u8 STW               = 0x24;  
static constexpr u8 STWU              = 0x25;  
static constexpr u8 STB               = 0x26;  
static constexpr u8 STBU              = 0x27;  
static constexpr u8 LHZ               = 0x28;  
static constexpr u8 LHZU              = 0x29;  
static constexpr u8 LHA               = 0x2A;  
static constexpr u8 LHAU              = 0x2B;  
static constexpr u8 STH               = 0x2C;  
static constexpr u8 STHU              = 0x2D;  
static constexpr u8 LMW               = 0x2E;  
static constexpr u8 STMW              = 0x2F;  
static constexpr u8 LFS               = 0x30;  
static constexpr u8 LFSU              = 0x31;  
static constexpr u8 LFD               = 0x32;  
static constexpr u8 LFDU              = 0x33;  
static constexpr u8 STFS              = 0x34;  
static constexpr u8 STFSU             = 0x35;  
static constexpr u8 STFD              = 0x36;  
static constexpr u8 STFDU             = 0x37;  
static constexpr u8 FX_DS_FORM_LOADS  = 0x3A;  
static constexpr u8 FP_SINGLE_EXT_OPS = 0x3B;  
static constexpr u8 FX_DS_FORM_STORES = 0x3E;  
static constexpr u8 FP_DOUBLE_EXT_OPS = 0x3F;  

namespace PPC {

  struct OpcodeInfo {
    std::string mnemonic;
    InstructionFormat instruction_format;
  };

  const std::unordered_map<u8, OpcodeInfo> opcode_info_map = {
      {0x02,                {"tdi", InstructionFormat::D_FORM}},
      {0x03,                {"twi", InstructionFormat::D_FORM}},
      {0x07,              {"mulli", InstructionFormat::D_FORM}},
      {0x08,             {"subfic", InstructionFormat::D_FORM}},
      {0x09,               {"dozi", InstructionFormat::D_FORM}},
      {0x0A,              {"cmpli", InstructionFormat::D_FORM}},
      {0x0B,               {"cmpi", InstructionFormat::D_FORM}},
      {0x0C,              {"addic", InstructionFormat::D_FORM}},
      {0x0D,             {"addic.", InstructionFormat::D_FORM}},
      {0x0E,               {"addi", InstructionFormat::D_FORM}},
      {0x0F,              {"addis", InstructionFormat::D_FORM}},
      {0x10,                 {"bc", InstructionFormat::B_FORM}},
      {0x11,                {"sc", InstructionFormat::SC_FORM}},
      {0x12,                  {"b", InstructionFormat::I_FORM}},
      {0x13,      {"CR ops, etc.", InstructionFormat::XL_FORM}},
      {0x14,             {"rlwimi", InstructionFormat::M_FORM}},
      {0x15,             {"rlwinm", InstructionFormat::M_FORM}},
      {0x16,               {"rlmi", InstructionFormat::M_FORM}},
      {0x17,              {"rlwnm", InstructionFormat::M_FORM}},
      {0x18,                {"ori", InstructionFormat::D_FORM}},
      {0x19,               {"oris", InstructionFormat::D_FORM}},
      {0x1A,               {"xori", InstructionFormat::D_FORM}},
      {0x1B,              {"xoris", InstructionFormat::D_FORM}},
      {0x1C,              {"andi.", InstructionFormat::D_FORM}},
      {0x1D,             {"andis.", InstructionFormat::D_FORM}},
      {0x1E,       {"FX_DWD_ROT", InstructionFormat::MDS_FORM}},
      {0x1F,         {"FX_EXT_OPS", InstructionFormat::UNDEFINED}},
      {0x20,                {"lwz", InstructionFormat::D_FORM}},
      {0x21,            {"lwzu", InstructionFormat::UNDEFINED}},
      {0x22,                {"lbz", InstructionFormat::D_FORM}},
      {0x23,               {"lbzu", InstructionFormat::D_FORM}},
      {0x24,             {"stw", InstructionFormat::UNDEFINED}},
      {0x25,            {"stwu", InstructionFormat::UNDEFINED}},
      {0x26,             {"stb", InstructionFormat::UNDEFINED}},
      {0x27,               {"stbu", InstructionFormat::D_FORM}},
      {0x28,                {"lhz", InstructionFormat::D_FORM}},
      {0x29,               {"lhzu", InstructionFormat::D_FORM}},
      {0x2A,                {"lha", InstructionFormat::D_FORM}},
      {0x2B,               {"lhau", InstructionFormat::D_FORM}},
      {0x2C,                {"sth", InstructionFormat::D_FORM}},
      {0x2D,               {"sthu", InstructionFormat::D_FORM}},
      {0x2E,                {"lmw", InstructionFormat::D_FORM}},
      {0x2F,               {"stmw", InstructionFormat::D_FORM}},
      {0x30,                {"lfs", InstructionFormat::D_FORM}},
      {0x31,               {"lfsu", InstructionFormat::D_FORM}},
      {0x32,                {"lfd", InstructionFormat::D_FORM}},
      {0x33,               {"lfdu", InstructionFormat::D_FORM}},
      {0x34,               {"stfs", InstructionFormat::D_FORM}},
      {0x35,              {"stfsu", InstructionFormat::D_FORM}},
      {0x36,               {"stfd", InstructionFormat::D_FORM}},
      {0x37,              {"stfdu", InstructionFormat::D_FORM}},
      {0x3A,  {"FX_DS_FORM_LOADS", InstructionFormat::DS_FORM}},
      {0x3B,  {"FP_SINGLE_EXT_OPS", InstructionFormat::A_FORM}},
      {0x3E, {"FX_DS_FORM_STORES", InstructionFormat::DS_FORM}},
      {0x3F,  {"FP_DOUBLE_EXT_OPS", InstructionFormat::D_FORM}},
  };



  const std::unordered_map<u16, OpcodeInfo> ext_op_31_opcode_info_map = {
      {0x0,                {"cmp", InstructionFormat::X_FORM}},
      {138,                {"add", InstructionFormat::XO_FORM}},
      {266,                {"add", InstructionFormat::XO_FORM}},
      
  };

  void tdi(PPC::InstructionInfo&, PPE& ppe);
  void twi(PPC::InstructionInfo&, PPE& ppe);
  void mulli(PPC::InstructionInfo&, PPE& ppe);
  void subfic(PPC::InstructionInfo&, PPE& ppe);
  void dozi(PPC::InstructionInfo&, PPE& ppe);
  void cmpli(PPC::InstructionInfo&, PPE& ppe);
  void cmpi(PPC::InstructionInfo&, PPE& ppe);
  void addic(PPC::InstructionInfo&, PPE& ppe);
  void addi(PPC::InstructionInfo&, PPE& ppe);
  void bc(PPC::InstructionInfo&, PPE& ppe);
  void sc(PPC::InstructionInfo&, PPE& ppe);
  void b(PPC::InstructionInfo&, PPE& ppe);
  void rlwimi(PPC::InstructionInfo&, PPE& ppe);
  void rlwinm(PPC::InstructionInfo&, PPE& ppe);
  void ori(PPC::InstructionInfo&, PPE& ppe);
  void oris(PPC::InstructionInfo&, PPE& ppe);
  void xori(PPC::InstructionInfo&, PPE& ppe);
  void xoris(PPC::InstructionInfo&, PPE& ppe);
  void andi(PPC::InstructionInfo&, PPE& ppe);
  void andis(PPC::InstructionInfo&, PPE& ppe);
  void lwz(PPC::InstructionInfo&, PPE& ppe);
  void gen_load(PPC::InstructionInfo&, PPE& ppe);
  void gen_store(PPC::InstructionInfo&, PPE& ppe);
  

   template <typename RetValWidth, typename InputWidth>
  RetValWidth get_bits(u32 n, std::pair<u8, u8> be_range) {
    const auto& [start, end] = be_range;

    assert(start < end);

    n = (n >> ((sizeof(InputWidth) * 8) - end - 1));

    u16 mask = (1 << (end + 1 - start)) - 1;

    if (start != 0) {
      n &= mask;
    }

    // fmt::println("retval: {}\n", (RetValWidth)n);

    return n;
  }
}  // namespace PPC