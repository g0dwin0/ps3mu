#include "ppc/ppc.hpp"

#include <bit>

#include "common.hpp"
#include "common/defs.hpp"
#include "lv2/syscalls/syscall.hpp"
#include "nid/nid.hpp"
#include "ppc/parser.hpp"
#include "ppe.hpp"
#include "self/self.hpp"

void PPC::addi(InstructionInfo& info, PPE& ppe) {
  if (info.ra == 0) {
    ppe.gpr.at(info.rt) = EXTS(15, info.si);
  } else {
    ppe.gpr.at(info.rt) = ppe.gpr.at(info.ra) + EXTS(15, info.si);
  }
};

void PPC::addis(InstructionInfo& info, PPE& ppe) {
  if (info.ra == 0) {
    ppe.gpr.at(info.rt) = EXTS(31, info.si << 16);
  } else {
    ppe.gpr.at(info.rt) = ppe.gpr.at(info.ra) + EXTS(31, info.si << 16);
  }
};

void PPC::mulli(InstructionInfo& info, PPE& ppe) {
  (void)info;
  (void)ppe;
  FATAL();
};
void PPC::subfic(InstructionInfo& info, PPE& ppe) {
  (void)info;
  (void)ppe;
  FATAL();
};
void PPC::dozi(InstructionInfo& info, PPE& ppe) {
  (void)info;
  (void)ppe;
  FATAL();
};
void PPC::cmpli(InstructionInfo& info, PPE& ppe) {
  u64 a = 0;

  if (info.l == 0) {
    a = ppe.get_reg(info.ra) & 0xFFFFFFFF;
  } else {
    a = ppe.get_reg(info.ra);
  }

  u64 comp_val = info.ui & 0xFFFF;

  ppe.set_cr_field(info.bf, a < comp_val ? CR_LT : a > comp_val ? CR_GT : CR_EQ);

  fmt::println("CR POST CMPLI: {:#016x}", ppe.cr);
};
void PPC::cmpi(InstructionInfo& info, PPE& ppe) {
  (void)info;
  (void)ppe;

  i64 a = 0;

  if (info.l == 0) {
    a = (i64)EXTS(31, ppe.get_reg(info.ra) & 0xFFFFFFFF);
  } else {
    a = ppe.get_reg(info.ra);
  }

  u64 ext_si = EXTS(15, info.si);

  i64 s_si = (i64)ext_si;

  fmt::println("A: {:#016x}", a);
  fmt::println("INFO.BF: {:#016x}", info.bf);
  fmt::println("EX_TSI: {:#016x}", ext_si);

  ppe.set_cr_field(info.bf, a < s_si ? CR_LT : a > s_si ? CR_GT : CR_EQ);

  fmt::println("CR POST CMPI: {:#016x}", ppe.cr);
};

void PPC::cmp(InstructionInfo& info, PPE& ppe) {
  (void)info;
  (void)ppe;

  i64 a, b = 0;
  if (info.l == 0) {
    a = (i64)EXTS(31, ppe.get_reg(info.ra) & 0xFFFFFFFF);
    b = (i64)EXTS(31, ppe.get_reg(info.rb) & 0xFFFFFFFF);
    ppe.set_cr_field(info.bf, a < b ? CR_LT : a > b ? CR_GT : CR_EQ);

  } else {
    a = (i64)ppe.get_reg(info.ra);
    b = (i64)ppe.get_reg(info.rb);
    ppe.set_cr_field(info.bf, a < b ? CR_LT : a > b ? CR_GT : CR_EQ);
  }
};

void PPC::addic(InstructionInfo& info, PPE& ppe) {
  (void)info;
  (void)ppe;
  FATAL();
};
void PPC::branch(InstructionInfo& info, PPE& ppe) {
  if (info.lk) {
    ppe.lr = ppe.cia + 4;
  }

  u64 target_addr = 0;

  if (info.aa) {
    target_addr = EXTS(25, info.li << 2);
  } else {
    target_addr = info.address + EXTS(25, info.li << 2);
  }

  fmt::println("calculated NIA: {:#016x}", target_addr);

  if (stub_map.contains(target_addr)) {
    const auto& [function_str, nid, func] = stub_map.at(target_addr);
    const auto& [name, ptr, toc]          = nid_table_map.at(nid);
    fmt::println("{}", name);
    // ppe.set_reg(2, toc);

    ptr(ppe);

    return;
  }

  ppe.nia = target_addr;

  fmt::println("end of branch");
};

