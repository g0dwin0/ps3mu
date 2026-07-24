#pragma once
#include <string>
#include <unordered_map>

#include "common/defs.hpp"
#include "ppc/parser.hpp"
#include "ppe.hpp"
static constexpr u16 DCBT = 278;
static constexpr u16 ADD = 266;

static constexpr u8 STORE_DOUBLEWORD = 0x3E;
static constexpr u16 MTOCRF          = 144;
static constexpr u16 SUBF            = 40;
static constexpr u16 SUBFE            = 136;

static constexpr u16 ADDME            = 234;
static constexpr u16 SUBFME            = 232;
static constexpr u16 SUBFZE            = 200;
static constexpr u16 ADDZ            = 202;

static constexpr u8 TDI              = 0x02;
static constexpr u8 TWI              = 0x03;
static constexpr u8 MULLI            = 0x07;
static constexpr u8 SUBFIC           = 0x08;
static constexpr u8 DOZI             = 0x09;
static constexpr u8 CMPLI            = 0x0A;
static constexpr u8 CMPI             = 0x0B;
static constexpr u8 ADDIC            = 0x0C;
static constexpr u8 ADDE             = 138;
// static constexpr u8 ADDIC             = 0x0D;
static constexpr u8 ADDI       = 0x0E;
static constexpr u8 ADDIS      = 0x0F;
static constexpr u8 BC         = 0x10;
static constexpr u8 SC         = 0x11;
static constexpr u8 B          = 0x12;
static constexpr u8 CR         = 0x13;
static constexpr u8 RLWIMI     = 0x14;
static constexpr u8 RLWINM     = 0x15;
static constexpr u8 RLMI       = 0x16;
static constexpr u8 RLWNM      = 0x17;
static constexpr u8 ORI        = 0x18;
static constexpr u8 ORIS       = 0x19;
static constexpr u8 XORI       = 0x1A;
static constexpr u8 XORIS      = 0x1B;
static constexpr u8 ANDI       = 0x1C;
static constexpr u8 ANDIS      = 0x1D;
static constexpr u8 FX_DWD_ROT = 0x1E;
static constexpr u8 FX_EXT_OPS = 0x1F;
static constexpr u8 LWZ        = 0x20;
static constexpr u8 LWZU       = 0x21;
static constexpr u8 LBZ        = 0x22;
static constexpr u8 LBZU       = 0x23;
static constexpr u8 LBZX       = 0x57;
static constexpr u16 LHZX      = 0x117;
static constexpr u16 LWZX      = 23;
static constexpr u16 LWAX      = 341;
static constexpr u16 LBZUX     = 119;
static constexpr u16 LHZUX     = 311;
static constexpr u16 LWZUX     = 55;

static constexpr u16 LWAUX = 373;

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
static constexpr u8 ADDC              = 10;
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
static constexpr u16 SUBFC            = 8;

static constexpr u16 MTSPR            = 467;
static constexpr u16 _OR              = 444;
static constexpr u16 EXTSB            = 954;
static constexpr u16 EXTSH            = 922;
static constexpr u16 EXTSW            = 986;
static constexpr u16 STDU             = 62;
static constexpr u16 LDX              = 21;
static constexpr u16 LDUX             = 53;
static constexpr u16 STBX             = 215;
static constexpr u16 STHX             = 407;
static constexpr u16 STWX             = 151;
static constexpr u16 STDX             = 149;

static constexpr u16 STBUX = 247;
static constexpr u16 STHUX = 439;
static constexpr u16 STWUX = 183;
static constexpr u16 STDUX = 181;

static constexpr u16 MULLW = 235;
static constexpr u16 MULLD = 233;

static constexpr u16 MULHW = 75;
static constexpr u16 MULHD = 73;



static constexpr u16 DIVWU = 459;
static constexpr u16 DIVDU = 457;
static constexpr u16 DIVW  = 491;
static constexpr u16 DIVD  = 489;

static constexpr u16 SRADI = 413;
static constexpr u16 SRAWI = 824;
static constexpr u16 SRAD  = 794;
static constexpr u16 SRAW  = 792;
static constexpr u16 SRW   = 536;
static constexpr u16 SRD   = 539;
static constexpr u16 SLW   = 24;
static constexpr u16 SLD   = 27;

static constexpr u16 LHAX  = 343;
static constexpr u16 LHAUX = 375;

