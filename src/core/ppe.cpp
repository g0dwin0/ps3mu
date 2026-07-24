#include "ppe.hpp"

#include <algorithm>
#include <bit>

#include "common.hpp"
#include "common/defs.hpp"
#include "ppc/parser.hpp"
#include "ppc/ppc.hpp"
#include "spdlog/fmt/bundled/base.h"
#include "spdlog/fmt/bundled/format.h"
u32 PPE::fetch(u64 addr) const {
  addr = vmm.virt_to_phys(addr);

  return read_p_be<u32>(*(u32*)(this->bus->memory.data() + addr));
}

void PPE::step() {
  global_step_count++;
  cia                      = nia;
  u64 opcode               = fetch(cia);
  instruction_info.address = cia;
  nia                      = cia + 4;

  // // spdlog::info("CIA: {:#016x}", cia);
  decode(opcode, instruction_info);
  execute(instruction_info);
}

void PPE::execute(PPC::InstructionInfo& info) {
  spdlog::info("{:#016x} - {}", info.address, info.mnemnonic);

  switch (info.opcode) {
    case 2:
    case 3: {
      assert(0);
      break;
    }

    case MULLI: {
      PPC::mulli(info, *this);
      break;
    }
    case SUBFIC: {
      PPC::subfic(info, *this);
      break;
    }

    case 10: {
      PPC::cmpli(info, *this);
      break;
    }
    case CMPI: {
      PPC::cmpi(info, *this);
      break;
    }
    case ADDIC: {
      PPC::addic(info, *this);
      break;
    }
      // case ADDIC+1: {
      //   PPC::addic(info, *this);
      //   break;
      // }

    case 18: {
      PPC::branch(info, *this);
      break;
    }

    case ADDI: {
      PPC::addi(info, *this);
      break;
    }
    case ADDIS: {
      PPC::addis(info, *this);
      break;
    }

    case 21: {
      PPC::rlwinm(info, *this);
      break;
    }

    case ORI: {
      PPC::ori(info, *this);
      break;
    }

    case ORIS: {
      PPC::oris(info, *this);
      break;
    }

    case XORIS: {
      PPC::xoris(info, *this);
      break;
    }

    case 30: {
      if (info.rc) FATAL();
      // u8 instruction_bits = PPC::get_bits<u8, u32>(info.full_opcode, {27, 30});

      u8 rotate_type = PPC::get_bits<u8, u32>(info.full_opcode, {27, 30});

      switch (rotate_type) {
        case 0:
        case 1: {  // rldicl

          u64 r = std::rotl(get_gpr(info.rs), info.sh);
          // spdlog::info("sh: {:#06x}", info.sh);
          // spdlog::info("mb: {:#06x}", info.mb);

          u64 m = MASK(0, 63 - info.mb);
          // spdlog::info("mask: {:#064b} - mb: {} - res: {}", m, info.mb, r & m);
          set_gpr(info.ra, r & m);
          break;
        }
        case 2:
        case 3: {  // rldicr
          // spdlog::info("info.rs: {:#016x}", get_gpr(info.rs));
          // spdlog::info("info.ra: {:#016x}", get_gpr(info.ra));
          u64 r = std::rotl(get_gpr(info.rs), info.sh);
          // spdlog::info("sh: {:#06x}", info.sh);
          // spdlog::info("me: {:#06x}", info.me);

          u64 m = MASK(63 - info.me, 63);
          // spdlog::info("MASK: {:#066b}", m);
          set_gpr(info.ra, r & m);
          break;
        }

        case 4:
        case 5: {  // rldic
          // spdlog::info("info.rs: {:#016x}", get_gpr(info.rs));
          // spdlog::info("info.ra: {:#016x}", get_gpr(info.ra));
          u64 r = std::rotl(get_gpr(info.rs), info.sh);
          // spdlog::info("sh: {:#06x}", info.sh);
          // spdlog::info("mb: {:#06x}", info.mb);

          u64 m = MASK(info.sh, 63 - info.mb);
          set_gpr(info.ra, r & m);
          // FATAL();

          break;
        }
        case 6:
        case 7: {  // rldimi
          u64 r = std::rotl(get_gpr(info.rs), info.sh);
          // spdlog::info("sh: {:#06x}", info.sh);
          // spdlog::info("mb: {:#06x}", info.mb);

          u64 m = MASK(info.sh, 63 - info.mb);

          set_gpr(info.ra, (r & m) | (get_gpr(info.ra) & (~m)));  // TODO: sus

          if (info.rc) FATAL("RC");
          // spdlog::info("post ra: {:#016x}", get_gpr(info.ra));
          // FATAL();
          break;
        }

        case 8: {  // rldcl
          u8 n = get_gpr(info.rb) & 0xFF;

          u64 r = std::rotl<u64>(get_gpr(info.rs), n);

          u64 m = MASK(0, 63 - info.mb);
          set_gpr(info.ra, r & m);  // TODO: sus
          break;
        }
        case 9: {  // rldcr
          u8 n = get_gpr(info.rb) & 0xFF;

          u64 r = std::rotl<u64>(get_gpr(info.rs), n);

          u64 m = MASK(63 - info.me, 63);
          set_gpr(info.ra, r & m);  // TODO: sus
          break;
        }

        default: {
          // break;
          FATAL(std::format("bits: {}", rotate_type));
        }
      }
      break;
    }
    case FX_EXT_OPS: {
      u16 instruction_bits = PPC::get_bits<u16, u32>(info.full_opcode, {21, 30});

      switch (instruction_bits) {
        case 0: {
          PPC::cmp(info, *this);
          break;
        }

        case SUBFC: {
          PPC::subfc(info, *this);
          break;
        }

        case MFSPR: {
          PPC::mfspr(info, *this);
          break;
        }

        case MTSPR: {
          PPC::mtspr(info, *this);
          break;
        }

        case ADDC: {
          PPC::addc(info, *this);
          break;
        }

        case _OR: {
          PPC::OR(info, *this);
          break;
        }

        case EXTSW: {
          EXTSx<u32>(info, *this);
          break;
        }

        case EXTSH: {
          EXTSx<u16>(info, *this);
          break;
        }

        case EXTSB: {
          EXTSx<u8>(info, *this);
          break;
        }

        case 19: {
          PPC::mfcr(info, *this);
          break;
        }

        case LDX: {
          gen_load_indexed_gpr<u64, false, false>(info, *this);
          break;
        }

        case LBZX: {
          gen_load_indexed_gpr<u8, false, false>(info, *this);
          break;
        }
        case LHAX: {
          gen_load_indexed_gpr<u16, true, false>(info, *this);
          break;
        }
        case LHZX: {
          gen_load_indexed_gpr<u16, false, false>(info, *this);
          break;
        }

        case LWZX: {
          gen_load_indexed_gpr<u32, false, false>(info, *this);
          break;
        }

        case LWAX: {
          gen_load_indexed_gpr<u32, true, false>(info, *this);
          break;
        }

        case LBZUX: {
          gen_load_indexed_gpr<u8, false, true>(info, *this);
          break;
        }
        case LHZUX: {
          gen_load_indexed_gpr<u16, false, true>(info, *this);
          break;
        }
        case LWZUX: {
          gen_load_indexed_gpr<u32, false, true>(info, *this);
          break;
        }

        case LHAUX: {
          gen_load_indexed_gpr<u16, true, true>(info, *this);
          break;
        }

        case LWAUX: {
          gen_load_indexed_gpr<u32, true, true>(info, *this);
          break;
        }

        case LDUX: {
          gen_load_indexed_gpr<u64, false, true>(info, *this);
          break;
        }
        case STBX: {
          gen_store_indexed<u8, false, false>(info, *this);
          break;
        }
        case STHX: {
          gen_store_indexed<u16, false, false>(info, *this);
          break;
        }

        case STWX: {
          gen_store_indexed<u32, false, false>(info, *this);
          break;
        }
        case STDX: {
          gen_store_indexed<u64, false, false>(info, *this);
          break;
        }

        case STBUX: {
          gen_store_indexed<u8, false, true>(info, *this);
          break;
        }
        case STHUX: {
          gen_store_indexed<u16, false, true>(info, *this);
          break;
        }

        case STWUX: {
          gen_store_indexed<u32, false, true>(info, *this);
          break;
        }
        case STDUX: {
          gen_store_indexed<u64, false, true>(info, *this);
          break;
        }

        case 32: {  // cmpl
          // PPC::cmpl(info, *this);
          // spdlog::info("doing comp");
          u64 a = 0;
          u64 b = 0;

          if (info.l == 0) {
            a = get_gpr(info.ra) & 0xFFFFFFFF;
            b = get_gpr(info.rb) & 0xFFFFFFFF;
          } else {
            a = get_gpr(info.ra);
            b = get_gpr(info.rb);
          }

          set_cr_field(info.bf, a < b ? CR_LT : a > b ? CR_GT : CR_EQ);

          // spdlog::info("[post cmp] CR = {:#016x}", cr);
          break;
        }

        case 28: {
          PPC::AND(info, *this);
          break;
        }
        case 104: {
          PPC::NEG(info, *this);
          break;
        }
        case 316: {
          PPC::XOR(info, *this);
          break;
        }

        case 144: {
          if (PPC::get_bits<u8, u32>(info.full_opcode, {11, 11}) == 0) {
            PPC::mtcrf(info, *this);
          } else {
            PPC::mtocrf(info, *this);
          }
          break;
        }

        case 138:
        case 266: {
          PPC::add(info, *this);
          break;
        }

        case 278: {
          // TODO: implement dcbt?
          break;
        }

        case NOR: {
          PPC::NOR(info, *this);

          break;
        }

        case CNTLZD: {
          PPC::CNTLZx<u64>(info, *this);
          break;
        }
        case CNTLZW: {
          PPC::CNTLZx<u32>(info, *this);
          break;
        }
        
        case SUBF: {
          PPC::subf(info, *this);
          break;
        }
        case SUBFE: {
          PPC::subfe(info, *this);
          break;
        }


        case SRD: {
          shift<Right, u64>(info, *this);
          break;
        }
        case SRW: {
          shift<Right, u32>(info, *this);
          break;
        }

        case SLD: {
          shift<Left, u64>(info, *this);
          break;
        }
        case SLW: {
          shift<Left, u32>(info, *this);
          break;
        }

        case MULLW: {
          PPC::mullw(info, *this);
          break;
        }
        case MULLD: {
          PPC::mulld(info, *this);
          break;
        }

        case MULHW: {
          PPC::mulhw(info, *this);
          break;
        }
        case MULHD: {
          PPC::mulhd(info, *this);
          break;
        }

        case DIVW: {
          div<i32, PPC::Signed>(info, *this);
          break;
        }
        case DIVD: {
          div<i64, PPC::Signed>(info, *this);
          break;
        }

        case DIVWU: {
          div<u32, PPC::Unsigned>(info, *this);
          break;
        }
        case DIVDU: {
          div<u64, PPC::Unsigned>(info, *this);
          break;
        }

        case SRAWI: {
          shift_algebraic<Right, u32>(info, *this);
          // FATAL();
          break;
        }
        case ADDME: {
          PPC::subf(info, *this);
          break;
        }
        case SUBFME: {
          PPC::subf(info, *this);
          break;
        }
        case ADDZ: {
          PPC::subf(info, *this);
          break;
        }
        case SUBFZE: {
          PPC::subf(info, *this);
          break;
        }
        
        
        

        default: FATAL(fmt::format("unhandled exec: {}", instruction_bits));
      }
      break;
    }

    case 16: {
      PPC::branch_conditional(info, *this);
      break;
    }

    case 32: {
      gen_load<u32, false, false, GPR>(info, *this);
      break;
    }
    case 34: {
      gen_load<u8, false, false, GPR>(info, *this);
      break;
    }
    case 40: {
      gen_load<u16, false, false, GPR>(info, *this);
      break;
    }
    case LHA: {
      gen_load<u16, false, true, GPR>(info, *this);
      break;
    }

    case LBZU: {
      gen_load<u8, true, false, GPR>(info, *this);
      break;
    }

    case LHZU: {
      gen_load<u16, true, false, GPR>(info, *this);
      break;
    }

    case LFD: {
      gen_load<u64, false, false, FPR>(info, *this);
      break;
    }
    case LFDU: {
      gen_load<u64, true, false, FPR>(info, *this);
      break;
    }

    case STFD: {
      gen_store<u64, false, FPR>(info, *this);
      break;
    }
    case STFDU: {
      gen_store<u64, true, FPR>(info, *this);
      break;
    }

    case STBU: {
      gen_store<u8, true, GPR>(info, *this);
      break;
    }

    case 58: {                                                               // LD[U]
      if (PPC::get_bits<u8, u32>(info.full_opcode, {30, 31}) == 1) {         // LDU
        gen_load<u64, true, false, GPR>(info, *this);                        // update is ignored, can't derive this at compile time
      } else if (PPC::get_bits<u8, u32>(info.full_opcode, {30, 31}) == 0) {  // LD
        gen_load<u64, false, false, GPR>(info, *this);                       // update is ignored, can't derive this at compile time
      } else {
        gen_load<u32, false, true, GPR>(info, *this);  // LWA
      }
      break;
    }

    case SC: {
      PPC::sc(info, *this);
      break;
    }
    case STW: {
      PPC::gen_store<u32, false, GPR>(info, *this);
      break;
    }
    case STB: {
      PPC::gen_store<u8, false, GPR>(info, *this);
      break;
    }
    case STH: {
      PPC::gen_store<u16, false, GPR>(info, *this);
      break;
    }
    case STHU: {
      PPC::gen_store<u16, true, GPR>(info, *this);
      break;
    }
    case STWU: {
      PPC::gen_store<u32, true, GPR>(info, *this);
      break;
    }

    case STDU: {
      if (info.xo > 1) FATAL();

      if (info.xo == 0) {  // STD
        PPC::gen_store<u64, false, GPR>(info, *this);
      } else {  // STDU
        PPC::gen_store<u64, true, GPR>(info, *this);
      }
      break;
    }

    case LWZU: {
      PPC::gen_load<u32, true, false, GPR>(info, *this);
      break;
    }

    case 19: {
      // see ppc book1 (cr ops table 16)

      u16 instruction_bits = PPC::get_bits<u16, u32>(info.full_opcode, {21, 30});

      // spdlog::info("INSTRUCTION BITS: {}", instruction_bits);
      switch (instruction_bits) {
        case 16: {
          PPC::bclr(info, *this);
          break;
        }

        case 528: {
          PPC::bcctr(info, *this);
          break;
        }

        default: {
          FATAL("UNIMPLEMENTED CR OP");
          break;
        }
      }
      break;
    }

    case XORI: {
      PPC::xori(info, *this);
      break;
    }
    default: {
      FATAL(fmt::format("could not execute opcd: {} full opcode: {:#016x}", info.opcode, info.full_opcode));
      break;
    }
  }
}