void PPC::branch_conditional(InstructionInfo& info, PPE& ppe) {
  fmt::println("[pre-branch] CR = {:#016x}", ppe.cr);
  if (info.lk) {
    ppe.lr = ppe.cia + 4;
  }
  if (!ppe.check_branch_condition(info.bo, info.bi)) return;

  if (stub_map.contains(info.target_addr)) {
    const auto& [function_str, nid, func] = stub_map.at(info.target_addr);
    const auto& [name, ptr, toc]          = nid_table_map.at(nid);
    fmt::println("{}", name);
    // ppe.set_reg(2, toc);

    ptr(ppe);

    return;
  }

  ppe.nia = info.target_addr;

  fmt::println("end of branch");
};

void PPC::bclr(InstructionInfo& info, PPE& ppe) {
  if (info.lk) {
    ppe.lr = ppe.cia + 4;
  }
  if (!ppe.check_branch_condition(info.bo, info.bi)) return;
  fmt::println("[BCLR] entered");

  u64 calc_addr = (ppe.lr >> 2) << 2;

  if (stub_map.contains(ppe.nia)) {
    const auto& [function_str, nid, func] = stub_map.at(ppe.nia);
    const auto& [name, ptr, toc]          = nid_table_map.at(nid);
    fmt::println("{}", name);
    // ppe.set_reg(2, toc);

    ptr(ppe);

    return;
  }

  ppe.nia = calc_addr;

  fmt::println("end of branch BCLR");
};

void PPC::bcctr(InstructionInfo& info, PPE& ppe) {
  if (info.lk) {
    ppe.lr = ppe.cia + 4;
  }
  if (!ppe.check_branch_condition(info.bo, info.bi)) return;
  fmt::println("[BCCTR] entered");

  ppe.nia = (ppe.ctr >> 2) << 2;

  if (stub_map.contains(ppe.nia)) {
    const auto& [function_str, nid, func] = stub_map.at(ppe.nia);
    const auto& [name, ptr, toc]          = nid_table_map.at(nid);
    fmt::println("{}", name);
    // ppe.set_reg(2, toc);

    ptr(ppe);

    return;
  }

  fmt::println("calculated NIA: {:#016x}", ppe.nia);

  fmt::println("end of branch BCCTR");
};

void PPC::AND(InstructionInfo& info, PPE& ppe) {
  ppe.set_reg(info.ra, ppe.get_reg(info.rs) & ppe.get_reg(info.rb));

  if (info.rc) FATAL();
}

void PPC::XOR(InstructionInfo& info, PPE& ppe) {
  ppe.set_reg(info.ra, ppe.get_reg(info.rs) ^ ppe.get_reg(info.rb));

  if (info.rc) FATAL();
}

void PPC::NEG(InstructionInfo& info, PPE& ppe) {
  ppe.set_reg(info.rt, ~(ppe.get_reg(info.ra)) + 1);

  if (info.rc || info.oe) FATAL();
}

void PPC::bc(InstructionInfo& info, PPE& ppe) {
  (void)info;
  (void)ppe;
  FATAL();
};
void PPC::sc(InstructionInfo& info, PPE& ppe) {
  (void)info;
  (void)ppe;
  lv2_handle_syscall(ppe);
  fmt::println("[SC] r11: {:#06x}", ppe.get_reg(11));
};
void PPC::b(InstructionInfo& info, PPE& ppe) {
  (void)info;
  (void)ppe;
  FATAL();
};
void PPC::rlwimi(InstructionInfo& info, PPE& ppe) {
  (void)info;
  (void)ppe;
  FATAL();
};
void PPC::rlwinm(InstructionInfo& info, PPE& ppe) {
  fmt::println("RS: {}: {:#018x}", info.rs, ppe.get_reg(info.rs));
  fmt::println("RA: {}: {:#018x}", info.ra, ppe.get_reg(info.ra));

  u32 rot_val = ppe.get_reg(info.rs) & 0xFFFFFFFF;

  u32 r = std::rotl(rot_val, info.sh);

  fmt::println("MB+32: {}", 31 - info.mb);
  fmt::println("ME+32: {}", 31 - info.me);

  u64 m = MASK(31 - info.me, 31 - info.mb);

  fmt::println("MASK: {:#066b}", m);
  ppe.set_reg(info.ra, r & m);
  fmt::println("result: {:#018x}", r & m);

  if (info.rc) FATAL();
};
void PPC::ori(InstructionInfo& info, PPE& ppe) {
  u64 or_val = info.ui;
  fmt::println("or val: {:#010x}", or_val);

  ppe.gpr.at(info.ra) = (ppe.gpr.at(info.rs) | (or_val & 0xFFFF));
};
void PPC::oris(InstructionInfo& info, PPE& ppe) {
  u64 or_val = (info.ui & 0xFFFF) << 16;
  fmt::println("or val: {:#010x}", or_val);

  ppe.gpr.at(info.ra) = ppe.gpr.at(info.rs) | or_val;
};
void PPC::xori(InstructionInfo& info, PPE& ppe) {
  (void)info;
  (void)ppe;
  FATAL();
};
void PPC::xoris(InstructionInfo& info, PPE& ppe) {
  (void)info;
  (void)ppe;
  FATAL();
};
void PPC::andi(InstructionInfo& info, PPE& ppe) {
  (void)info;
  (void)ppe;
  FATAL();
};
void PPC::andis(InstructionInfo& info, PPE& ppe) {
  (void)info;
  (void)ppe;
  FATAL();
}
void PPC::mfspr(InstructionInfo& info, PPE& ppe) {
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
      fmt::println("CTR: {:#016x}", ppe.ctr);
      ppe.gpr.at(info.rt) = ppe.ctr;
      break;
    }

    default: FATAL();
  }
}