enum TargetRegisterType { FPR, GPR };
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
      {   10,      {"addc", InstructionFormat::XO_FORM}},
      {    32,     {"cmpl", InstructionFormat::X_FORM}},
      {  SUBF,    {"subf", InstructionFormat::XO_FORM}},
      {  ADDE,    {"adde", InstructionFormat::XO_FORM}},
      {   266,     {"add", InstructionFormat::XO_FORM}},
      {    19,   {"mfcr", InstructionFormat::XFX_FORM}},
      {    28,      {"and", InstructionFormat::X_FORM}},
      {    60,     {"andc", InstructionFormat::X_FORM}},
      {   412,       {"or", InstructionFormat::X_FORM}},
      {   8,       {"subfc", InstructionFormat::XO_FORM}},
      {   SUBFE,       {"subfe", InstructionFormat::XO_FORM}},
      {   ADDME,       {"addme", InstructionFormat::XO_FORM}},
      {   SUBFME,       {"subfme", InstructionFormat::XO_FORM}},
      {   ADDZ,       {"addz", InstructionFormat::XO_FORM}},
      {   SUBFZE,       {"subfze", InstructionFormat::XO_FORM}},
      
      
      
      
      
      

      {   316,      {"xor", InstructionFormat::X_FORM}},
      {   104,     {"neg", InstructionFormat::XO_FORM}},
      {   278,     {"dcbt", InstructionFormat::X_FORM}},
      {   124,      {"nor", InstructionFormat::X_FORM}},
      {CNTLZD,   {"cntlzd", InstructionFormat::X_FORM}},
      {CNTLZW,   {"cntlzw", InstructionFormat::X_FORM}},
      {   LDX,      {"ldx", InstructionFormat::X_FORM}},
      {  LBZX,     {"lbzx", InstructionFormat::X_FORM}},
      {  LHZX,     {"lhzx", InstructionFormat::X_FORM}},
      {  LWZX,     {"lwzx", InstructionFormat::X_FORM}},

      {  LHAX,     {"lhax", InstructionFormat::X_FORM}},
      { LHAUX,    {"lhaux", InstructionFormat::X_FORM}},

      {  LWAX,     {"lwax", InstructionFormat::X_FORM}},
      { LBZUX,    {"lbzux", InstructionFormat::X_FORM}},
      { LHZUX,    {"lhzux", InstructionFormat::X_FORM}},
      { LWZUX,    {"lwzux", InstructionFormat::X_FORM}},

      { LWAUX,    {"lwaux", InstructionFormat::X_FORM}},

      {  LDUX,     {"ldux", InstructionFormat::X_FORM}},

      {  STBX,     {"stbx", InstructionFormat::X_FORM}},
      {  STHX,     {"sthx", InstructionFormat::X_FORM}},
      {  STWX,     {"stwx", InstructionFormat::X_FORM}},
      {  STDX,     {"stdx", InstructionFormat::X_FORM}},

      { STBUX,    {"stbux", InstructionFormat::X_FORM}},
      { STHUX,    {"sthux", InstructionFormat::X_FORM}},
      { STWUX,    {"stwux", InstructionFormat::X_FORM}},
      { STDUX,    {"stdux", InstructionFormat::X_FORM}},

      { MULLW,   {"MULLW", InstructionFormat::XO_FORM}},
      { MULLD,   {"MULLD", InstructionFormat::XO_FORM}},

      { MULHW,   {"MULHW", InstructionFormat::XO_FORM}},
      { MULHD,   {"MULHD", InstructionFormat::XO_FORM}},

      

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
      { EXTSB,    {"extsb", InstructionFormat::X_FORM}},
      { DIVWU,   {"divwu", InstructionFormat::XO_FORM}},
      {  DIVW,    {"divw", InstructionFormat::XO_FORM}},
      { DIVDU,   {"divdu", InstructionFormat::XO_FORM}},
      {  DIVD,    {"divd", InstructionFormat::XO_FORM}},

      { SRAWI,    {"srawi", InstructionFormat::X_FORM}},
      { SRADI,   {"sradi", InstructionFormat::XS_FORM}},
  };

  void mulli(InstructionInfo&, PPE& ppe);
  void mullw(InstructionInfo&, PPE& ppe);
  void mulhw(InstructionInfo&, PPE& ppe);
  
  void mulld(InstructionInfo&, PPE& ppe);
  void mulhd(InstructionInfo&, PPE& ppe);
  

  void subfic(InstructionInfo&, PPE& ppe);
  void dozi(InstructionInfo&, PPE& ppe);
  void cmpli(InstructionInfo&, PPE& ppe);
  void cmpi(InstructionInfo&, PPE& ppe);
  void cmp(InstructionInfo&, PPE& ppe);

  void addic(InstructionInfo&, PPE& ppe);
  void addc(InstructionInfo&, PPE& ppe);

  void branch(InstructionInfo&, PPE& ppe);
  void bclr(InstructionInfo&, PPE& ppe);
  void bcctr(InstructionInfo&, PPE& ppe);
  void mtocrf(InstructionInfo& info, PPE& ppe);
  void mtcrf(InstructionInfo& info, PPE& ppe);

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
  void subfze(InstructionInfo& info, PPE& ppe);
  void subfe(InstructionInfo& info, PPE& ppe);
  
  void subfc(InstructionInfo& info, PPE& ppe);

  template <AccessWidth Size>
  void CNTLZx(InstructionInfo& info, PPE& ppe) {
    ppe.set_gpr(info.ra, std::countl_zero<Size>(ppe.get_gpr(info.rs)));

    if (info.rc) FATAL();
  }

  template <AccessWidth Size, bool Update, bool SignExtRT, TargetRegisterType type>
  void gen_load(InstructionInfo& info, PPE& ppe) {
    u64 b  = 0;
    u64 ea = 0;

    if constexpr (Update) {
      b = ppe.get_gpr(info.ra);
    } else {
      if (info.ra != 0) {
        b = ppe.get_gpr(info.ra) & 0xFFFFFFFF;
      }
    }

    if constexpr ((sizeof(Size) == 8 || (SignExtRT && sizeof(Size) == 4)) && type == GPR) {
      ea = b + EXTS(15, info.ds << 2);
    } else {
      ea = b + EXTS(15, info.d);
    }

    spdlog::info("EA = {:#016x}", ea);
    u64 result      = ppe.bus->read<Size>(ea);
    u64 write_value = SignExtRT ? EXTS((sizeof(Size) * 8) - 1, result) : result;

    if constexpr (type == GPR) {
      ppe.set_gpr(info.rt, write_value);
    } else {
      ppe.set_fpr(info.frt, write_value);
    }

    if (Update) {
      ppe.set_gpr(info.ra, ea);
    }
  }

  template <AccessWidth Size, bool Algebraic, bool Update>
  void gen_load_indexed_gpr(InstructionInfo& info, PPE& ppe) {
    u64 b  = 0;
    u64 ea = 0;

    constexpr u8 size = sizeof(Size) * 8;

    if constexpr (!Update) {
      if (info.ra != 0) {
        b = ppe.get_gpr(info.ra);
      }
    } else {
      b = ppe.get_gpr(info.ra);
    }

    ea = b + ppe.get_gpr(info.rb);

    spdlog::info("EA = {:#016x}", ea);
    u64 result = ppe.bus->read<Size>(ea);

    if constexpr (Algebraic) {
      result = EXTS(size - 1, result);
    }

    ppe.set_gpr(info.rt, result);

    if constexpr (Update) {
      ppe.set_gpr(info.ra, ea);
    }
  }

  template <AccessWidth Size, bool Algebraic, bool Update>
  void gen_load_indexed_fpr(InstructionInfo& info, PPE& ppe) {
    u64 b  = 0;
    u64 ea = 0;

    constexpr u8 size = sizeof(Size) * 8;

    if constexpr (!Update) {
      if (info.ra != 0) {
        b = ppe.get_gpr(info.ra);
      }
    } else {
      b = ppe.get_gpr(info.ra);
    }

    ea = b + ppe.get_gpr(info.rb);

    spdlog::info("EA = {:#016x}", ea);
    u64 result = ppe.bus->read<Size>(ea);

    if constexpr (Algebraic) {
      result = EXTS(size - 1, result);
    }

    ppe.set_gpr(info.rt, result);

    if constexpr (Update) {
      ppe.set_gpr(info.ra, ea);
    }
  }

  template <AccessWidth Size, bool Algebraic, bool Update>
  void gen_store_indexed(InstructionInfo& info, PPE& ppe) {
    (void)Algebraic;

    if (Algebraic) FATAL();

    u64 b  = 0;
    u64 ea = 0;

    // constexpr u8 size = sizeof(Size) * 8;

    if constexpr (!Update) {
      spdlog::info("ROUTE 1");
      if (info.ra != 0) {
        b = ppe.get_gpr(info.ra);
      }
    } else {
      b = ppe.get_gpr(info.ra);
    }

    spdlog::info("B: {}", b);
    ea = b + ppe.get_gpr(info.rb);

    // spdlog::info("EA = {:#016x}", ea);
    // u64 result          = ppe.bus->read<Size>(ea);

    u64 x = ppe.get_gpr(info.rs);

    ppe.bus->write<Size>(ea, static_cast<Size>(x));

    // if constexpr (Algebraic) {
    //   result = EXTS(size-1, result);
    // }

    if constexpr (Update) {
      ppe.set_gpr(info.ra, ea);
    }
  }

  template <AccessWidth Size, bool Update, TargetRegisterType type>
  void gen_store(InstructionInfo& info, PPE& ppe) {
    u64 b = 0;

    if constexpr (Update) {
      b = ppe.get_gpr(info.ra);
    } else {
      b = info.ra != 0 ? ppe.get_gpr(info.ra) : 0;
    }

    u64 ea = b;

    if constexpr (sizeof(Size) == 8 && type != FPR) {
      ea += EXTS(15, info.ds << 2);
    } else {
      ea += EXTS(15, info.d);
    }

    if constexpr (type == GPR) {
      ppe.bus->write<Size>(ea, ppe.get_gpr(info.rs));
    } else {
      ppe.bus->write<Size>(ea, ppe.get_gpr(info.frs));
    }

    if constexpr (Update) {
      ppe.set_gpr(info.ra, ea);
    }
  }

  template <ShiftDirection Direction, AccessWidth Size>
  void shift(InstructionInfo& info, PPE& ppe) {
    u64 mask       = 0;
    u64 rb_value   = ppe.get_gpr(info.rb);
    u64 n          = rb_value & (sizeof(Size) == 8 ? 0x7f : 0x3f);
    u8 n_limit     = sizeof(Size) == 8 ? 63 : 31;
    u8 shift_value = 0;

    spdlog::info("RB: {:#016x}", ppe.get_gpr(info.rb));
    spdlog::info("RS: {:#016x}", ppe.get_gpr(info.rs));
    spdlog::info("SZ: {}", sizeof(Size));

    if constexpr (Direction == Right) {
      shift_value = 64 - n;
    } else {
      shift_value = n;
    }

    const Size r = std::rotl(Size(ppe.get_gpr(info.rs)) & (sizeof(Size) == 4ull ? 0xFFFFFFFF : 0xFFFFFFFFFFFFFFFF), shift_value);
    spdlog::info("r: {:#016x}", r);

    spdlog::info("N: {}", n);

    if (n <= n_limit) {
      if constexpr (Direction == Right) {
        mask = MASK(0, n_limit - n);
      } else {
        mask = MASK(n, n_limit);
      }
    }

    ppe.set_gpr(info.ra, r & mask);
  }

  template <ShiftDirection Direction = Right, AccessWidth Size>
  void shift_algebraic(InstructionInfo& info, PPE& ppe) {
    static_assert(Direction == Right);

    u64 n        = info.sh;
    const Size r = std::rotl<Size>(Size(ppe.get_gpr(info.rs)) & (sizeof(Size) == 4ull ? U32_MAX : U64_MAX), 64 - n);
    u8 n_limit   = sizeof(Size) == 8 ? 63 : 31;
    u64 mask     = 0;

    spdlog::info("RB: {:#016x}", ppe.get_gpr(info.rb));
    spdlog::info("RS: {:#016x}", ppe.get_gpr(info.rs));
    spdlog::info("SZ: {}", sizeof(Size));

    mask = MASK(0, n_limit - n);

    u64 expanded_carry = (ppe.get_gpr(info.rs) & (1ull << (n_limit))) != 0 ? U64_MAX : 0;
    // if(carry)

    ppe.set_gpr(info.ra, (r & mask) | (expanded_carry & ~mask));

    if (((expanded_carry & ((r & ~mask)) & U32_MAX)) != 0) {
      ppe.set_carry();
    } else {
      ppe.reset_carry();
    }
  }

  enum DivSign { Signed, Unsigned };
  template <AccessWidth Size, DivSign Sign>
  void div(InstructionInfo& info, PPE& ppe) {
    u64 mask = U64_MAX;
    if constexpr (sizeof(Size) == 4) {
      mask = U32_MAX;
    }
    Size dividend, divisor;

    // if (!D) FATAL();
    if (info.rc) FATAL();
    if (info.oe) FATAL();

    dividend = ppe.get_gpr(info.ra);
    divisor  = ppe.get_gpr(info.rb);

    if constexpr (sizeof(Size) == 4 && Sign == Signed) {
      dividend = EXTS(31, (Size)ppe.get_gpr(info.ra));
      divisor  = EXTS(31, (Size)ppe.get_gpr(info.rb));
    }
    Size result = dividend / divisor;
    ppe.set_gpr(info.rt, result & mask);
  }

  enum WordHeight { LOW_WORD, HIGH_WORD };
  template <AccessWidth Size, bool Unsigned, WordHeight WordHeight>
  void mul(InstructionInfo& info, PPE& ppe) {
    (void)(ppe);
    (void)(info);
  }

  template <AccessWidth Size, size_t size = sizeof(Size) * 8>
  void EXTSx(InstructionInfo& info, PPE& ppe) {
    static_assert(size <= 32, "Cannot sign extend values larger than 32-bits.");
    ppe.set_gpr(info.ra, EXTS(size - 1, ppe.get_gpr(info.rs)));

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
