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
    ppe.set_gpr(info.rt, EXTS(15, info.si));
  } else {
    ppe.set_gpr(info.rt, ppe.get_gpr(info.ra) + EXTS(15, info.si));
  }
}

void PPC::addis(InstructionInfo& info, PPE& ppe) {
  if (info.ra == 0) {
    ppe.set_gpr(info.rt, EXTS(31, info.si << 16));
  } else {
    ppe.set_gpr(info.rt, ppe.get_gpr(info.ra) + EXTS(31, info.si << 16));
  }
}

void PPC::mulli(InstructionInfo& info, PPE& ppe) {
  i64 ra      = ppe.get_gpr(info.ra);
  i64 exts_si = EXTS(31, info.si);

  i128 result = ra * exts_si;

  result &= U64_MAX;
  ppe.set_gpr(info.rt, result);
}
void PPC::mullw(InstructionInfo& info, PPE& ppe) {
  (void)info;
  (void)ppe;
  i32 a = ppe.get_gpr(info.ra) & U32_MAX;
  i32 b = ppe.get_gpr(info.rb) & U32_MAX;

  spdlog::info("A: {:#016x}", a);
  spdlog::info("B: {:#016x}", b);
  spdlog::info("A+B: {:#016x}", (i64)a * b);

  ppe.set_gpr(info.rt, (i64)a * b);
}

void PPC::mulhw(InstructionInfo& info, PPE& ppe) {
  (void)info;
  (void)ppe;
  i32 a = ppe.get_gpr(info.ra) & U32_MAX;
  i32 b = ppe.get_gpr(info.rb) & U32_MAX;

  i64 product = (i64)a*b;
  u64 new_rt = ppe.get_gpr(info.rt);

  new_rt &= ~(U32_MAX);


  new_rt |= (product >> 32);

  spdlog::info("A: {:#016x}", a);
  spdlog::info("B: {:#016x}", b);
  spdlog::info("A+B: {:#016x}", (i64)a * b);

  ppe.set_gpr(info.rt, new_rt);
}




void PPC::mulld(InstructionInfo& info, PPE& ppe) {
  (void)info;
  (void)ppe;
  i128 res = (i64)ppe.get_gpr(info.ra) * (i64)ppe.get_gpr(info.rb);

  i64 low = (i64)res;
  // u64 high = res >> 64;

  // spdlog::info("ra: {:#016x}", ppe.get_reg(info.ra));
  // spdlog::info("rb: {:#016x}", ppe.get_reg(info.rb));
  //
  // spdlog::info("result: {:#032x}", res);
  // FATAL();

  ppe.set_gpr(info.rt, low);
}

void PPC::mulhd(InstructionInfo& info, PPE& ppe) {
  (void)info;
  (void)ppe;
  i128 res = (i64)ppe.get_gpr(info.ra) * (i64)ppe.get_gpr(info.rb);

  i64 high = res >> 64;

  // spdlog::info("ra: {:#016x}", ppe.get_reg(info.ra));
  // spdlog::info("rb: {:#016x}", ppe.get_reg(info.rb));
  //
  // spdlog::info("result: {:#032x}", res);
  // FATAL();

  ppe.set_gpr(info.rt, high);
}



void PPC::subfic(InstructionInfo& info, PPE& ppe) {
  unsigned long long junk = 0;

  bool carry = __builtin_uaddll_overflow(~(ppe.get_gpr(info.ra)), EXTS(15, info.si) + 1, &junk);

  ppe.set_gpr(info.rt, ~(ppe.get_gpr(info.ra)) + EXTS(15, info.si) + 1);

  carry ? ppe.set_carry() : ppe.reset_carry();
}

void PPC::dozi(InstructionInfo& info, PPE& ppe) {
  (void)info;
  (void)ppe;
  FATAL();
}

void PPC::cmpli(InstructionInfo& info, PPE& ppe) {
  u64 a = 0;

  if (info.l == 0) {
    a = ppe.get_gpr(info.ra) & 0xFFFFFFFF;
  } else {
    a = ppe.get_gpr(info.ra);
  }

  u64 comp_val = info.ui & 0xFFFF;

  ppe.set_cr_field(info.bf, a < comp_val ? CR_LT : a > comp_val ? CR_GT : CR_EQ);

  spdlog::info("CR POST CMPLI: {:#016x}", ppe.cr);
}

