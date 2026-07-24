#include "common.hpp"
#include "ppc/parser.hpp"
#include "ppc/ppc.hpp"
#include "ppe.hpp"

void PPE::decode(u32 op, PPC::InstructionInfo& instruction_info) {
  u8 opcd = PPC::get_bits<u8, u32>(op, {0, 5});
  spdlog::info("decoding: {:#08x}", op);

  // spdlog::info("NIA: {:#016x}", nia);
  // spdlog::info("opcd: {:#010x} - {}", opcd, opcd);

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
          spdlog::info("3format");
          break;
        }

        default: FATAL();
      }
      break;
    }
    case 30: {
      u8 instruction_bits = PPC::get_bits<u8, u32>(op, {27, 30});
      spdlog::info("1instruction bits: {}", instruction_bits);

      if (instruction_bits <= 7) {
        format = PPC::InstructionFormat::MD_FORM;
      } else {
        format = PPC::InstructionFormat::MDS_FORM;
      }
      spdlog::info("format: {}", PPC::instruction_format_str_map.at(format));

      break;
    }
    case 31: {
      u16 instruction_bits = PPC::get_bits<u16, u32>(op, {21, 30});
      spdlog::info("2instruction bits: {}", instruction_bits);
      format = PPC::fx_ext_ops_map.at(instruction_bits).instruction_format;
      spdlog::info("2format: {}", PPC::instruction_format_str_map.at(format));
      break;
    }

    default: {
      spdlog::info("opcd: {}", opcd);
      format = PPC::opcode_info_map.at(opcd).instruction_format;
      spdlog::info("1format: {}", PPC::instruction_format_str_map.at(format));
      break;
    }
  }

  switch (format) {
    case PPC::InstructionFormat::I_FORM: {
      // spdlog::info("decoding I_FORM instruction");
      instruction_info.li = PPC::get_bits<u32, u32>(op, {6, 29});
      instruction_info.aa = PPC::get_bits<u8, u32>(op, {30, 30});
      instruction_info.lk = PPC::get_bits<u8, u32>(op, {31, 31});
      u8 branch_form      = PPC::get_bits<u8, u32>(op, {30, 31});

      spdlog::info("opcode: {:#032b}", op);
      spdlog::info("opcode: {:#016x}", instruction_info.li);

      spdlog::info("li: {:#016x}", instruction_info.li);
      spdlog::info("aa: {:#016x}", instruction_info.aa);
      spdlog::info("lk: {:#016x}", instruction_info.lk);

      if (!instruction_info.aa && !instruction_info.lk) instruction_info.mnemnonic = "b";
      if (instruction_info.aa && !instruction_info.lk) instruction_info.mnemnonic = "ba";

      if (!instruction_info.aa && instruction_info.lk) instruction_info.mnemnonic = "bl";
      if (instruction_info.aa && instruction_info.lk) instruction_info.mnemnonic = "bla";

      spdlog::info("branch form: {}", PPC::branch_form_type_table.at(branch_form));
      spdlog::info("{}", instruction_info.mnemnonic);
      break;
    }
    case PPC::InstructionFormat::B_FORM: {
      spdlog::info("decoding B_FORM instruction");

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

      spdlog::info("address of current instruction: {:#016x}", instruction_info.address);

      target_addr = EXTS(15, instruction_info.bd << 2);

      if (!instruction_info.aa) {
        target_addr += instruction_info.address;
      }

      spdlog::info("decoded target addr:{:#016x}", target_addr);

      if (instruction_info.lk) {
        FATAL();
      }

      instruction_info.mnemnonic   = fmt::format("{} {},{},{:#016x}", instruction_info.mnemnonic, instruction_info.bo, instruction_info.bi, target_addr);
      instruction_info.target_addr = target_addr;

      spdlog::info("{}", instruction_info.mnemnonic);
      break;
    }
    case PPC::InstructionFormat::SC_FORM: {
      instruction_info.lev       = PPC::get_bits<u8, u32>(op, {20, 26});
      instruction_info.mnemnonic = fmt::format("sc {:#04x}", instruction_info.lev);

      spdlog::info("sc {:#04x}", instruction_info.lev);
      break;
    }
    case PPC::InstructionFormat::D_FORM: {
      spdlog::info("decoding D_FORM instruction");

      instruction_info.bf = PPC::get_bits<u8, u32>(op, {6, 8});
      instruction_info.l  = PPC::get_bits<u8, u32>(op, {10, 10});
      instruction_info.rt = instruction_info.rs = instruction_info.to = instruction_info.frt = instruction_info.frs = PPC::get_bits<u8, u32>(op, {6, 10});
      instruction_info.ra                                                                                           = PPC::get_bits<u8, u32>(op, {11, 15});
      instruction_info.d = instruction_info.si = instruction_info.ui = PPC::get_bits<u16, u32>(op, {16, 31});

      switch (opcd) {
        case STBU: {
          instruction_info.mnemnonic = fmt::format("stbu r{},{:#04x}(r{})", instruction_info.rt, (i16)instruction_info.d, instruction_info.ra);
          break;
        }
        case XORI: {
          instruction_info.mnemnonic = fmt::format("xori r{},r{} {:#04x}", instruction_info.ra, instruction_info.rs, instruction_info.ui);
          break;
        }
        case XORIS: {
          instruction_info.mnemnonic = fmt::format("xoris r{},r{} {:#04x}", instruction_info.ra, instruction_info.rs, instruction_info.ui);
          break;
        }

        case SUBFIC: {
          instruction_info.mnemnonic = fmt::format("subfic r{},r{} {:#04x}", instruction_info.rt, instruction_info.ra, instruction_info.si);
          break;
        }
        case MULLI: {
          instruction_info.mnemnonic = fmt::format("mulli r{},r{} {:#04x}", instruction_info.rt, instruction_info.ra, instruction_info.si);
          break;
        }


        

        case ADDIC: {
          instruction_info.mnemnonic = fmt::format("addic r{},r{} {:#04x}", instruction_info.rt, instruction_info.ra, instruction_info.si);
          break;
        }

        case ADDIC+1: {
          instruction_info.mnemnonic = fmt::format("addic. r{},r{} {:#04x}", instruction_info.rt, instruction_info.ra, instruction_info.si);
          break;
        }

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

        case LBZX: {
          instruction_info.mnemnonic = fmt::format("lbzx r{},{:#04x}(r{})", instruction_info.rt, instruction_info.d, instruction_info.ra);
          break;
        }

        case STB: {
          instruction_info.mnemnonic = fmt::format("stb r{},{:#04x}(r{})", instruction_info.rs, instruction_info.d, instruction_info.ra);
          break;
        }

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

        case LFD: {
          instruction_info.mnemnonic = fmt::format("lfd r{},{:#04x}(r{})", instruction_info.frt, (i16)instruction_info.d, instruction_info.ra);
          break;
        }
        case LFDU: {
          instruction_info.mnemnonic = fmt::format("lfdu r{},{:#04x}(r{})", instruction_info.frt, (i16)instruction_info.d, instruction_info.ra);
          break;
        }

        case STFD: {
          instruction_info.mnemnonic = fmt::format("stfd r{},{:#04x}(r{})", instruction_info.frs, (i16)instruction_info.d, instruction_info.ra);
          break;
        }
        case STFDU: {
          instruction_info.mnemnonic = fmt::format("stfdu r{},{:#04x}(r{})", instruction_info.frs, (i16)instruction_info.d, instruction_info.ra);
          break;
        }

        default: {
          spdlog::error("no decoding logic for opcode: {}", opcd);
          FATAL();
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
          spdlog::info("m: {:#064b}", MASK(instruction_info.mb + 32, instruction_info.me + 32));
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
      instruction_info.rb = instruction_info.sh = PPC::get_bits<u8, u32>(op, {16, 20});

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

        case EXTSB: {
          instruction_info.mnemnonic = fmt::format("extsb{} r{},r{}", instruction_info.rc ? "." : "", instruction_info.ra, instruction_info.rs);
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
        case LDX: {
          instruction_info.mnemnonic = fmt::format("ldx r{},{:#04x}(r{})", instruction_info.rt, instruction_info.ra, instruction_info.rb);
          break;
        }

        case LBZX: {
          instruction_info.mnemnonic = fmt::format("lbzx r{},{:#04x}(r{})", instruction_info.rt, instruction_info.ra, instruction_info.rb);
          break;
        }

        case LHZX: {
          instruction_info.mnemnonic = fmt::format("lhzx r{},{:#04x}(r{})", instruction_info.rt, instruction_info.ra, instruction_info.rb);
          break;
        }

        case LWZX: {
          instruction_info.mnemnonic = fmt::format("lwzx r{},{:#04x}(r{})", instruction_info.rt, instruction_info.ra, instruction_info.rb);
          break;
        }

        case LHAX: {
          instruction_info.mnemnonic = fmt::format("lhax r{},{:#04x}(r{})", instruction_info.rt, instruction_info.ra, instruction_info.rb);
          break;
        }

        case LHAUX: {
          instruction_info.mnemnonic = fmt::format("lhaux r{},{:#04x}(r{})", instruction_info.rt, instruction_info.ra, instruction_info.rb);
          break;
        }

        case LWAX: {
          instruction_info.mnemnonic = fmt::format("lwax r{},{:#04x}(r{})", instruction_info.rt, instruction_info.ra, instruction_info.rb);
          break;
        }

        case LBZUX: {
          instruction_info.mnemnonic = fmt::format("lbzux r{},{:#04x}(r{})", instruction_info.rt, instruction_info.ra, instruction_info.rb);
          break;
        }

        case LHZUX: {
          instruction_info.mnemnonic = fmt::format("lhzux r{},{:#04x}(r{})", instruction_info.rt, instruction_info.ra, instruction_info.rb);
          break;
        }
        case LWZUX: {
          instruction_info.mnemnonic = fmt::format("lwzux r{},{:#04x}(r{})", instruction_info.rt, instruction_info.ra, instruction_info.rb);
          break;
        }
        case LWAUX: {
          instruction_info.mnemnonic = fmt::format("lwaux r{},{:#04x}(r{})", instruction_info.rt, instruction_info.ra, instruction_info.rb);
          break;
        }

        case LDUX: {
          instruction_info.mnemnonic = fmt::format("ldux r{},{:#04x}(r{})", instruction_info.rt, instruction_info.ra, instruction_info.rb);
          break;
        }

        case STBX: {
          instruction_info.mnemnonic = fmt::format("stbx r{},{:#04x}(r{})", instruction_info.rt, instruction_info.ra, instruction_info.rb);
          break;
        }
        case STHX: {
          instruction_info.mnemnonic = fmt::format("sthx r{},{:#04x}(r{})", instruction_info.rt, instruction_info.ra, instruction_info.rb);
          break;
        }

        case STWX: {
          instruction_info.mnemnonic = fmt::format("stwx r{},{:#04x}(r{})", instruction_info.rt, instruction_info.ra, instruction_info.rb);
          break;
        }
        case STDX: {
          instruction_info.mnemnonic = fmt::format("stdx r{},{:#04x}(r{})", instruction_info.rt, instruction_info.ra, instruction_info.rb);
          break;
        }

        case STBUX: {
          instruction_info.mnemnonic = fmt::format("stbux r{},{:#04x}(r{})", instruction_info.rt, instruction_info.ra, instruction_info.rb);
          break;
        }
        case STHUX: {
          instruction_info.mnemnonic = fmt::format("sthux r{},{:#04x}(r{})", instruction_info.rt, instruction_info.ra, instruction_info.rb);
          break;
        }

        case STWUX: {
          instruction_info.mnemnonic = fmt::format("stwux r{},{:#04x}(r{})", instruction_info.rt, instruction_info.ra, instruction_info.rb);
          break;
        }
        case STDUX: {
          instruction_info.mnemnonic = fmt::format("stdux r{},{:#04x}(r{})", instruction_info.rt, instruction_info.ra, instruction_info.rb);
          break;
        }

        case SRAWI: {
          instruction_info.mnemnonic = fmt::format("srawi r{},r{}, {:#04x}", instruction_info.ra, instruction_info.rs, instruction_info.sh);
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

      spdlog::info("XL XO: {}", instruction_info.xo);

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
          if (PPC::get_bits<u8, u32>(instruction_info.full_opcode, {11, 11}) == 0) {
            instruction_info.mnemnonic = fmt::format("mtcrf r{}", instruction_info.rs);
          } else {
            instruction_info.mnemnonic = fmt::format("mtocrf {:#04x},r{}", instruction_info.fxm, instruction_info.rs);
          }
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
        case ADDC: {
          instruction_info.mnemnonic = fmt::format("addc r{},r{} {:#04x}", instruction_info.rt, instruction_info.ra, instruction_info.si);
          break;
        }

        case DIVD: {
          instruction_info.mnemnonic = fmt::format("divd r{},r{} {:#04x}", instruction_info.rt, instruction_info.ra, instruction_info.si);
          break;
        }

        case MULLW: {
          instruction_info.mnemnonic = fmt::format("mullw r{},r{},r{}", instruction_info.rt, instruction_info.ra, instruction_info.rb);
          break;
        }

        case MULLD: {
          instruction_info.mnemnonic = fmt::format("mulld r{},r{},r{}", instruction_info.rt, instruction_info.ra, instruction_info.rb);
          break;
        }

        case MULHW: {
          instruction_info.mnemnonic = fmt::format("mulhw r{},r{},r{}", instruction_info.rt, instruction_info.ra, instruction_info.rb);
          break;
        }

        case MULHD: {
          instruction_info.mnemnonic = fmt::format("mulhd r{},r{},r{}", instruction_info.rt, instruction_info.ra, instruction_info.rb);
          break;
        }

        
        case DIVW: {
          instruction_info.mnemnonic = fmt::format("divw r{},r{},r{}", instruction_info.rt, instruction_info.ra, instruction_info.rb);
          break;
        }

        case DIVWU: {
          instruction_info.mnemnonic = fmt::format("divwu r{},r{},r{}", instruction_info.rt, instruction_info.ra, instruction_info.rb);
          break;
        }

        case DIVDU: {
          instruction_info.mnemnonic = fmt::format("divdu r{},r{},r{}", instruction_info.rt, instruction_info.ra, instruction_info.rb);
          break;
        }

        case 104: {
          instruction_info.mnemnonic = fmt::format("neg{}{} r{}, r{}", instruction_info.oe ? "o" : "", instruction_info.rc ? "." : "", instruction_info.rt, instruction_info.ra);
          break;
        }

        case 138:
        case 266: {
          instruction_info.mnemnonic = fmt::format("add{}{} r{}, r{}", instruction_info.oe ? "o" : "", instruction_info.rc ? "." : "", instruction_info.ra, instruction_info.rb);
          break;
        }

        case 40: {
          instruction_info.mnemnonic =
              fmt::format("subf{}{} r{}, r{}, r{}", instruction_info.oe ? "o" : "", instruction_info.rc ? "." : "", instruction_info.rt, instruction_info.ra, instruction_info.rb);
          break;
        }

        case SUBFC: {
          instruction_info.mnemnonic =
              fmt::format("subfc{}{} r{}, r{}, r{}", instruction_info.oe ? "o" : "", instruction_info.rc ? "." : "", instruction_info.rt, instruction_info.ra, instruction_info.rb);
          break;
        }
        case SUBFE: {
          instruction_info.mnemnonic =
              fmt::format("subfe{}{} r{}, r{}, r{}", instruction_info.oe ? "o" : "", instruction_info.rc ? "." : "", instruction_info.rt, instruction_info.ra, instruction_info.rb);
          break;
        }
        case ADDME: {
          instruction_info.mnemnonic =
              fmt::format("addme{}{} r{}, r{}, r{}", instruction_info.oe ? "o" : "", instruction_info.rc ? "." : "", instruction_info.rt, instruction_info.ra, instruction_info.rb);
          break;
        }
        
         case SUBFME: {
          instruction_info.mnemnonic =
              fmt::format("subfme{}{} r{}, r{}, r{}", instruction_info.oe ? "o" : "", instruction_info.rc ? "." : "", instruction_info.rt, instruction_info.ra, instruction_info.rb);
          break;
        }
        
         case ADDZ: {
          instruction_info.mnemnonic =
              fmt::format("addz{}{} r{}, r{}, r{}", instruction_info.oe ? "o" : "", instruction_info.rc ? "." : "", instruction_info.rt, instruction_info.ra, instruction_info.rb);
          break;
        }
         case SUBFZE: {
          instruction_info.mnemnonic =
              fmt::format("subfze{}{} r{}, r{}, r{}", instruction_info.oe ? "o" : "", instruction_info.rc ? "." : "", instruction_info.rt, instruction_info.ra, instruction_info.rb);
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