void PPC::OR(InstructionInfo& info, PPE& ppe) {
  ppe.set_reg(info.ra, ppe.get_reg(info.rs) | ppe.get_reg(info.rb));

  if (info.rc) FATAL();
}

void PPC::add(InstructionInfo& info, PPE& ppe) {
  ppe.set_reg(info.rt, ppe.get_reg(info.ra) + ppe.get_reg(info.rb));

  if (info.rc || info.oe) FATAL();
}

void PPC::NOR(InstructionInfo& info, PPE& ppe) {
  ppe.set_reg(info.ra, ~((ppe.get_reg(info.rs) | ppe.get_reg(info.rb))));

  if (info.rc) FATAL();
}

void PPC::subf(InstructionInfo& info, PPE& ppe) {
  fmt::println("RA: {:#018x}", ppe.get_reg(info.ra));
  fmt::println("RB: {:#018x}", ppe.get_reg(info.rb));
  fmt::println("op: {:#018x}", info.full_opcode);

  ppe.set_reg(info.rt, ~(ppe.get_reg(info.ra)) + ppe.get_reg(info.rb) + 1ULL);

  fmt::println("{:#018x}", ppe.get_reg(info.ra));

  if (info.rc || info.oe) FATAL();
}
void PPC::mtspr(InstructionInfo& info, PPE& ppe) {
  SPR spr = (SPR)((info.d >> 5) & 0b11111);

  switch (spr) {
    case SPR::XER: {
      ppe.xer = ppe.get_reg(info.rs);
      break;
    }
    case SPR::LR: {
      fmt::println("old LR: {:#016x} new LR:{:#016x}", ppe.lr, ppe.get_reg(info.rs));

      ppe.lr = ppe.get_reg(info.rs);
      break;
    }
    case SPR::CTR: {
      ppe.ctr = ppe.get_reg(info.rs);
      fmt::println("mtctr");
      break;
    }

    default: FATAL();
  }
}

void PPC::mfcr(InstructionInfo& info, PPE& ppe) {
  fmt::println("MFCR: cr: {:#016x}", ppe.cr);
  ppe.set_reg(info.rt, ppe.cr);
}

void PPC::mtocrf(InstructionInfo& info, PPE& ppe) {
  fmt::println("FXM: {}", info.fxm);
  fmt::println("R12: {}", ppe.get_reg(12));

  enum FXM_TO_CR {
    CR7 = 1 << 0,
    CR6 = 1 << 1,
    CR5 = 1 << 2,
    CR4 = 1 << 3,
    CR3 = 1 << 4,
    CR2 = 1 << 5,
    CR1 = 1 << 6,
    CR0 = 1 << 7,
  };

  int field = 0;

  switch (info.fxm) {
    case 0x1: field = 7; break;
    case 0x2: field = 6; break;
    case 0x4: field = 5; break;
    case 0x8: field = 4; break;
    case 0x10: field = 3; break;
    case 0x20: field = 2; break;
    case 0x40: field = 1; break;
    case 0x80: field = 0; break;
    default: FATAL();
  }

  int shift = (7 - field) * 4;
  ppe.cr    = (ppe.cr & ~(0xF << shift)) | ((ppe.get_reg(info.rs) >> shift) & 0xF) << shift;
}
