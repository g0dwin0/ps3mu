#pragma once
#include <string>
#include <unordered_map>

#include "common/defs.hpp"
#include "ppc/parser.hpp"
#include "ppe.hpp"
static constexpr u8 STORE_DOUBLEWORD = 0x3E;
static constexpr u16 MTOCRF          = 144;
static constexpr u16 SUBF            = 40;
static constexpr u8 TDI              = 0x02;
static constexpr u8 TWI              = 0x03;
static constexpr u8 MULLI            = 0x07;
static constexpr u8 SUBFIC           = 0x08;
static constexpr u8 DOZI             = 0x09;
static constexpr u8 CMPLI            = 0x0A;
static constexpr u8 CMPI             = 0x0B;
static constexpr u8 ADDIC            = 0x0C;
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
static constexpr u16 NOR              = 124;
static constexpr u16 CNTLZD           = 58;
static constexpr u16 CNTLZW           = 26;
static constexpr u16 MFSPR            = 339;
static constexpr u16 MTSPR            = 467;
static constexpr u16 _OR              = 444;
static constexpr u16 EXTSH            = 922;
static constexpr u16 EXTSW            = 986;
static constexpr u16 STDU             = 62;

static constexpr u16 SRW = 536;
static constexpr u16 SRD = 539;

static constexpr u16 SLW = 24;
static constexpr u16 SLD = 27;

enum : u8 { CR_LT = 0x8, CR_GT = 0x4, CR_EQ = 0x2, CR_SO = 0x1 };

namespace PPC {

  struct OpcodeInfo {
    std::string mnemonic;
    InstructionFormat instruction_format;
  };