void PPC::cmpi(InstructionInfo& info, PPE& ppe) {
  (void)info;
  (void)ppe;

  i64 a = 0;

  if (info.l == 0) {
    a = (i64)EXTS(31, ppe.get_gpr(info.ra) & 0xFFFFFFFF);
  } else {
    a = ppe.get_gpr(info.ra);
  }

  u64 ext_si = EXTS(15, info.si);

  i64 s_si = (i64)ext_si;

  spdlog::info("A: {:#016x}", a);
  spdlog::info("INFO.BF: {:#016x}", info.bf);
  spdlog::info("EX_TSI: {:#016x}", ext_si);

  ppe.set_cr_field(info.bf, a < s_si ? CR_LT : a > s_si ? CR_GT : CR_EQ);

  spdlog::info("CR POST CMPI: {:#016x}", ppe.cr);
}

void PPC::cmp(InstructionInfo& info, PPE& ppe) {
  (void)info;
  (void)ppe;

  i64 a, b = 0;
  if (info.l == 0) {
    a = (i64)EXTS(31, ppe.get_gpr(info.ra) & 0xFFFFFFFF);
    b = (i64)EXTS(31, ppe.get_gpr(info.rb) & 0xFFFFFFFF);
    ppe.set_cr_field(info.bf, a < b ? CR_LT : a > b ? CR_GT : CR_EQ);

  } else {
    a = (i64)ppe.get_gpr(info.ra);
    b = (i64)ppe.get_gpr(info.rb);
    ppe.set_cr_field(info.bf, a < b ? CR_LT : a > b ? CR_GT : CR_EQ);
  }
};

void PPC::addic(InstructionInfo& info, PPE& ppe) {
  i64 result;

  result = (i64)ppe.get_gpr(info.ra);

  result += EXTS(15, info.si);

  ppe.set_gpr(info.rt, result);
}
void PPC::addc(InstructionInfo& info, PPE& ppe) {
  i64 a = ppe.get_gpr(info.ra);
  i64 b = ppe.get_gpr(info.rb);

  unsigned long carry_out = 0;
  i64 result              = __builtin_addcl(a, b, 0, &carry_out);

  ppe.set_gpr(info.rt, result);
  carry_out ? ppe.set_carry() : ppe.reset_carry();
}

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

  spdlog::info("calculated NIA: {:#016x}", target_addr);

  if (stub_map.contains(target_addr)) {
    const auto& [function_str, nid, func] = stub_map.at(target_addr);
    const auto& [name, ptr, toc]          = get_nid_map().at(nid);
    spdlog::info("{}", name);
    // ppe.set_reg(2, toc);

    ptr(ppe);

    return;
  }

  ppe.nia = target_addr;

  spdlog::info("end of branch");
}

void PPC::branch_conditional(InstructionInfo& info, PPE& ppe) {
  spdlog::info("[pre-branch] CR = {:#016x}", ppe.cr);
  if (info.lk) {
    ppe.lr = ppe.cia + 4;
  }
  if (!ppe.check_branch_condition(info.bo, info.bi)) return;

  if (stub_map.contains(info.target_addr)) {
    const auto& [function_str, nid, func] = stub_map.at(info.target_addr);
    const auto& [name, ptr, toc]          = get_nid_map().at(nid);
    spdlog::info("{}", name);
    // ppe.set_reg(2, toc);

    ptr(ppe);

    return;
  }

  ppe.nia = info.target_addr;

  spdlog::info("end of branch");
}

void PPC::bclr(InstructionInfo& info, PPE& ppe) {
  if (info.lk) {
    ppe.lr = ppe.cia + 4;
  }
  if (!ppe.check_branch_condition(info.bo, info.bi)) return;
  spdlog::info("[BCLR] entered");

  u64 calc_addr = (ppe.lr >> 2) << 2;

  if (stub_map.contains(ppe.nia)) {
    const auto& [function_str, nid, func] = stub_map.at(ppe.nia);
    const auto& [name, ptr, toc]          = get_nid_map().at(nid);
    spdlog::info("{}", name);
    // ppe.set_reg(2, toc);

    ptr(ppe);

    return;
  }

  ppe.nia = calc_addr;

  spdlog::info("end of branch BCLR");
}

