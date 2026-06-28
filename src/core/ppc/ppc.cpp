#include "ppc/ppc.hpp"

#include <cassert>

#include "common.hpp"
#include "common/defs.hpp"
#include "ppc/parser.hpp"
#include "ppe.hpp"

template <typename T>
T EXTS(T v) {
  return v;
}

void PPC::addi(PPC::InstructionInfo& info, PPE& ppe) {
  // (void)info;
  // (void)ppe;
  fmt::println("HELLO");
  if (info.ra == 0) {
    ppe.gpr.at(info.rt) = info.si;
  } else {
    assert(0);
  }
};

void PPC::tdi(PPC::InstructionInfo& info, PPE& ppe) {
  (void)info;
  (void)ppe;
};
void PPC::twi(PPC::InstructionInfo& info, PPE& ppe) {
  (void)info;
  (void)ppe;
};
void PPC::mulli(PPC::InstructionInfo& info, PPE& ppe) {
  (void)info;
  (void)ppe;
};
void PPC::subfic(PPC::InstructionInfo& info, PPE& ppe) {
  (void)info;
  (void)ppe;
};
void PPC::dozi(PPC::InstructionInfo& info, PPE& ppe) {
  (void)info;
  (void)ppe;
};
void PPC::cmpli(PPC::InstructionInfo& info, PPE& ppe) {
  (void)info;
  (void)ppe;
};
void PPC::cmpi(PPC::InstructionInfo& info, PPE& ppe) {
  (void)info;
  (void)ppe;
};
void PPC::addic(PPC::InstructionInfo& info, PPE& ppe) {
  (void)info;
  (void)ppe;
};
void PPC::bc(PPC::InstructionInfo& info, PPE& ppe) {
  (void)info;
  (void)ppe;
};
void PPC::sc(PPC::InstructionInfo& info, PPE& ppe) {
  (void)info;
  (void)ppe;
};
void PPC::b(PPC::InstructionInfo& info, PPE& ppe) {
  (void)info;
  (void)ppe;
};
void PPC::rlwimi(PPC::InstructionInfo& info, PPE& ppe) {
  (void)info;
  (void)ppe;
};
void PPC::rlwinm(PPC::InstructionInfo& info, PPE& ppe) {
  (void)info;
  (void)ppe;
};
void PPC::ori(PPC::InstructionInfo& info, PPE& ppe) {
  u64 or_val = info.ui;
  fmt::println("or val: {:#010x}", or_val);

  ppe.gpr.at(info.ra) = (u64)(ppe.gpr.at(info.rs) | or_val);
};
void PPC::oris(PPC::InstructionInfo& info, PPE& ppe) {
  u64 or_val = info.ui << 16;
  fmt::println("or val: {:#010x}", or_val);

  ppe.gpr.at(info.ra) = ppe.gpr.at(info.rs) | or_val;
};
void PPC::xori(PPC::InstructionInfo& info, PPE& ppe) {
  (void)info;
  (void)ppe;
};
void PPC::xoris(PPC::InstructionInfo& info, PPE& ppe) {
  (void)info;
  (void)ppe;
};
void PPC::andi(PPC::InstructionInfo& info, PPE& ppe) {
  (void)info;
  (void)ppe;
};
void PPC::andis(PPC::InstructionInfo& info, PPE& ppe) {
  (void)info;
  (void)ppe;
};
void PPC::lwz(PPC::InstructionInfo& info, PPE& ppe) {
  (void)info;
  (void)ppe;
};

void PPC::gen_load(PPC::InstructionInfo& info, PPE& ppe) {
  (void)info;
  (void)ppe;

  switch (info.opcode) {
    case 32: {
      u64 b  = 0;
      u64 ea = 0;
      if (info.ra != 0) {
        fmt::println("RA = {:#010x}", ppe.gpr.at(info.ra));
        b = ppe.gpr.at(info.ra);
      }

      ea = b + info.d;

      ppe.gpr.at(info.rt) = ppe.bus->read<u32>(ea);
      break;
    }



    default: {
      fmt::println("no decode logic for: {}", info.opcode);
      assert(0);
    }
  }
};
void PPC::gen_store(PPC::InstructionInfo& info, PPE& ppe) {
  (void)info;
  (void)ppe;


  switch(info.opcode) {

    case 62: {
      u64 ea = ppe.get_reg(info.ra) + (i16)(info.ds << 2);


      ppe.bus->write<u64>(ea, ppe.get_reg(info.rs));
      fmt::println("written");

      ppe.set_reg(info.ra, ea);
      break;
    }
    default: assert(0);

  }
};