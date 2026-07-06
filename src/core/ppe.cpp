#include "ppe.hpp"

#include <bit>

#include "common.hpp"
#include "common/defs.hpp"
#include "ppc/parser.hpp"
#include "ppc/ppc.hpp"
#include "spdlog/fmt/bundled/format.h"
u32 PPE::fetch(u64 addr) {
  fmt::println("reading: {:#010x}", addr);
  addr = vmm.virt_to_phys(addr);

  return read_p_be<u32>(*(u32*)(this->bus->memory.data() + addr));
}

void PPE::step() {
  cia        = nia;
  u64 opcode = fetch(cia);

  nia = cia + 4;
  decode(opcode, instruction_info);
  fmt::println("opcode: {}", instruction_info.opcode);
  execute(instruction_info);

  // PPC::execute(opcode, instruction_info);
}

void PPE::execute(PPC::InstructionInfo& info) {
  fmt::println("{}", info.mnemnonic);
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

          u64 m = MASK(info.mb, 63);

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
          u64 a = 0;
          u64 b = 0;

          if (info.l == 0) {
            a = get_reg(info.ra) >> 32;
            b = get_reg(info.rb) >> 32;
          } else {
            a = get_reg(info.ra);
            b = get_reg(info.rb);
          }

          if (a > b) {
            set_bit_cr_reg(info.bf, 0);
          }

          if (a < b) {
            set_bit_cr_reg(info.bf, 1);
          }

          if (a == b) {
            set_bit_cr_reg(info.bf, 2);
          }

          // assert(0);
          break;
        }

        default: assert(0);
      }
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

    default: {
      FATAL(fmt::format("could not execute opcode: {:#016x}", info.full_opcode));
      break;
    }
  }
}

void PPE::set_bit_cr_reg(u8 cr_n, u8 bit) {
  assert(bit < 4);

  u8 offset = (cr_n * 4) + (u8)bit;

  cr |= (1 << offset);
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

bool PPE::test_bit_cr_reg(u8 bit) { return cr & (1 << bit); };