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
  decode(opcode, instruction_info);
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
      if (info.xo == 1) {
        PPC::gen_store(info, *this);
      } else {
        assert(0);
      }
      break;
    }

    default: assert(0 && info.opcode);
  }
}

constexpr u8 STORE_DOUBLEWORD = 0x3E;

void PPE::decode(u32 op, PPC::InstructionInfo& instruction_info) {
  fmt::println("decoding: {:#08x}", op);

  // spdlog::info("opcd: {:#010b}", op);
  // spdlog::info("get_bits_result: {}", get_bits<u32>(op, {0, 6}));
  // spdlog::info("get_bits_result: {}", get_bits<u32>(op, {6, 11}));

  // spdlog::info("opcd: {:#010b}", op);
  // op = read_p_be<u32>(op);
  // first establish which format is used
  // spdlog::info("opcd: {:#010b}", op);
  // spdlog::info("opcd: {:#010x}", (op >> 2) & 0x3f);
  // spdlog::info("rt: {:#010x}", (op >> 4) & 0x3f);
  // spdlog::info("ra: {:#010x}", (op & 0x3f) >> 2);
  // spdlog::info("xo: {:#010x}", (op & 0x3f) >> 2);

  u8 opcd = PPC::get_bits<u8, u32>(op, {0, 5});
  fmt::println("opcd: {:#010x} - {}", opcd, opcd);
  instruction_info.opcode = opcd;
  // spdlog::info("x: {:#010x}", opcd);

  PPC::InstructionFormat format = PPC::InstructionFormat::UNDEFINED;

  format = PPC::opcode_info_map.at(opcd).instruction_format;

  fmt::println("format: {}", PPC::instruction_format_str_map.at(format));

  switch (format) {
    case PPC::InstructionFormat::I_FORM: {
      fmt::println("decoding I_FORM instruction");
      instruction_info.li = PPC::get_bits<u8, u32>(op, {6, 29});
      instruction_info.aa = PPC::get_bits<u8, u32>(op, {30, 30});
      instruction_info.lk = PPC::get_bits<u8, u32>(op, {31, 31});

      break;
    }
    case PPC::InstructionFormat::B_FORM: {
      fmt::println("decoding B_FORM instruction");

      instruction_info.bo = PPC::get_bits<u8, u32>(op, {6, 10});
      instruction_info.bi = PPC::get_bits<u8, u32>(op, {11, 15});
      instruction_info.bd = PPC::get_bits<u8, u32>(op, {16, 29});
      instruction_info.aa = PPC::get_bits<u8, u32>(op, {30, 30});
      instruction_info.lk = PPC::get_bits<u8, u32>(op, {31, 31});

      break;
    }
    case PPC::InstructionFormat::SC_FORM:{assert(0); exit(-1); break;}
    case PPC::InstructionFormat::D_FORM: {
      fmt::println("decoding D_FORM instruction");
      instruction_info.rt = instruction_info.rs = instruction_info.to = instruction_info.frt = instruction_info.frs = PPC::get_bits<u8, u32>(op, {6, 10});
      instruction_info.ra                                                                                           = PPC::get_bits<u8, u32>(op, {11, 15});
      instruction_info.d = instruction_info.si = instruction_info.ui = PPC::get_bits<u8, u32>(op, {16, 31});

      switch (opcd) {
        case ADDI: {
          if (instruction_info.ra == 0) {
            instruction_info.mnemnonic = fmt::format("li r{}, {:#04x}", instruction_info.rt, instruction_info.si);
          } else {
            instruction_info.mnemnonic = fmt::format("addi r{},r{},{:#06x}", instruction_info.rt, instruction_info.ra, instruction_info.si);
          }
          break;
        }

        case 24: {
          instruction_info.mnemnonic = fmt::format("ori r{},r{},{:#04x}", instruction_info.ra, instruction_info.rs, instruction_info.ui);
          break;
        }

        case 25: {
          instruction_info.mnemnonic = fmt::format("oris r{},r{},{:#04x}", instruction_info.ra, instruction_info.rs, instruction_info.ui);
          break;
        }

        case 32: {
          instruction_info.mnemnonic = fmt::format("lwz r{},{:#04x}(r{})", instruction_info.rt, instruction_info.d, instruction_info.ra);
          break;
        }

        case 33: {
          instruction_info.mnemnonic = fmt::format("lwzi r{},{:#04x}(r{})", instruction_info.rt, instruction_info.d, instruction_info.ra);
          break;
        }

        default: {
          spdlog::error("no decoding logic for opcode: {}", opcd);
          assert(0);
        }
      }

      break;
    }
    case PPC::InstructionFormat::DS_FORM: {
      switch (opcd) {
        case STORE_DOUBLEWORD: {
          instruction_info.rt = instruction_info.rs = PPC::get_bits<u8, u32>(op, {6, 10});

          // 0b11111000001000011111111110000001
          instruction_info.ra = PPC::get_bits<u8, u32>(op, {11, 15});
          instruction_info.ds = PPC::get_bits<i16, u32>(op, {16, 29});
          instruction_info.xo = PPC::get_bits<u8, u32>(op, {30, 31});

          fmt::println("RT: {}", instruction_info.rt);
          fmt::println("RA: {}", instruction_info.ra);
          fmt::println("DS: {}", instruction_info.ds);
          fmt::println("XO: {}", instruction_info.xo);

          if (op & 1) {
            instruction_info.mnemnonic = fmt::format("stdu r{},{:#04x}(r{})", instruction_info.rs, (i16)(instruction_info.ds << 2), instruction_info.ra);
          } else {
            fmt::println("sum else");
          }

          break;
        }

        default: break;
      };

      break;
    }
    case PPC::InstructionFormat::A_FORM: {assert(0); exit(-1); break;}
    case PPC::InstructionFormat::M_FORM: {exit(-1); break;}
    case PPC::InstructionFormat::MD_FORM: {assert(0); exit(-1); break;}
    case PPC::InstructionFormat::MDS_FORM: {assert(0); exit(-1); break;}
    case PPC::InstructionFormat::X_FORM: {assert(0); exit(-1); break;}
    case PPC::InstructionFormat::XL_FORM: {assert(0); exit(-1); break;}
    case PPC::InstructionFormat::XFX_FORM: {assert(0); exit(-1); break;}
    case PPC::InstructionFormat::XFL_FORM: {assert(0); exit(-1); break;}
    case PPC::InstructionFormat::XS_FORM: {assert(0); exit(-1); break;}
    case PPC::InstructionFormat::XO_FORM: {assert(0); exit(-1); break;}
    case PPC::InstructionFormat::UNDEFINED: {assert(0); exit(-1); break;}
    default: {
      throw std::runtime_error(std::format("unimplemented decoder for undefined opcode: {:#04x}", opcd));
      break;
    }

    break;
  }
};