void PPE::set_cr_field(u8 crfield, u8 value) {
  const u8 sh = 4 * (7 - crfield);
  value       = (value & 0xE) | ((xer >> 31) & 1);
  cr          = (cr & ~(0xFu << sh)) | (u32(value) << sh);
}

void PPE::clear_bit_cr_reg(u8 cr_n, u8 bit) {
  assert(bit < 4);

  u8 offset = (cr_n * 4) + bit;

  cr &= ~(1 << offset);
}

void PPE::clear_full_cr_reg(u8 cr_n) {
  switch (cr_n) {
    case 0: {
      cr0 = 0;
      break;
    }
    case 1: {
      cr1 = 0;
      break;
    }
    case 2: {
      cr2 = 0;
      break;
    }
    case 3: {
      cr3 = 0;
      break;
    }
    case 4: {
      cr4 = 0;
      break;
    }
    case 5: {
      cr5 = 0;
      break;
    }
    case 6: {
      cr6 = 0;
      break;
    }
    case 7: {
      cr7 = 0;
      break;
    }
    default: assert(0);
  }
}

bool PPE::test_bit_cr_reg(u8 bit) { return cr & (1ull << (31 - bit)); }
void PPE::set_carry() { xer |= 1ull << 34; }
void PPE::reset_carry() { xer &= ~(1ull << 34); }
bool PPE::carry_is_set() { return (xer & (1ull << 34)) != 0; }

bool PPE::check_branch_condition(u8 bo, u8 bi) {
  // // spdlog::info("BO======{}", bo);
  // // spdlog::info("BI======{}", bi);
  const bool bo0 = (bo >> 4) & 1;
  const bool bo1 = (bo >> 3) & 1;
  const bool bo2 = (bo >> 2) & 1;
  const bool bo3 = (bo >> 1) & 1;

  if (!bo2) ctr -= 1;

  const bool ctr_ok  = bo2 || ((ctr != 0) != bo3);
  const bool cond_ok = bo0 || (((cr >> (31 - bi)) & 1) == bo1);

  return ctr_ok && cond_ok;
}