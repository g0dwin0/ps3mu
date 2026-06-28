#include "ppe.hpp"

#include "common.hpp"
#include "common/defs.hpp"
#include "ppc/parser.hpp"
#include "ppc/ppc.hpp"

u32 PPE::fetch(u64 addr) {
  fmt::println("reading: {:#010x}", addr);
  return read_p_be<u32>(*(u32*)(this->bus->memory.data() + addr));
}

void PPE::step() {
  u64 opcode = fetch(nia);
  PPC::decode(opcode, instruction_info);
  fmt::println("opcode: {}", instruction_info.opcode);
  execute(instruction_info);

  nia += 4;
  cia = nia;
  // PPC::execute(opcode, instruction_info);
}

void PPE::execute(PPC::InstructionInfo& info) {
  fmt::println("{}", info.mnemnonic);
  switch (info.opcode) {
    case 2: {
      PPC::tdi(info, *this);
      break;
    }

    case 3: {
      PPC::twi(info, *this);
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

    case ADDI: {
      PPC::addi(info, *this);
      break;
    }
      // case 15: {
      //   PPC::addis(info, *this);
      //   break;
      // }

    case ORI: {
      PPC::ori(info, *this);
      break;
    }

    case ORIS: {
      PPC::oris(info, *this);
      break;
    }

    case 32:
    case 33:
    case 34:
    case 35:
    case 40:
    case 41:
    case 42:
    case 43: {
      PPC::gen_load(info, *this);
      break;
    };

    case 36:
    case 37:
    case 38:
    case 39:
    case 44:
    case 45:
    case 47: {
      PPC::gen_store(info, *this);
      break;
    };

    case 62: {
      if(info.xo == 1) {
        PPC::gen_store(info, *this);
      } else {
        assert(0);
      }
      break;
    }

    default: assert(0 && info.opcode);
  }
}