void PPC::bcctr(InstructionInfo& info, PPE& ppe) {
  if (info.lk) {
    ppe.lr = ppe.cia + 4;
  }
  if (!ppe.check_branch_condition(info.bo, info.bi)) return;
  spdlog::info("[BCCTR] entered");

  ppe.nia = (ppe.ctr >> 2) << 2;

  if (stub_map.contains(ppe.nia)) {
    const auto& [function_str, nid, func] = stub_map.at(ppe.nia);
    const auto& [name, ptr, toc]          = get_nid_map().at(nid);
    spdlog::info("{}", name);
    // ppe.set_reg(2, toc);

    ptr(ppe);

    return;
  }

  spdlog::info("calculated NIA: {:#016x}", ppe.nia);

  spdlog::info("end of branch BCCTR");
}

void PPC::AND(InstructionInfo& info, PPE& ppe) {
  ppe.set_gpr(info.ra, ppe.get_gpr(info.rs) & ppe.get_gpr(info.rb));

  if (info.rc) FATAL();
}

void PPC::XOR(InstructionInfo& info, PPE& ppe) {
  ppe.set_gpr(info.ra, ppe.get_gpr(info.rs) ^ ppe.get_gpr(info.rb));

  if (info.rc) FATAL();
}

void PPC::NEG(InstructionInfo& info, PPE& ppe) {
  ppe.set_gpr(info.rt, ~(ppe.get_gpr(info.ra)) + 1);

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
  spdlog::info("[SC] r11: {:#06x}", ppe.get_gpr(11));
}

void PPC::b(InstructionInfo& info, PPE& ppe) {
  (void)info;
  (void)ppe;
  FATAL();
}

void PPC::rlwimi(InstructionInfo& info, PPE& ppe) {
  (void)info;
  (void)ppe;
  FATAL();
}

void PPC::rlwinm(InstructionInfo& info, PPE& ppe) {
  spdlog::info("RS: {}: {:#018x}", info.rs, ppe.get_gpr(info.rs));
  spdlog::info("RA: {}: {:#018x}", info.ra, ppe.get_gpr(info.ra));

  u32 rot_val = ppe.get_gpr(info.rs) & 0xFFFFFFFF;

  u32 r = std::rotl(rot_val, info.sh);

  spdlog::info("MB+32: {}", 31 - info.mb);
  spdlog::info("ME+32: {}", 31 - info.me);

  u64 m = MASK(31 - info.me, 31 - info.mb);

  spdlog::info("MASK: {:#066b}", m);
  ppe.set_gpr(info.ra, r & m);
  spdlog::info("result: {:#018x}", r & m);

  if (info.rc) FATAL();
}
void PPC::ori(InstructionInfo& info, PPE& ppe) {
  u64 or_val = info.ui;
  // spdlog::info("or val: {:#010x}", or_val);

  ppe.set_gpr(info.ra, (ppe.get_gpr(info.rs) | (or_val & 0xFFFF)));
}
void PPC::oris(InstructionInfo& info, PPE& ppe) {
  u64 or_val = (info.ui & 0xFFFF) << 16;
  // spdlog::info("or val: {:#010x}", or_val);

  // ppe.gpr.at(info.ra) = ppe.gpr.at(info.rs) | or_val;

  ppe.set_gpr(info.ra, ppe.get_gpr(info.rs) | or_val);
}
void PPC::xori(InstructionInfo& info, PPE& ppe) {
  (void)info;
  (void)ppe;

  ppe.set_gpr(info.ra, ppe.get_gpr(info.rs) ^ (info.ui & 0xFFFF));
}
void PPC::xoris(InstructionInfo& info, PPE& ppe) {
  u64 or_val = (info.ui & 0xFFFF) << 16;

  // ppe.gpr.at(info.ra) = ppe.gpr.at(info.rs) ^ or_val;
  ppe.set_gpr(info.ra, ppe.get_gpr(info.rs) ^ or_val);
}
void PPC::andi(InstructionInfo& info, PPE& ppe) {
  (void)info;
  (void)ppe;
  FATAL();
}
void PPC::andis(InstructionInfo& info, PPE& ppe) {
  (void)info;
  (void)ppe;
  FATAL();
}
void PPC::mfspr(InstructionInfo& info, PPE& ppe) {
  SPR spr = (SPR)((info.d >> 5) & 0b11111);

  switch (spr) {
    case SPR::XER: {
      ppe.set_gpr(info.rt, ppe.xer);
      break;
    }
    case SPR::LR: {
      ppe.set_gpr(info.rt, ppe.lr);
      break;
    }
    case SPR::CTR: {
      spdlog::info("CTR: {:#016x}", ppe.ctr);
      ppe.set_gpr(info.rt, ppe.ctr);
      break;
    }

    default: FATAL();
  }
}

