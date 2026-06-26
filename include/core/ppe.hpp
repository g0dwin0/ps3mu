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

struct PPE {


  std::array<u64, 32> gpr;
  std::array<u64, 32> fpr;
  u64 lr;
  u64 ctr;
  u64 xer;
  FPSCR fpscr;
  u64 nia;  // next instruction address (not exposed afaik)
  u64 cia;  // current instruction address (also not exposed, but used by the fetch unit)

  Bus* bus = nullptr;
  
  PPC::InstructionInfo instruction_info;

  u64 fetch(u64 addr);
  void step();
  void execute(PPC::InstructionInfo&);
  union {
    u32 cr;

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