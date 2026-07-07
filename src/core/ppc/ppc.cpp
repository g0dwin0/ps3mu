#include "ppc/ppc.hpp"

#include <cassert>

#include "common.hpp"
#include "common/defs.hpp"
#include "lv2/syscalls/syscall.hpp"
#include "ppc/parser.hpp"
#include "ppe.hpp"
#include "self/self.hpp"
void PPC::addi(PPC::InstructionInfo& info, PPE& ppe) {
  if (info.ra == 0) {
    ppe.gpr.at(info.rt) = EXTS(15, info.si);
  } else {
    ppe.gpr.at(info.rt) = ppe.gpr.at(info.ra) + EXTS(15, info.si);
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

  u64 a = 0;

  if (info.l == 0) {
    a = EXTS(31, ppe.get_reg(info.ra) & 0xFFFFFFFF);
  } else {
    a = ppe.get_reg(info.ra);
  }

  u64 ext_si = EXTS(15, info.si);

  fmt::println("A: {:#016x}", a);
  fmt::println("INFO.BF: {:#016x}", info.bf);
  fmt::println("EX_TSI: {:#016x}", ext_si);

  if (a < ext_si) {
    ppe.set_bit_cr_reg(info.bf, 0);
  } else if (a > ext_si) {
    ppe.set_bit_cr_reg(info.bf, 1);
  } else {
    ppe.set_bit_cr_reg(info.bf, 2);
  }

  fmt::println("CR POST CMPI: {:#016x}", ppe.cr);
};
void PPC::addic(PPC::InstructionInfo& info, PPE& ppe) {
  (void)info;
  (void)ppe;
};
void PPC::branch(PPC::InstructionInfo& info, PPE& ppe) {
  fmt::println("AA: {}", info.aa);
  fmt::println("LK: {}", info.lk);

  // if(info.aa) {
  // assume aa = 0, lk = 1
  fmt::println("CIA: {:#016x}", ppe.cia);
  fmt::println("LI: {:#016x}", info.li);
  fmt::println("LI<<2: {:#016x}", info.li << 2);
  fmt::println("LI [EXT]: {:#016x}", EXTS(23, info.li << 2));
  // 00000000000000111111110011101000

  u64 target_addr = 0;

  if (info.aa) {
    target_addr = EXTS(23, info.li << 2);
  } else {
    target_addr = info.address + EXTS(23, info.li << 2);
  }

  fmt::println("calculated NIA: {:#016x}", target_addr);

  if (function_map.contains(target_addr)) {
    const auto& [function_str, nid, func] = function_map.at((u64)ppe.cia + (info.li << 2));

    fmt::println("{}", function_str);

    if (func == nullptr) {
      FATAL(fmt::format("{} is not bound to a function", function_str));
    }

    func(ppe);
    return;
  }

  ppe.nia = target_addr;

  if (info.lk) {
    ppe.lr = ppe.cia + 4;
  }

  fmt::println("end of branch");
};

void PPC::branch_conditional(PPC::InstructionInfo& info, PPE& ppe) {
  // bool condition_met = (ppe.cr & (1ull << info.bi)) != 0;
  fmt::println("[pre-branch] CR = {:#016x}", ppe.cr);
  if (!ppe.check_branch_condition(info.bo, info.bi)) return;

  ppe.nia = info.target_addr;

  // fmt::println("BCBCBC TARGET: {:016x}", ppe.nia);

  if (function_map.contains(info.target_addr)) {
    const auto& [function_str, nid, func] = function_map.at((u64)ppe.cia + (info.li << 2));

    fmt::println("{}", function_str);

    if (func == nullptr) {
      FATAL(fmt::format("{} is not bound to a function", function_str));
    }

    func(ppe);
    return;
  }

  ppe.nia = info.target_addr;

  if (info.lk) {
    ppe.lr = ppe.cia + 4;
  }

  fmt::println("end of branch");
};

void PPC::bclr(PPC::InstructionInfo& info, PPE& ppe) {
  if (!ppe.check_branch_condition(info.bo, info.bi)) return;
  fmt::println("[BCLR] entered");

  if (function_map.contains((ppe.lr >> 2) << 2)) {
    const auto& [function_str, nid, func] = function_map.at((u64)ppe.cia + (info.li << 2));

    fmt::println("{}", function_str);

    if (func == nullptr) {
      FATAL(fmt::format("{} is not bound to a function", function_str));
    }

    func(ppe);
    return;
  }

  ppe.nia = (ppe.lr >> 2) << 2;

  if (info.lk) {
    ppe.lr = ppe.cia + 4;
  }

  fmt::println("end of branch BCLR");
};

void PPC::bc(PPC::InstructionInfo& info, PPE& ppe) {
  (void)info;
  (void)ppe;
};
void PPC::sc(PPC::InstructionInfo& info, PPE& ppe) {
  (void)info;
  (void)ppe;
  lv2_handle_syscall(ppe);
  fmt::println("[SC] r11: {:#06x}", ppe.get_reg(11));
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
  u64 n = info.sh;

  u64 r = std::rotl((u32)ppe.get_reg(info.rs), n);

  u64 m = MASK(info.mb + 32, info.me + 32);

  ppe.set_reg(info.ra, r & m);
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

    default: FATAL();
  }
}

