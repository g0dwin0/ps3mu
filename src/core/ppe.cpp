#include "ppe.hpp"

#include <bit>

#include "common.hpp"
#include "common/defs.hpp"
#include "ppc/parser.hpp"
#include "ppc/ppc.hpp"
#include "spdlog/fmt/bundled/base.h"
#include "spdlog/fmt/bundled/format.h"
u32 PPE::fetch(u64 addr) {
  fmt::println("CIA: {:#016x} reading: {:#010x}", cia, addr);
  addr = vmm.virt_to_phys(addr);

  return read_p_be<u32>(*(u32*)(this->bus->memory.data() + addr));
}

void PPE::step() {
  cia                      = nia;
  u64 opcode               = fetch(cia);
  instruction_info.address = cia;
  nia                      = cia + 4;

  if (cia == 0x103B4) {
    fmt::println("R8: {:#016x}", gpr[8]);
    fmt::println("R31: {:#016x}", gpr[31]);
    fmt::println("R10: {:#016x}", gpr[10]);
    fmt::println("R28: {:#016x}", gpr[28]);
  }
  decode(opcode, instruction_info);
  execute(instruction_info);
}

void PPE::execute(PPC::InstructionInfo& info) {
  fmt::println("{:#016x} - {}", info.address, info.mnemnonic);
  switch (info.opcode) {
    case 2:
    case 3: {
      assert(0);
      break;
    }

    case 7: {
      PPC::mulli(info, *this);
      break;
    }
    case 8: {
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
    case 12: {
      PPC::addic(info, *this);
      break;
    }

    case 18: {
      PPC::branch(info, *this);
      break;
    }

    case ADDI: {
      PPC::addi(info, *this);
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

    case 30: {
      // FATAL("not implemented");

      u8 instruction_bits = PPC::get_bits<u8, u32>(info.full_opcode, {27, 30});

      switch (instruction_bits) {
        case 0:
        case 1: {  // rldicl
          u64 r = std::rotl(get_reg(info.rs), info.sh);
          fmt::println("sh: {:#06x}", info.sh);
          fmt::println("mb: {:#06x}", info.mb);

          u64 m = MASK(info.mb, 63);
          // fmt::println("mask: {:#064b} - mb: {}", m, info.mb);
          set_reg(info.ra, r & m);

          if (instruction_bits == 1) {
            assert(0);
          }
          break;
        }

        case 2:
        case 3: {  // rldicr
          assert(0);
          break;
        }

        case 4:
        case 5: {  // rldic
          assert(0);
          break;
        }

        case 6:
        case 7: {  // rldimi
          assert(0);
          break;
        }

        case 8:
        case 9: {  // rldcl
          assert(0);
          break;
        }

        default: assert(0);
      }
      break;
    }
    case FX_EXT_OPS: {
      u16 instruction_bits = PPC::get_bits<u16, u32>(info.full_opcode, {21, 30});

      switch (instruction_bits) {
        case MFSPR: {
          PPC::mfspr(info, *this);
          break;
        }

        case MTSPR: {
          PPC::mtspr(info, *this);
          break;
        }

        case _OR: {
          PPC::OR(info, *this);
          break;
        }

        case EXTSW: {
          assert(0);
          break;
        }

          // static constexpr u16 X

        case 32: {
          fmt::println("doing comp");
          u64 a = 0;
          u64 b = 0;

          if (info.l == 0) {
            // FATAL();
            a = get_reg(info.ra) & 0xFFFFFFFF;
            b = get_reg(info.rb) & 0xFFFFFFFF;
          } else {
            a = get_reg(info.ra);
            b = get_reg(info.rb);
          }

          if (a < b) {
            set_bit_cr_reg(info.bf, 0);
          } else if (a > b) {
            set_bit_cr_reg(info.bf, 1);
          } else {
            set_bit_cr_reg(info.bf, 2);
          }

          fmt::println("[post cmp] CR = {:#016x}", cr);

          // assert(0);
          break;
        }

        default: assert(0);
      }
      break;
    }

    case 16: {
      PPC::branch_conditional(info, *this);
      break;
    }

    case 32:
    case 33:
    case 34:
    case 35:
    case 40:
    case 41:
    case 42:
    case 43:
    case 58: {
      PPC::gen_load(info, *this);
      break;
    };

    case SC: {
      PPC::sc(info, *this);
      break;
    }
    case 36:
    case 37:
    case 38:
    case 39:
    case 44:
    case 45:
    case 47:
    case 62: {
      PPC::gen_store(info, *this);
      break;
    };

    case 19: {
      // see ppc book1 (cr ops table 16)

      u16 instruction_bits = PPC::get_bits<u16, u32>(info.full_opcode, {21, 30});

      fmt::println("INSTRUCTION BITS: {}", instruction_bits);
      switch (instruction_bits) {
        case 16: {
          PPC::bclr(info, *this);
          break;
        }

        default: {
          FATAL("UNIMPLEMENTED CR OP");
          break;
        }
      }
      break;
    }
    default: {
      FATAL(fmt::format("could not execute opcd: {} full opcode: {:#016x}", info.opcode, info.full_opcode));
      break;
    }
  }
}

void PPE::set_bit_cr_reg(u8 crfield, u8 bit) {
  u8 base = crfield * 4;
  // clear all 4 bits of this field first
  cr &= ~(0xFull << (28 - base));  // adjust shift to match your bit ordering
  cr |= (1ull << (31 - (base + bit)));
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

bool PPE::test_bit_cr_reg(u8 bit) { return cr & (1ull << (31 - bit)); };

bool PPE::check_branch_condition(u8 BO, u8 BI) {
  fmt::println("BO======{}", BO);
  fmt::println("BI======{}", BI);

  //   Decrement the CTR, then branch if the decremented CTRM:63!=0 and CRBI=0
  if (BO <= 1) {
    fmt::println("PC");
    ctr--;
    if (ctr != 0 && !test_bit_cr_reg(BI)) return true;
  }

  //   Decrement the CTR, then branch if the
  // decremented CTRM:63=0 and CRBI=0
  if ((BO >> 1) == 1) {
    fmt::println("PB");
    ctr--;
    if (ctr == 0 && !test_bit_cr_reg(BI)) return true;
  }

  // Branch if CRBI=0
  if ((BO >> 2) == 1) {
    fmt::println("PA");
    return !test_bit_cr_reg(BI);
  }

  //   Decrement the CTR, then branch if the
  // decremented CTRM:63!=0 and CRBI=1
  if ((BO >> 3) == 1) {
    ctr--;
    if (ctr != 0 && test_bit_cr_reg(BI)) return true;
  }

  //   Decrement the CTR, then branch if the
  // decremented CTRM:63=0 and CRBI=1
  if ((BO >> 3) == 1) {
    ctr--;
    if (ctr == 0 && test_bit_cr_reg(BI)) return true;
  }

  // Branch if CRBI=1
  if ((BO >> 2) == 3) {
    return test_bit_cr_reg(BI);
  }

  //   Decrement the CTR, then branch if the
  if ((BO & 0b10110) == 16) {
    fmt::println("Z");
    ctr--;
    return ctr != 0;
  }

  // Decrement the CTR, then branch if the
  if ((BO & 0b10110) == 18) {
    ctr--;
    return ctr == 0;
  }

  fmt::println("always branch!");
  // always branch
  return true;
}