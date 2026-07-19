#include "common.hpp"
#include "ppc/parser.hpp"
#include "ppc/ppc.hpp"
#include "ppe.hpp"

void PPE::decode(u32 op, PPC::InstructionInfo& instruction_info) {
  u8 opcd = PPC::get_bits<u8, u32>(op, {0, 5});
  fmt::println("decoding: {:#08x}", op);

  // fmt::println("NIA: {:#016x}", nia);
  // fmt::println("opcd: {:#010x} - {}", opcd, opcd);

  instruction_info.opcode      = opcd;
  instruction_info.full_opcode = op;

  PPC::InstructionFormat format = PPC::InstructionFormat::UNDEFINED;

  switch (opcd) {
    case 19: {
      u16 instruction_bits = PPC::get_bits<u16, u32>(op, {21, 30});

      switch (instruction_bits) {
        case 16:
        case 18:
        case 33:
        case 82:
        case 129:
        case 150:
        case 193:
        case 225:
        case 257:
        case 289:
        case 528: {
          format = PPC::InstructionFormat::XL_FORM;
          fmt::println("3format");
          break;
        }

        default: FATAL();
      }
      break;
    }
    case 30: {
      u8 instruction_bits = PPC::get_bits<u8, u32>(op, {27, 30});
      fmt::println("1instruction bits: {}", instruction_bits);

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
      fmt::println("2instruction bits: {}", instruction_bits);
      format = PPC::fx_ext_ops_map.at(instruction_bits).instruction_format;
      fmt::println("2format: {}", PPC::instruction_format_str_map.at(format));
      break;
    }

    default: {
      fmt::println("opcd: {}", opcd);
      format = PPC::opcode_info_map.at(opcd).instruction_format;
      fmt::println("1format: {}", PPC::instruction_format_str_map.at(format));
      break;
    }
  }

  switch (format) {
    case PPC::InstructionFormat::I_FORM: {
      // fmt::println("decoding I_FORM instruction");
      instruction_info.li = PPC::get_bits<u32, u32>(op, {6, 29});
      instruction_info.aa = PPC::get_bits<u8, u32>(op, {30, 30});
      instruction_info.lk = PPC::get_bits<u8, u32>(op, {31, 31});
      u8 branch_form      = PPC::get_bits<u8, u32>(op, {30, 31});

      fmt::println("opcode: {:#032b}", op);
      fmt::println("opcode: {:#016x}", instruction_info.li);

      fmt::println("li: {:#016x}", instruction_info.li);
      fmt::println("aa: {:#016x}", instruction_info.aa);
      fmt::println("lk: {:#016x}", instruction_info.lk);

      if (!instruction_info.aa && !instruction_info.lk) instruction_info.mnemnonic = "b";
      if (instruction_info.aa && !instruction_info.lk) instruction_info.mnemnonic = "ba";

      if (!instruction_info.aa && instruction_info.lk) instruction_info.mnemnonic = "bl";
      if (instruction_info.aa && instruction_info.lk) instruction_info.mnemnonic = "bla";

      fmt::println("branch form: {}", PPC::branch_form_type_table.at(branch_form));
      fmt::println("{}", instruction_info.mnemnonic);
      break;
    }
    case PPC::InstructionFormat::B_FORM: {
      fmt::println("decoding B_FORM instruction");

      instruction_info.bo = PPC::get_bits<u8, u32>(op, {6, 10});
      instruction_info.bi = PPC::get_bits<u8, u32>(op, {11, 15});
      instruction_info.bd = PPC::get_bits<u16, u32>(op, {16, 29});
      instruction_info.aa = PPC::get_bits<u8, u32>(op, {30, 30});
      instruction_info.lk = PPC::get_bits<u8, u32>(op, {31, 31});

      if (!instruction_info.aa && !instruction_info.lk) instruction_info.mnemnonic = "bc";
      if (instruction_info.aa && !instruction_info.lk) instruction_info.mnemnonic = "bca";

      if (!instruction_info.aa && instruction_info.lk) instruction_info.mnemnonic = "bcl";
      if (instruction_info.aa && instruction_info.lk) instruction_info.mnemnonic = "bcla";

      u64 target_addr = 0;

      fmt::println("address of current instruction: {:#016x}", instruction_info.address);

      target_addr = EXTS(15, instruction_info.bd << 2);

      if (!instruction_info.aa) {
        target_addr += instruction_info.address;
      }

      fmt::println("decoded target addr:{:#016x}", target_addr);

      if (instruction_info.lk) {
        FATAL();
      }

      instruction_info.mnemnonic   = fmt::format("{} {},{},{:#016x}", instruction_info.mnemnonic, instruction_info.bo, instruction_info.bi, target_addr);
      instruction_info.target_addr = target_addr;

      fmt::println("{}", instruction_info.mnemnonic);
      break;
    }
    case PPC::InstructionFormat::SC_FORM: {
      instruction_info.lev       = PPC::get_bits<u8, u32>(op, {20, 26});
      instruction_info.mnemnonic = fmt::format("sc {:#04x}", instruction_info.lev);

      fmt::println("sc {:#04x}", instruction_info.lev);
      break;
    }
    case PPC::InstructionFormat::D_FORM: {
      fmt::println("decoding D_FORM instruction");

      instruction_info.bf = PPC::get_bits<u8, u32>(op, {6, 8});
      instruction_info.l  = PPC::get_bits<u8, u32>(op, {10, 10});
      instruction_info.rt = instruction_info.rs = instruction_info.to = instruction_info.frt = instruction_info.frs = PPC::get_bits<u8, u32>(op, {6, 10});
      instruction_info.ra                                                                                           = PPC::get_bits<u8, u32>(op, {11, 15});
      instruction_info.d = instruction_info.si = instruction_info.ui = PPC::get_bits<u16, u32>(op, {16, 31});

      switch (opcd) {
        case ADDI: {
          if (instruction_info.ra == 0) {
            instruction_info.mnemnonic = fmt::format("li r{}, {:#04x}", instruction_info.rt, instruction_info.si);
          } else {
            instruction_info.mnemnonic = fmt::format("addi r{},r{},{:#06x}", instruction_info.rt, instruction_info.ra, instruction_info.si);
          }
          break;
        }
        case CMPLI: {
          instruction_info.mnemnonic = fmt::format("cmpli cr{},r{},{}", instruction_info.bf, instruction_info.ra, instruction_info.si);
          break;
        }
        case ADDIS: {
          if (instruction_info.ra == 0) {
            instruction_info.mnemnonic = fmt::format("lis r{}, {:#04x}", instruction_info.rt, instruction_info.si);
          } else {
            instruction_info.mnemnonic = fmt::format("addis r{},r{},{:#06x}", instruction_info.rt, instruction_info.ra, instruction_info.si);
          }
          break;
        }

        case ORI: {
          instruction_info.mnemnonic = fmt::format("ori r{},r{},{:#04x}", instruction_info.ra, instruction_info.rs, instruction_info.ui);
          break;
        }

        case ORIS: {
          instruction_info.mnemnonic = fmt::format("oris r{},r{},{:#04x}", instruction_info.ra, instruction_info.rs, instruction_info.ui);
          break;
        }

        case LHZ: {
          instruction_info.mnemnonic = fmt::format("lhz r{},{:#04x}(r{})", instruction_info.rt, (i16)instruction_info.d, instruction_info.ra);
          break;
        }

        case LWZ: {
          instruction_info.mnemnonic = fmt::format("lwz r{},{:#04x}(r{})", instruction_info.rt, (i16)instruction_info.d, instruction_info.ra);
          break;
        }

        case LWZU: {
          instruction_info.mnemnonic = fmt::format("lwzu r{},{:#04x}(r{})", instruction_info.rt, instruction_info.d, instruction_info.ra);
          break;
        }

        case STW: {
          instruction_info.mnemnonic = fmt::format("stw r{},{:#04x}(r{})", instruction_info.rs, instruction_info.d, instruction_info.ra);
          break;
        }

        case STH: {
          instruction_info.mnemnonic = fmt::format("sth r{},{:#04x}(r{})", instruction_info.rs, instruction_info.d, instruction_info.ra);
          break;
        }

        case STHU: {
          instruction_info.mnemnonic = fmt::format("sthu r{},{:#04x}(r{})", instruction_info.rs, instruction_info.d, instruction_info.ra);
          break;
        }

        case STWU: {
          instruction_info.mnemnonic = fmt::format("stwu r{},{:#04x}(r{})", instruction_info.rs, instruction_info.d, instruction_info.ra);
          break;
        }

        case CMPI: {
          instruction_info.mnemnonic = fmt::format("cmpi cr{},r{},{}", instruction_info.bf, instruction_info.ra, instruction_info.si);
          break;
        }
        case LBZ: {
          instruction_info.mnemnonic = fmt::format("lbz r{},{:#04x}(r{})", instruction_info.rt, instruction_info.d, instruction_info.ra);
          break;
        }

          // case STB: {
          //   instruction_info.mnemnonic = fmt::format("stb r{},{:#04x}(r{})", instruction_info.rs, instruction_info.d, instruction_info.ra);
          //   break;
          // }

        case LHA: {
          instruction_info.mnemnonic = fmt::format("lha r{},{:#04x}(r{})", instruction_info.rt, (i16)instruction_info.d, instruction_info.ra);
          break;
        }

        case LBZU: {
          instruction_info.mnemnonic = fmt::format("lbzu r{},{:#04x}(r{})", instruction_info.rt, (i16)instruction_info.d, instruction_info.ra);
          break;
        }

        case LHZU: {
          instruction_info.mnemnonic = fmt::format("lhzu r{},{:#04x}(r{})", instruction_info.rt, (i16)instruction_info.d, instruction_info.ra);
          break;
        }

        default: {
          FATAL();
          spdlog::error("no decoding logic for opcode: {}", opcd);
          assert(0);
        }
      }

      break;
    }
    case PPC::InstructionFormat::DS_FORM: {
      instruction_info.rt = instruction_info.rs = PPC::get_bits<u8, u32>(op, {6, 10});
      instruction_info.ra                       = PPC::get_bits<u8, u32>(op, {11, 15});
      instruction_info.ds                       = PPC::get_bits<u16, u32>(op, {16, 29});
      instruction_info.xo                       = PPC::get_bits<u8, u32>(op, {30, 31});

      switch (opcd) {
        case STORE_DOUBLEWORD: {
          instruction_info.mnemnonic = fmt::format("std{} r{},{:#04x}(r{})", instruction_info.xo ? "u" : "", instruction_info.rs, (i16)(instruction_info.ds << 2), instruction_info.ra);
          break;
        }

        case 58: {
          instruction_info.mnemnonic = fmt::format("ld{} r{},{:#04x}(r{})", instruction_info.xo ? "u" : "", instruction_info.rs, instruction_info.ds << 2, instruction_info.ra);
          break;
        }

        default: {
          FATAL(fmt::format("no decode path for DS_FORM with opcd: {}", opcd));
          break;
        }
      };

      break;
    }
    case PPC::InstructionFormat::A_FORM: {
      FATAL();
      break;
    }
    case PPC::InstructionFormat::M_FORM: {
      instruction_info.rs = PPC::get_bits<u8, u32>(op, {6, 10});
      instruction_info.ra = PPC::get_bits<u8, u32>(op, {11, 15});
      instruction_info.sh = instruction_info.rb = PPC::get_bits<u8, u32>(op, {16, 20});
      instruction_info.mb                       = PPC::get_bits<u8, u32>(op, {21, 25});
      instruction_info.me                       = PPC::get_bits<u8, u32>(op, {26, 30});
      instruction_info.rc                       = PPC::get_bits<u8, u32>(op, {31, 31});

      switch (opcd) {
        case 21: {
          fmt::println("m: {:#064b}", MASK(instruction_info.mb + 32, instruction_info.me + 32));
          instruction_info.mnemnonic = fmt::format("rlwinm{} ", instruction_info.rc ? "." : "");
          break;
        }

        default: FATAL();
      }
      break;
    }
    case PPC::InstructionFormat::MD_FORM: {
      instruction_info.rs = PPC::get_bits<u8, u32>(op, {6, 10});
      instruction_info.ra = PPC::get_bits<u8, u32>(op, {11, 15});
      instruction_info.sh = (PPC::get_bits<u8, u32>(op, {30, 30}) << 5) + PPC::get_bits<u8, u32>(op, {16, 20});
      instruction_info.mb = (PPC::get_bits<u8, u32>(op, {26, 26}) << 5) + PPC::get_bits<u8, u32>(op, {21, 25});
      instruction_info.me = (PPC::get_bits<u8, u32>(op, {26, 26}) << 5) + PPC::get_bits<u8, u32>(op, {21, 25});

      instruction_info.xo = PPC::get_bits<u8, u32>(op, {27, 29});
      instruction_info.rc = PPC::get_bits<u8, u32>(op, {31, 31});

      if (opcd == 30) {
        u8 instruction_bits = PPC::get_bits<u8, u32>(op, {27, 29});

        switch (instruction_bits) {
          case 0: {
            instruction_info.mnemnonic =
                fmt::format("rldicl{} r{},r{},{:#04x},{:#04x}", instruction_info.rc ? "." : "", instruction_info.ra, instruction_info.rs, instruction_info.sh, instruction_info.mb);
            break;
          }
          case 1: {
            instruction_info.mnemnonic =
                fmt::format("rldicr{} r{},r{},{:#04x},{:#04x}", instruction_info.rc ? "." : "", instruction_info.ra, instruction_info.rs, instruction_info.sh, instruction_info.me);
            break;
          }
          case 2: {
            instruction_info.mnemnonic =
                fmt::format("rldic{} r{},r{},{:#04x},{:#04x}", instruction_info.rc ? "." : "", instruction_info.ra, instruction_info.rs, instruction_info.sh, instruction_info.me);
            break;
          }
          case 3: {
            instruction_info.mnemnonic =
                fmt::format("rldimi{} r{},r{},{:#04x},{:#04x}", instruction_info.rc ? "." : "", instruction_info.ra, instruction_info.rs, instruction_info.sh, instruction_info.me);
            break;
          }

          default: FATAL(std::format("{}", instruction_bits));
        }
      } else {
        FATAL();
      }

      break;
    }
    case PPC::InstructionFormat::MDS_FORM: {
      instruction_info.rs = PPC::get_bits<u8, u32>(op, {6, 10});
      instruction_info.ra = PPC::get_bits<u8, u32>(op, {11, 15});
      instruction_info.rb = PPC::get_bits<u8, u32>(op, {16, 20});

      instruction_info.mb = (PPC::get_bits<u8, u32>(op, {26, 26}) << 5) + PPC::get_bits<u8, u32>(op, {21, 25});
      instruction_info.me = (PPC::get_bits<u8, u32>(op, {26, 26}) << 5) + PPC::get_bits<u8, u32>(op, {21, 25});

      instruction_info.xo = PPC::get_bits<u8, u32>(op, {27, 30});
      instruction_info.rc = PPC::get_bits<u8, u32>(op, {31, 31});
      break;
    }
    case PPC::InstructionFormat::X_FORM: {
      instruction_info.l  = PPC::get_bits<u8, u32>(op, {10, 10});
      instruction_info.rt = instruction_info.rs = instruction_info.to = instruction_info.frt = instruction_info.frs = PPC::get_bits<u8, u32>(op, {6, 10});
      instruction_info.bf                                                                                           = PPC::get_bits<u8, u32>(op, {6, 8});
      instruction_info.xo                                                                                           = PPC::get_bits<u16, u32>(op, {21, 30});
      instruction_info.rc                                                                                           = PPC::get_bits<u8, u32>(op, {31, 31});
      instruction_info.ra = instruction_info.fra = PPC::get_bits<u8, u32>(op, {11, 15});
      instruction_info.rb                        = PPC::get_bits<u8, u32>(op, {16, 20});

      if (instruction_info.rc) FATAL();

      switch (instruction_info.xo) {
        case 0: {
          instruction_info.mnemnonic = fmt::format("cmp cr{},r{},r{}", instruction_info.bf, instruction_info.l, instruction_info.ra, instruction_info.rb);
          break;
        };
        case 32: {
          instruction_info.mnemnonic = fmt::format("cmpl cr{},r{},r{}", instruction_info.bf, instruction_info.ra, instruction_info.rb);
          break;
        }

        case 278: {
          instruction_info.mnemnonic = fmt::format("dcbt r{},r{}", instruction_info.ra, instruction_info.rs, instruction_info.rb);
          break;
        }

        case 316: {
          instruction_info.mnemnonic = fmt::format("xor{} r{},r{},r{}", instruction_info.rc ? "." : "", instruction_info.ra, instruction_info.rs, instruction_info.rb);
          break;
        }
        case 444: {
          instruction_info.mnemnonic = fmt::format("or{} r{},r{},r{}", instruction_info.rc ? "." : "", instruction_info.ra, instruction_info.rs, instruction_info.rb);
          break;
        }

        case 28: {
          instruction_info.mnemnonic = fmt::format("and{} r{},r{},r{}", instruction_info.rc ? "." : "", instruction_info.ra, instruction_info.rs, instruction_info.rb);
          break;
        }

        case NOR: {
          instruction_info.mnemnonic = fmt::format("nor{} r{},r{},r{}", instruction_info.rc ? "." : "", instruction_info.ra, instruction_info.rs, instruction_info.rb);
          break;
        }

        case EXTSW: {
          instruction_info.mnemnonic = fmt::format("extsw{} r{},r{}", instruction_info.rc ? "." : "", instruction_info.ra, instruction_info.rs);
          break;
        }
        case EXTSH: {
          instruction_info.mnemnonic = fmt::format("extsh{} r{},r{}", instruction_info.rc ? "." : "", instruction_info.ra, instruction_info.rs);
          break;
        }

        case CNTLZD: {
          instruction_info.mnemnonic = fmt::format("cntlzd{} r{},r{}", instruction_info.rc ? "." : "", instruction_info.ra, instruction_info.rs);
          break;
        }
        case CNTLZW: {
          instruction_info.mnemnonic = fmt::format("cntlzw{} r{},r{}", instruction_info.rc ? "." : "", instruction_info.ra, instruction_info.rs);
          break;
        }

        case SRD: {
          instruction_info.mnemnonic = fmt::format("srd{} r{},r{},r{}", instruction_info.rc ? "." : "", instruction_info.ra, instruction_info.rs, instruction_info.rb);
          break;
        }
        case SRW: {
          instruction_info.mnemnonic = fmt::format("srw{} r{},r{},r{}", instruction_info.rc ? "." : "", instruction_info.ra, instruction_info.rs, instruction_info.rb);
          break;
        }

        case SLD: {
          instruction_info.mnemnonic = fmt::format("sld{} r{},r{},r{}", instruction_info.rc ? "." : "", instruction_info.ra, instruction_info.rs, instruction_info.rb);
          break;
        }

        case SLW: {
          instruction_info.mnemnonic = fmt::format("slw{} r{},r{},r{}", instruction_info.rc ? "." : "", instruction_info.ra, instruction_info.rs, instruction_info.rb);
          break;
        }
        default: FATAL(std::format("{} - unhandled", instruction_info.xo));
      }
      break;
    }
    case PPC::InstructionFormat::XL_FORM: {
      instruction_info.bo = PPC::get_bits<u8, u32>(op, {6, 10});
      instruction_info.bi = PPC::get_bits<u16, u32>(op, {11, 15});
      instruction_info.bh = PPC::get_bits<u16, u32>(op, {19, 20});
      instruction_info.lk = PPC::get_bits<u16, u32>(op, {31, 31});
      instruction_info.xo = PPC::get_bits<u16, u32>(op, {21, 30});
      instruction_info.bf = PPC::get_bits<u8, u32>(op, {6, 8});

      fmt::println("XL XO: {}", instruction_info.xo);

      switch (instruction_info.xo) {
        case 16: {
          instruction_info.mnemnonic = "BCLR";
          break;
        }
        case 528: {
          instruction_info.mnemnonic = "BCCTR";
          break;
        }
        default: FATAL();
      }
      instruction_info.mnemnonic = fmt::format("xl_form branch conditional opcode idk do the mnemnonic stuff later it's 12:36am");
      break;
    }
    case PPC::InstructionFormat::XFX_FORM: {
      instruction_info.rt = instruction_info.rs = PPC::get_bits<u8, u32>(op, {6, 10});
      instruction_info.d                        = PPC::get_bits<u16, u32>(op, {11, 20});
      instruction_info.xo                       = PPC::get_bits<u16, u32>(op, {21, 30});
      instruction_info.fxm                      = PPC::get_bits<u16, u32>(op, {12, 19});

      SPR spr = (SPR)((instruction_info.d >> 5) & 0b11111);

      u16 op_type = PPC::get_bits<u16, u32>(op, {21, 30});

      switch (op_type) {
        case 467: {
          instruction_info.mnemnonic = fmt::format("mtspr {},r{}", spr_enum_to_str_map.at(spr), instruction_info.rs);
          break;
        }

        case 339: {
          instruction_info.mnemnonic = fmt::format("mfspr r{},{}", instruction_info.rt, spr_enum_to_str_map.at(spr));
          break;
        }

        case 19: {
          instruction_info.mnemnonic = fmt::format("mfcr r{}", instruction_info.rt);
          break;
        }

        case 144: {
          instruction_info.mnemnonic = fmt::format("mtocrf {:#04x},r{}", instruction_info.fxm, instruction_info.rs);
          break;
        }

        default: FATAL();
      }

      break;
    }
    case PPC::InstructionFormat::XFL_FORM: {
      FATAL();
      break;
    }
    case PPC::InstructionFormat::XS_FORM: {
      FATAL();
      break;
    }
    case PPC::InstructionFormat::XO_FORM: {
      instruction_info.rt = PPC::get_bits<u8, u32>(op, {6, 10});
      instruction_info.ra = PPC::get_bits<u8, u32>(op, {11, 15});
      instruction_info.rb = PPC::get_bits<u8, u32>(op, {16, 20});
      instruction_info.oe = PPC::get_bits<u8, u32>(op, {21, 21});
      instruction_info.xo = PPC::get_bits<u16, u32>(op, {22, 30});
      instruction_info.rc = PPC::get_bits<u8, u32>(op, {31, 31});
      if (instruction_info.rc || instruction_info.oe) FATAL();

      switch (instruction_info.xo) {
        case 104: {
          instruction_info.mnemnonic = fmt::format("neg{}{} r{}, r{}", instruction_info.oe ? "o" : "", instruction_info.rc ? "." : "", instruction_info.rt, instruction_info.ra);
          break;
        }
        case 266: {
          instruction_info.mnemnonic = fmt::format("add{}{} r{}, r{}", instruction_info.oe ? "o" : "", instruction_info.rc ? "." : "", instruction_info.ra, instruction_info.rb);
          break;
        }

        case 40: {
          instruction_info.mnemnonic =
              fmt::format("subf{}{} r{}, r{}, r{}", instruction_info.oe ? "o" : "", instruction_info.rc ? "." : "", instruction_info.rt, instruction_info.ra, instruction_info.rb);
          break;
        }

        default: FATAL(std::format("{}", instruction_info.xo));
      }
      break;
    }
    case PPC::InstructionFormat::UNDEFINED: {
      FATAL();
      break;
    }
    default: {
      throw std::runtime_error(std::format("unimplemented decoder for undefined opcode: {:#04x}", opcd));
      break;
    }

    break;
  }
};