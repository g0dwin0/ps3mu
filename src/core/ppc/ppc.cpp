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
void PPC::branch(PPC::InstructionInfo& info, PPE& ppe) {
  (void)info;
  (void)ppe;
fmt::println("AA: {}", info.aa);
fmt::println("LK: {}", info.lk);

  assert(!info.aa && info.lk);
  // if(info.aa) {
  // assume aa = 0, lk = 1
fmt::println("CIA: {:#016x}", ppe.cia);
fmt::println("LI: {:#016x}", info.li);

  ppe.nia = ppe.cia + (info.li << 2);
  fmt::println("calculated NIA: {:#016x}", ppe.nia);

  if (info.lk) {
    ppe.lr = ppe.cia;
  }

  fmt::println("end of branch");
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
void PPC::mfspr(PPC::InstructionInfo& info, PPE& ppe) {
  SPR spr = (SPR)((info.d >> 5) & 0b11111);

  switch (spr) {
    case SPR::XER: {
      ppe.gpr.at(info.rt) = ppe.xer;
      break;
    }
    case SPR::LR: {
      ppe.gpr.at(info.rt) = ppe.lr;
      break;
    }
    case SPR::CTR: {
      ppe.gpr.at(info.rt) = ppe.ctr;
      break;
    }

    default: return;
  }
}

void PPC::OR(PPC::InstructionInfo& info, PPE& ppe) {
  ppe.set_reg(info.ra, ppe.get_reg(info.rs) | ppe.get_reg(info.rb));

  if (info.rc) assert(0);
}

void PPC::mtspr(PPC::InstructionInfo& info, PPE& ppe) {
  SPR spr = (SPR)((info.d >> 5) & 0b11111);

  switch (spr) {
    case SPR::XER: {
      ppe.xer = ppe.gpr.at(info.rt);
      break;
    }
    case SPR::LR: {
      ppe.lr = ppe.gpr.at(info.rt);
      break;
    }
    case SPR::CTR: {
      ppe.ctr = ppe.gpr.at(info.rt);
      break;
    }

    default: return;
  }
}

void PPC::gen_load(PPC::InstructionInfo& info, PPE& ppe) {
  (void)info;
  (void)ppe;

  switch (info.opcode) {
    case 32: {
      u64 b  = 0;
      u64 ea = 0;
      if (info.ra != 0) {
        fmt::println("RA = {:#016x}", ppe.gpr.at(info.ra));
        fmt::println("d = {:#016x}", (i16)info.d);
        
        b = ppe.gpr.at(info.ra);
      }

      ea = b + (i16)info.d;
              fmt::println("EA = {:#016x} -> {:#016x}", ea, ppe.bus->read<u32>(ea));


      ppe.gpr.at(info.rt) = (u64)ppe.bus->read<u32>(ea);
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

  switch (info.opcode) {
    case 36:
    case 37: {
      u64 ra = info.ra != 0 ? ppe.get_reg(info.ra) : 0;

      u64 ea = ra + info.ds;

      fmt::println("ea: {:#016x}", ea);

      ppe.bus->write<u32>(ea, ppe.get_reg(info.rs));
      // fmt::println("written");

      if (info.opcode == 37) {
        ppe.set_reg(info.ra, ea);
      }

      break;
    }

    case 62: {
      u64 ea = ppe.get_reg(info.ra) + (i16)(info.ds << 2);

      fmt::println("ea: {:#016x}", ea);

      ppe.bus->write<u64>(ea, ppe.get_reg(info.rs));
      // fmt::println("written");

      if (info.xo) {
        ppe.set_reg(info.ra, ea);
      }

      break;
    }
    default: assert(0 && "no valid gen store opcode");
  }
};
