#include "ppe.hpp"

#include "common.hpp"
#include "common/defs.hpp"
#include "ppc/parser.hpp"
#include "ppc/ppc.hpp"

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

    case 18: {
      PPC::branch(info, *this);
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

    case 30: {
      // FATAL("not implemented");

      u8 instruction_bits = PPC::get_bits<u8, u32>(info.full_opcode, {27, 30});

      switch (instruction_bits) {
        case 0:
        case 1: {  // rldicl
          break;
        }

        case 2:
        case 3: {  // rldicr
          break;
        }

        case 4:
        case 5: {  // rldic
          break;
        }

        case 6:
        case 7: {  // rldimi
          break;
        }

        case 8:
        case 9: {  // rldcl
          break;
        }

        default: assert(0);
      }
      break;
    }
    case 31: {
      u16 instruction_bits = PPC::get_bits<u16, u32>(info.full_opcode, {21, 30});

      switch (instruction_bits) {
        case 339: {
          PPC::mfspr(info, *this);
          break;
        }

        case 467: {
          PPC::mtspr(info, *this);
          break;
        }

        case 444: {
          PPC::OR(info, *this);
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
    case 47:
    case 62: {
      PPC::gen_store(info, *this);
      break;
    };

    default: assert(0 && info.opcode);
  }
}

constexpr u8 STORE_DOUBLEWORD = 0x3E;

void PPE::decode(u32 op, PPC::InstructionInfo& instruction_info) {
  u8 opcd = PPC::get_bits<u8, u32>(op, {0, 5});

  fmt::println("decoding: {:#08x}", op);
  fmt::println("opcd: {:#010x} - {}", opcd, opcd);

  instruction_info.opcode      = opcd;
  instruction_info.full_opcode = op;

  PPC::InstructionFormat format = PPC::InstructionFormat::UNDEFINED;

  switch (opcd) {
    case 30: {
      u8 instruction_bits = PPC::get_bits<u8, u32>(op, {27, 30});
      fmt::println("instruction bits: {}", instruction_bits);

      if (instruction_bits <= 7) {
        format = PPC::InstructionFormat::MD_FORM;
      } else {
        format = PPC::InstructionFormat::MDS_FORM;
      }

      fmt::println("format: {}", PPC::instruction_format_str_map.at(format));
      break;
    }
    case 31: {
      u16 instruction_bits = PPC::get_bits<u16, u32>(op, {21, 30});
      fmt::println("instruction bits: {}", instruction_bits);
      format = PPC::ext_op_31_opcode_info_map.at(instruction_bits).instruction_format;
      fmt::println("format: {}", PPC::instruction_format_str_map.at(format));
      break;
    }

    default: {
      format = PPC::opcode_info_map.at(opcd).instruction_format;
      fmt::println("format: {}", PPC::instruction_format_str_map.at(format));
      break;
    }
  }

  switch (format) {
    case PPC::InstructionFormat::I_FORM: {
      fmt::println("decoding I_FORM instruction");
      instruction_info.li = PPC::get_bits<u8, u32>(op, {6, 29});
      instruction_info.aa = PPC::get_bits<u8, u32>(op, {30, 30});
      instruction_info.lk = PPC::get_bits<u8, u32>(op, {31, 31});
      u8 branch_form      = PPC::get_bits<u8, u32>(op, {30, 31});

      fmt::println("li: {:#016x}", instruction_info.li);
      fmt::println("aa: {:#016x}", instruction_info.aa);
      fmt::println("lk: {:#016x}", instruction_info.lk);
      fmt::println("branch form: {}", PPC::branch_form_type_table.at(branch_form));

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
    case PPC::InstructionFormat::SC_FORM: {
      assert(0);
      exit(-1);
      break;
    }
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

        case 36: {
          instruction_info.mnemnonic = fmt::format("stw r{},{:#04x}(r{})", instruction_info.rs, instruction_info.d, instruction_info.ra);
          break;
        }
        case 37: {
          instruction_info.mnemnonic = fmt::format("stwu r{},{:#04x}(r{})", instruction_info.rs, instruction_info.d, instruction_info.ra);
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
          instruction_info.ds = (i16)(PPC::get_bits<u16, u32>(op, {16, 29}));
          instruction_info.xo = PPC::get_bits<u8, u32>(op, {30, 31});

          fmt::println("RT: {}", instruction_info.rt);
          fmt::println("RA: {}", instruction_info.ra);
          fmt::println("DS: {}", instruction_info.ds);
          fmt::println("XO: {}", instruction_info.xo);

          instruction_info.mnemnonic = fmt::format("std{} r{},{:#04x}(r{})", instruction_info.xo ? "u" : "", instruction_info.rs, (i16)(instruction_info.ds << 2), instruction_info.ra);
          break;
        }

        default: break;
      };

      break;
    }
    case PPC::InstructionFormat::A_FORM: {
      assert(0);
      exit(-1);
      break;
    }
    case PPC::InstructionFormat::M_FORM: {
      exit(-1);
      break;
    }
    case PPC::InstructionFormat::MD_FORM: {
      instruction_info.rs = PPC::get_bits<u8, u32>(op, {6, 10});
      instruction_info.ra = PPC::get_bits<u8, u32>(op, {11, 15});
      instruction_info.sh = PPC::get_bits<u16, u32>(op, {21, 30});
      instruction_info.mb = instruction_info.me = PPC::get_bits<u8, u32>(op, {21, 26});
      instruction_info.xo                       = PPC::get_bits<u8, u32>(op, {27, 27});
      instruction_info.rb = instruction_info.rb = instruction_info.rb = PPC::get_bits<u8, u32>(op, {16, 20});

      break;
    }
    case PPC::InstructionFormat::MDS_FORM: {
      assert(0);
      exit(-1);
      break;
    }
    case PPC::InstructionFormat::X_FORM: {
      instruction_info.rt = instruction_info.rs = instruction_info.to = instruction_info.frt = instruction_info.frs = PPC::get_bits<u8, u32>(op, {6, 10});
      instruction_info.bf                                                                                           = PPC::get_bits<u8, u32>(op, {6, 8});
      instruction_info.xo                                                                                           = PPC::get_bits<u16, u32>(op, {21, 30});
      instruction_info.rc                                                                                           = PPC::get_bits<u8, u32>(op, {31, 31});
      instruction_info.ra = instruction_info.fra = instruction_info.rb = PPC::get_bits<u8, u32>(op, {11, 15});
      instruction_info.rb = instruction_info.rb = instruction_info.rb = PPC::get_bits<u8, u32>(op, {16, 20});

      if (instruction_info.xo == 444) {
        instruction_info.mnemnonic = fmt::format("or{} r{},r{},r{}", instruction_info.rc ? "." : "", instruction_info.ra, instruction_info.rs, instruction_info.rb);
      } else {
        assert(0);
      }

      break;
    }
    case PPC::InstructionFormat::XL_FORM: {
      assert(0);
      exit(-1);
      break;
    }
    case PPC::InstructionFormat::XFX_FORM: {
      instruction_info.rt = PPC::get_bits<u8, u32>(op, {6, 10});

      instruction_info.d = PPC::get_bits<u16, u32>(op, {11, 20});

      // u8 spr_lo, spr_hi;

      // spr_lo = instruction_info.d & 0b11111;
      // spr_hi = (instruction_info.d>>5) & 0b11111;

      SPR spr = (SPR)((instruction_info.d >> 5) & 0b11111);

      u16 op_type = PPC::get_bits<u16, u32>(op, {21, 30});

      fmt::println("RT: {}", instruction_info.rt);
      fmt::println("D: {}", instruction_info.d);

      if (op_type == 467) {
        instruction_info.mnemnonic = fmt::format("mtspr {},r{}", spr_enum_to_str_map.at(spr), instruction_info.rs);
      } else if (op_type == 339) {
        // fmt::println("{:010b}", instruction_info.d);
        instruction_info.mnemnonic = fmt::format("mfspr r{},{}", instruction_info.rt, spr_enum_to_str_map.at(spr));
      } else {
        fmt::println("op type: {}", op_type);
        FATAL("AHHHHH");
      }

      break;
    }
    case PPC::InstructionFormat::XFL_FORM: {
      assert(0);
      exit(-1);
      break;
    }
    case PPC::InstructionFormat::XS_FORM: {
      assert(0);
      exit(-1);
      break;
    }
    case PPC::InstructionFormat::XO_FORM: {
      instruction_info.rt = PPC::get_bits<u8, u32>(op, {6, 10});
      instruction_info.ra = PPC::get_bits<u8, u32>(op, {11, 15});
      instruction_info.rb = PPC::get_bits<u8, u32>(op, {16, 20});
      instruction_info.oe = PPC::get_bits<u8, u32>(op, {21, 21});
      instruction_info.xo = PPC::get_bits<u8, u32>(op, {22, 30});
      instruction_info.rc = PPC::get_bits<u8, u32>(op, {31, 31});

      fmt::println("RT={}", instruction_info.rt);
      fmt::println("RA={}", instruction_info.ra);
      fmt::println("RB={}", instruction_info.rb);

      fmt::println("RC={}", instruction_info.rc);
      fmt::println("OE={}", instruction_info.oe);
      fmt::println("XO={}", instruction_info.xo);
      fmt::println("XO={}", instruction_info.xo);
      fmt::println("(RA)={}", ((i32)get_reg(instruction_info.ra) * 2));

      set_reg(instruction_info.rt, ((i32)get_reg(instruction_info.ra) + (i32)get_reg(instruction_info.rb)));
      // assert(0);
      // exit(-1);
      break;
    }
    case PPC::InstructionFormat::UNDEFINED: {
      assert(0);
      exit(-1);
      break;
    }
    default: {
      throw std::runtime_error(std::format("unimplemented decoder for undefined opcode: {:#04x}", opcd));
      break;
    }

    break;
  }
};