void PPC::gen_load(PPC::InstructionInfo& info, PPE& ppe) {
  (void)info;
  (void)ppe;

  switch (info.opcode) {
    case LWZ: {
      u64 b  = 0;
      u64 ea = 0;
      if (info.ra != 0) {
        fmt::println("RA({}) = {:#016x}", info.ra, ppe.gpr.at(info.ra));
        fmt::println("d = {:#016x}", info.d);

        b = ppe.gpr.at(info.ra);
      }

      ea = b + EXTS(15, info.d);
      fmt::println("EA = {:#016x}", ea);
      ppe.gpr.at(info.rt) = (u64)ppe.bus->read<u32>(ea);

      break;
    }

    case LBZ: {
      u64 b  = 0;
      u64 ea = 0;
      if (info.ra != 0) {
        fmt::println("RA = {:#016x}", ppe.gpr.at(info.ra));
        fmt::println("d = {:#016x}", info.d);

        b = ppe.gpr.at(info.ra);
      }

      ea = b + EXTS(15, info.d);
      fmt::println("EA = {:#016x}", ea);
      ppe.gpr.at(info.rt) = (u64)ppe.bus->read<u8>(ea);

      break;
    }

    case 58: {  // LD{U}
      fmt::println("hi from LD[U]");
      u64 ea = 0;
      u64 b  = 0;

      bool update = PPC::get_bits<u8, u32>(info.full_opcode, {30, 31});

      if (update) {
        ea = ppe.gpr.at(info.ra) + EXTS(13, info.ds << 2);

      } else {
        if (info.ra != 0) {
          b = ppe.gpr.at(info.ra);
        }
        ea = b + EXTS(13, info.ds << 2);
      }

      fmt::println("b:{:#016x}", b);
      fmt::println("RA:{:#016x}", info.ra);

      fmt::println("EA:{:#016x}", ea);
      ppe.set_reg(info.rt, htonll(ppe.bus->read<u64>(ea)));

      if (update) {
        ppe.set_reg(info.ra, ppe.bus->read<u64>(ea));
      }

      break;
    }

      // case 58: {
      //       u64  ea = b + (i16)info.d;

      //   break;
      // }

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
    case 37: {
      u64 ra = info.ra != 0 ? ppe.get_reg(info.ra) : 0;

      u64 ea = ra + info.ds;

      // fmt::println("ea: {:#016x}", ea);

      ppe.bus->write<u32>(ea, ppe.get_reg(info.rs));
      // fmt::println("written");

      if (info.opcode == 37) {
        ppe.set_reg(info.ra, ea);
      }

      break;
    }

    case STW: {
      u64 ra = info.ra != 0 ? ppe.get_reg(info.ra) : 0;

      u64 ea = ra + EXTS(15, info.d);

      fmt::println("info.d: {:#016x}", info.d);

      ppe.bus->write<u32>(ea, ppe.get_reg(info.rs));

      break;
    }

    case STB: {
      u64 ra = info.ra != 0 ? ppe.get_reg(info.ra) : 0;

      u64 ea = ra + EXTS(15, info.d);

      // fmt::println("ea: {:#016x}", ea);

      ppe.bus->write<u8>(ea, (u8)ppe.get_reg(info.rs));
      // fmt::println("written");
      break;
    }

    case 62: {  // STD(U)
      u64 ds = EXTS(13, info.ds << 2);

      u64 ea = ds;

      if (!info.xo) {
        ea += info.ra != 0 ? ppe.get_reg(info.ra) : 0;
      }

      ppe.bus->write<u64>(ea, ppe.get_reg(info.rs));

      if (info.xo) {
        ppe.set_reg(info.ra, ea);
      }

      break;
    }

    default: assert(0 && "no valid gen store opcode");
  }
};