  const std::unordered_map<u8, std::string> branch_form_type_table = {
      {0x0,   "b"},
      {0x1,  "bl"},
      {0x2,  "ba"},
      {0x3, "bla"},
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
      {0x1F,      {"FX_EXT_OPS", InstructionFormat::UNDEFINED}},
      {0x20,                {"lwz", InstructionFormat::D_FORM}},
      {0x21,               {"lwzu", InstructionFormat::D_FORM}},
      {0x22,                {"lbz", InstructionFormat::D_FORM}},
      {0x23,               {"lbzu", InstructionFormat::D_FORM}},
      {0x24,                {"stw", InstructionFormat::D_FORM}},
      {0x25,               {"stwu", InstructionFormat::D_FORM}},
      {0x26,                {"stb", InstructionFormat::D_FORM}},
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

  const std::unordered_map<u16, OpcodeInfo> fx_ext_ops_map = {
      {   0x0,      {"cmp", InstructionFormat::X_FORM}},
      {    32,     {"cmpl", InstructionFormat::X_FORM}},
      {  SUBF,    {"subf", InstructionFormat::XO_FORM}},
      // {   138,     {"adde", InstructionFormat::XO_FORM}},
      {   266,     {"add", InstructionFormat::XO_FORM}},
      {    19,   {"mfcr", InstructionFormat::XFX_FORM}},
      {    28,      {"and", InstructionFormat::X_FORM}},
      {   316,      {"xor", InstructionFormat::X_FORM}},
      {   104,     {"neg", InstructionFormat::XO_FORM}},
      {   278,     {"dcbt", InstructionFormat::X_FORM}},
      {   124,      {"nor", InstructionFormat::X_FORM}},
      {CNTLZD,   {"cntlzd", InstructionFormat::X_FORM}},
      {CNTLZW,   {"cntlzw", InstructionFormat::X_FORM}},

      {   536,      {"srw", InstructionFormat::X_FORM}},
      {   539,      {"srd", InstructionFormat::X_FORM}},

      {    27,      {"sld", InstructionFormat::X_FORM}},
      {    24,      {"slw", InstructionFormat::X_FORM}},

      {MTOCRF, {"mtocrf", InstructionFormat::XFX_FORM}},
      { MTSPR,  {"mtspr", InstructionFormat::XFX_FORM}},
      { MFSPR,  {"mfspr", InstructionFormat::XFX_FORM}},
      {   _OR,       {"or", InstructionFormat::X_FORM}},
      { EXTSW,    {"extsw", InstructionFormat::X_FORM}},
      { EXTSH,    {"extsh", InstructionFormat::X_FORM}},
  };

  void mulli(InstructionInfo&, PPE& ppe);
  void subfic(InstructionInfo&, PPE& ppe);
  void dozi(InstructionInfo&, PPE& ppe);
  void cmpli(InstructionInfo&, PPE& ppe);
  void cmpi(InstructionInfo&, PPE& ppe);
  void cmp(InstructionInfo&, PPE& ppe);

  void addic(InstructionInfo&, PPE& ppe);
  void branch(InstructionInfo&, PPE& ppe);
  void bclr(InstructionInfo&, PPE& ppe);
  void bcctr(InstructionInfo&, PPE& ppe);
  void mtocrf(InstructionInfo& info, PPE& ppe);
  void AND(InstructionInfo& info, PPE& ppe);
  void XOR(InstructionInfo& info, PPE& ppe);
  void NEG(InstructionInfo& info, PPE& ppe);

  void branch_conditional(InstructionInfo&, PPE& ppe);

  void addi(InstructionInfo&, PPE& ppe);
  void addis(InstructionInfo&, PPE& ppe);

  void bc(InstructionInfo&, PPE& ppe);
  void sc(InstructionInfo&, PPE& ppe);
  void b(InstructionInfo&, PPE& ppe);
  void rlwimi(InstructionInfo&, PPE& ppe);
  void rlwinm(InstructionInfo&, PPE& ppe);
  void OR(InstructionInfo&, PPE& ppe);
  void ori(InstructionInfo&, PPE& ppe);
  void oris(InstructionInfo&, PPE& ppe);
  void xori(InstructionInfo&, PPE& ppe);
  void xoris(InstructionInfo&, PPE& ppe);
  void andi(InstructionInfo&, PPE& ppe);
  void andis(InstructionInfo&, PPE& ppe);
  void mtspr(InstructionInfo&, PPE& ppe);
  void mfspr(InstructionInfo&, PPE& ppe);
  void mfcr(InstructionInfo&, PPE& ppe);
  void extsw(InstructionInfo& info, PPE& ppe);
  void extsh(InstructionInfo& info, PPE& ppe);
  void add(InstructionInfo& info, PPE& ppe);
  void NOR(InstructionInfo& info, PPE& ppe);
  void subf(InstructionInfo& info, PPE& ppe);

  template <AccessWidth Size>
  void CNTLZx(InstructionInfo& info, PPE& ppe) {
    ppe.set_reg(info.ra, std::countl_zero<Size>(ppe.get_reg(info.rs)));

    if (info.rc) FATAL();
  }

  template <AccessWidth Size, bool Update, bool SignExtRT>
  void gen_load(InstructionInfo& info, PPE& ppe) {
    u64 b  = 0;
    u64 ea = 0;

    if constexpr (Update) {
      b = ppe.gpr.at(info.ra);
    } else {
      if (info.ra != 0) {
        b = ppe.gpr.at(info.ra);
      }
    }

    if constexpr (sizeof(Size) == 8 || (SignExtRT && sizeof(Size) == 4)) {
      ea = b + EXTS(15, info.ds << 2);
    } else {
      ea = b + EXTS(15, info.d);
    }

    fmt::println("EA = {:#016x}", ea);
    u64 result          = ppe.bus->read<Size>(ea);
    ppe.gpr.at(info.rt) = SignExtRT ? EXTS((sizeof(Size) * 8) - 1, result) : result;

    if (Update) {
      ppe.set_reg(info.ra, ea);
    }
  }

  template <AccessWidth Size, bool Update>
  void gen_store(InstructionInfo& info, PPE& ppe) {
    u64 b = 0;

    if constexpr (Update) {
      b = ppe.get_reg(info.ra);
    } else {
      b = info.ra != 0 ? ppe.get_reg(info.ra) : 0;
    }

    u64 ea = b;

    if constexpr (sizeof(Size) == 8) {
      ea += EXTS(15, info.ds << 2);
    } else {
      ea += EXTS(15, info.d);
    }

    ppe.bus->write<Size>(ea, ppe.get_reg(info.rs));

    if constexpr (Update) {
      ppe.set_reg(info.ra, ea);
    }
  }

  template <ShiftDirection Direction, AccessWidth Size>
  void shift(InstructionInfo& info, PPE& ppe) {
    u64 mask = 0;
    u64 rb_value = ppe.get_reg(info.rb);
    u64 n        = rb_value & (sizeof(Size) == 8 ? 0x7f : 0x3f);
    u8 n_limit = sizeof(Size) == 8 ? 63 : 31;
    u8 shift_value = 0;

    fmt::println("RB: {:#016x}", ppe.get_reg(info.rb));
    fmt::println("RS: {:#016x}", ppe.get_reg(info.rs));
    fmt::println("SZ: {}", sizeof(Size));

    if constexpr (Direction == Right) {
      shift_value = 64-n;
    } else {
      shift_value = n;
    }

    const Size r = std::rotl(Size(ppe.get_reg(info.rs)) & (sizeof(Size) == 4ull ? 0xFFFFFFFF : 0xFFFFFFFFFFFFFFFF), shift_value);
    fmt::println("r: {:#016x}", r);

    fmt::println("N: {}", n);

    if (n <= n_limit) {
      if constexpr (Direction == Right) {
        mask = MASK(0, n_limit - n);
      } else {
        mask = MASK(n, n_limit);
      }
    }

    ppe.set_reg(info.ra, r & mask);
  }

  template <AccessWidth Size, size_t size = sizeof(Size) * 8>
  void EXTSx(InstructionInfo& info, PPE& ppe) {
    static_assert(size <= 32, "Cannot sign extend values larger than 32-bits.");
    ppe.set_reg(info.ra, EXTS(size - 1, ppe.get_reg(info.rs)));

    if (info.rc) FATAL();
  }

  template <typename RetValWidth, typename InputWidth>
  RetValWidth get_bits(u32 n, std::pair<u8, u8> be_range) {
    const auto& [start, end] = be_range;

    assert(start <= end);

    n = n >> ((sizeof(InputWidth) * 8) - end - 1);

    u32 mask = (1 << (end + 1 - start)) - 1;

    if (start != 0) {
      n &= mask;
    }

    return n;
  }

}  // namespace PPC