void PPC::OR(InstructionInfo& info, PPE& ppe) {
  ppe.set_gpr(info.ra, ppe.get_gpr(info.rs) | ppe.get_gpr(info.rb));

  if (info.rc) FATAL();
}

void PPC::add(InstructionInfo& info, PPE& ppe) {
  ppe.set_gpr(info.rt, ppe.get_gpr(info.ra) + ppe.get_gpr(info.rb));

  if (info.rc || info.oe) FATAL();
}

void PPC::NOR(InstructionInfo& info, PPE& ppe) {
  ppe.set_gpr(info.ra, ~((ppe.get_gpr(info.rs) | ppe.get_gpr(info.rb))));

  if (info.rc) FATAL();
}

void PPC::subf(InstructionInfo& info, PPE& ppe) {
  spdlog::info("RA: {:#018x}", ppe.get_gpr(info.ra));
  spdlog::info("RB: {:#018x}", ppe.get_gpr(info.rb));
  spdlog::info("op: {:#018x}", info.full_opcode);

  ppe.set_gpr(info.rt, ~(ppe.get_gpr(info.ra)) + ppe.get_gpr(info.rb) + 1ULL);

  spdlog::info("{:#018x}", ppe.get_gpr(info.ra));

  if (info.rc || info.oe) FATAL();
}

void PPC::subfe(InstructionInfo& info, PPE& ppe) {
  spdlog::info("RA: {:#018x}", ppe.get_gpr(info.ra));
  spdlog::info("RB: {:#018x}", ppe.get_gpr(info.rb));
  spdlog::info("op: {:#018x}", info.full_opcode);

  ppe.set_gpr(info.rt, ~(ppe.get_gpr(info.ra)) + ppe.get_gpr(info.rb) + 1ULL);

  spdlog::info("{:#018x}", ppe.get_gpr(info.ra));

  if (info.rc || info.oe) FATAL();
}



void PPC::subfc(InstructionInfo& info, PPE& ppe) {
  spdlog::info("RA: {:#018x}", ppe.get_gpr(info.ra));
  spdlog::info("RB: {:#018x}", ppe.get_gpr(info.rb));
  spdlog::info("op: {:#018x}", info.full_opcode);

  ppe.set_gpr(info.rt, ~(ppe.get_gpr(info.ra)) + ppe.get_gpr(info.rb) + 1ULL);

  // TODO: CA logic

  spdlog::info("{:#018x}", ppe.get_gpr(info.ra));

  if (info.rc || info.oe) FATAL();
}


void PPC::mtspr(InstructionInfo& info, PPE& ppe) {
  SPR spr = (SPR)((info.d >> 5) & 0b11111);

  switch (spr) {
    case SPR::XER: {
      ppe.xer = ppe.get_gpr(info.rs);
      break;
    }
    case SPR::LR: {
      spdlog::info("old LR: {:#016x} new LR:{:#016x}", ppe.lr, ppe.get_gpr(info.rs));

      ppe.lr = ppe.get_gpr(info.rs);
      break;
    }
    case SPR::CTR: {
      ppe.ctr = ppe.get_gpr(info.rs);
      spdlog::info("mtctr");
      break;
    }

    default: FATAL();
  }
}

void PPC::mfcr(InstructionInfo& info, PPE& ppe) {
  spdlog::info("MFCR: cr: {:#016x}", ppe.cr);
  ppe.set_gpr(info.rt, ppe.cr);
}

void PPC::mtcrf(InstructionInfo& info, PPE& ppe) {
  (void)(info);
  (void)(ppe);
  u8 full_4_bits = 0b1111;
  u64 mask       = 0;
  spdlog::info("fxm: {:#08b}", info.fxm);
  for (int i = 0; i < 8; i++) {
    if ((info.fxm & (1 << i)) != 0) {
      mask |= (full_4_bits << (i * 4));
    }
  }
  spdlog::info("mask: {:#066b}", mask);
  ppe.cr = ((ppe.get_gpr(info.rs) & 0xFFFFFFFF) & mask) | (ppe.cr & ~mask);
  // FATAL();
}
void PPC::mtocrf(InstructionInfo& info, PPE& ppe) {
  spdlog::info("FXM: {}", info.fxm);
  spdlog::info("R12: {}", ppe.get_gpr(12));

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
  ppe.cr    = (ppe.cr & ~(0xF << shift)) | ((ppe.get_gpr(info.rs) >> shift) & 0xF) << shift;
}
