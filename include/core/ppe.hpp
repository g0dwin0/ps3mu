#pragma once
#include <array>
#include <cstddef>

#include "bus.hpp"
#include "common/defs.hpp"
#include "ppc/parser.hpp"

union FPSCR {
  struct __attribute__((packed)) {
    u8 FX       : 1;
    u8 FEX      : 1;
    u8 VX       : 1;
    u8 OX       : 1;
    u8 UX       : 1;
    u8 ZX       : 1;
    u8 XX       : 1;
    u8 VXSNAN   : 1;
    u8 VXISI    : 1;
    u8 VXIDI    : 1;
    u8 VXZDZ    : 1;
    u8 VXIMZ    : 1;
    u8 VXVC     : 1;
    u8 FR       : 1;
    u8 FI       : 1;
    u8 FPRF     : 5;
    u8 RESERVED : 1;
    u8 VXSOFT   : 1;
    u8 VXSQRT   : 1;
    u8 VXCVI    : 1;
    u8 VE       : 1;
    u8 OE       : 1;
    u8 UE       : 1;
    u8 ZE       : 1;
    u8 XE       : 1;
    u8 NI       : 1;
    u8 RN       : 2;
  } FPSCR;
};

enum struct SPR : u8 {
  XER = 1,
  LR  = 8,
  CTR = 9,
};

const std::unordered_map<SPR, std::string> spr_enum_to_str_map = {
    {SPR::XER, "XER"},
    { SPR::LR,  "LR"},
    {SPR::CTR, "CTR"}
};

struct PPE {
  std::array<u64, 32> gpr{};
  std::array<u64, 32> fpr{};
  u64 lr{};
  u64 ctr{};
  u64 xer{};
  FPSCR fpscr{};
  u64 nia{};  // next instruction address (not exposed afaik)
  u64 cia{};  // current instruction address (also not exposed, but used by the fetch unit)

  Bus* bus = nullptr;

  PPC::InstructionInfo instruction_info{};

  u32 fetch(u64 addr);

  u64& get_reg_ref(u8 reg) { return gpr.at(reg); }
  u64 get_reg(u8 reg) { return gpr.at(reg); }

  void set_reg(u8 reg, u64 value) { gpr.at(reg) = value; }

  void decode(u32 op, PPC::InstructionInfo& instruction_info);
  void step();
  void execute(PPC::InstructionInfo&);
  void set_bit_cr_reg(u8 cr_n, u8 bit);
  void clear_bit_cr_reg(u8 cr_n, u8 bit);
  void clear_full_cr_reg(u8 cr_n);
  

  union {
    u32 cr{};

    struct {
      u8 cr0 : 4;
      u8 cr1 : 4;
      u8 cr2 : 4;
      u8 cr3 : 4;
      u8 cr4 : 4;
      u8 cr5 : 4;
      u8 cr6 : 4;
      u8 cr7 : 4;
    };
  };
};