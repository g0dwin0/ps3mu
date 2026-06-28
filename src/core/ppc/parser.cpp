
#include "ppc/parser.hpp"

#include <arpa/inet.h>
#include <netinet/in.h>

#include <format>
#include <stdexcept>

#include "common/defs.hpp"
#include "ppc/ppc.hpp"
#include "ppe.hpp"
#include "spdlog/spdlog.h"
namespace PPC {

  template<typename Width> 
  Width EXTS(Width x) {

    return x;
  }

  template <typename RetValWidth, typename InputWidth>
  RetValWidth get_bits(u32 n, std::pair<u8, u8> be_range) {
    const auto& [start, end] = be_range;

    assert(start < end);

    n = (n >> ((sizeof(InputWidth) * 8) - end - 1));

    u16 mask = (1 << (end + 1 - start)) - 1;

    if (start != 0) {
      n &= mask;
    }

    // fmt::println("retval: {}\n", (RetValWidth)n);

    return n;
  }

  constexpr u8 STORE_DOUBLEWORD = 0x3E;

  void decode(u32 op, InstructionInfo& instruction_info) {
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

    u8 opcd = get_bits<u8, u32>(op, {0, 5});

    instruction_info.opcode = opcd;
    // spdlog::info("x: {:#010x}", opcd);

    InstructionFormat format = InstructionFormat::UNDEFINED;

    format = opcode_info_map.at(opcd).instruction_format;

    fmt::println("format: {}", instruction_format_str_map.at(format));

    switch (format) {
      case InstructionFormat::I_FORM: {
        fmt::println("decoding I_FORM instruction");
        instruction_info.li = get_bits<u8, u32>(op, {6, 29});
        instruction_info.aa = get_bits<u8, u32>(op, {30, 30});
        instruction_info.lk = get_bits<u8, u32>(op, {31, 31});

        break;
      }
      case InstructionFormat::B_FORM: {
        fmt::println("decoding B_FORM instruction");

        instruction_info.bo = get_bits<u8, u32>(op, {6, 10});
        instruction_info.bi = get_bits<u8, u32>(op, {11, 15});
        instruction_info.bd = get_bits<u8, u32>(op, {16, 29});
        instruction_info.aa = get_bits<u8, u32>(op, {30, 30});
        instruction_info.lk = get_bits<u8, u32>(op, {31, 31});

        break;
      }
      case InstructionFormat::SC_FORM: break;
      case InstructionFormat::D_FORM: {
        fmt::println("decoding D_FORM instruction");
        instruction_info.rt = instruction_info.rs = instruction_info.to = instruction_info.frt = instruction_info.frs = get_bits<u8, u32>(op, {6, 10});
        instruction_info.ra = instruction_info.ra = get_bits<u8, u32>(op, {11, 15});
        instruction_info.d = instruction_info.si = instruction_info.ui = get_bits<u8, u32>(op, {16, 31});

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
            instruction_info.mnemnonic  = fmt::format("ori r{},r{},{:#04x}", instruction_info.ra, instruction_info.rs, instruction_info.ui);
            break;
          }

          case 25: {
            instruction_info.mnemnonic  = fmt::format("oris r{},r{},{:#04x}", instruction_info.ra, instruction_info.rs, instruction_info.ui);
            break;
          }

          case 32: {
            instruction_info.mnemnonic  = fmt::format("lwz r{},{:#04x}(r{})", instruction_info.rt, instruction_info.d, instruction_info.ra);
            break;
          }

          case 33: {
            instruction_info.mnemnonic  = fmt::format("lwzi r{},{:#04x}(r{})", instruction_info.rt, instruction_info.d, instruction_info.ra);
            break;
          }

          default: {
            spdlog::error("no decoding logic for opcode: {}", opcd);
            assert(0);
          }
        }

        break;
      }
      case InstructionFormat::DS_FORM: {
        switch (opcd) {
          case STORE_DOUBLEWORD: {
            instruction_info.rt = instruction_info.rs = get_bits<u8, u32>(op, {6, 10});

            // 0b11111000001000011111111110000001
            instruction_info.ra = get_bits<u8, u32>(op, {11, 15});
            instruction_info.ds = get_bits<i16, u32>(op, {16, 29});
            instruction_info.xo = get_bits<u8, u32>(op, {30, 31});

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
      case InstructionFormat::A_FORM: break;
      case InstructionFormat::M_FORM: break;
      case InstructionFormat::MD_FORM: break;
      case InstructionFormat::MDS_FORM: break;
      case InstructionFormat::X_FORM: break;
      case InstructionFormat::XL_FORM: break;
      case InstructionFormat::XFX_FORM: break;
      case InstructionFormat::XFL_FORM: break;
      case InstructionFormat::XS_FORM: break;
      case InstructionFormat::XO_FORM: break;
      case InstructionFormat::UNDEFINED:
      default: {
        throw std::runtime_error(std::format("unimplemented decoder for undefined opcode: {:#04x}", opcd));
        break;
      }

      break;
    }
  };

  // void execute(u32 op, InstructionInfo& instruction_info, PPE& ppe) {

  // }
}  // namespace PPC