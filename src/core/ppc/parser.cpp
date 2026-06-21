
#include "ppc/parser.hpp"

#include <netinet/in.h>

#include <format>
#include <stdexcept>

#include "common/defs.hpp"
// #include "endian.h"
#include <arpa/inet.h>
// #include <machine/endian.h>

#include "ppc/ppc.hpp"
#include "spdlog/spdlog.h"

namespace PPC {

  template <typename RetValWidth, typename InputWidth>
  RetValWidth get_bits(u32 n, std::pair<u8, u8> be_range) {
    const auto& [start, end] = be_range;

    assert(start < end);

    fmt::println("\nsizeof InputWidth * 8: {}", sizeof(InputWidth) * 8);
    fmt::println("start: {}", start);
    fmt::println("end: {}", end);
    fmt::println("eh: {}\n", ((sizeof(InputWidth) * 8) - end-1));




    // u8 BIT_LENGTH = 32;
    
    // 32 bit number, want to get range 6-11.

    // 1. shift out the 32-bit number so 6-11 moves to 0-5.
    // 2. 32 - 11 = 23 -- correct shift value

    // 32 bit number, want to get range 30-31.

    // 1. shift out the 32-bit number so 30-31 moves to 0-1.
    // 2. 32 - 31 = 1 -- incorrect shift value (supposed to be 31)



    

    // u8 shift_value =  BIT_LENGTH - end;


    n = (n >> ((sizeof(InputWidth) * 8) - end-1));

    fmt::println("n after shifting: {:#010b}", n);

    u16 mask = (1 << (end + 1 - start)) - 1;
    fmt::println("{:#010b}", mask);

    // 0b10000001111111110010000111111000

    if (start != 0) {
      n &= mask;
    }

    fmt::println("retval: {}\n", (RetValWidth)n);

    return n;
  }

  constexpr u8 STORE_DOUBLEWORD = 0x3E;

  void decode(u32 op, InstructionInfo& instruction_info) {
    // spdlog::info("opcd: {:#010b}", op);
    // spdlog::info("get_bits_result: {}", get_bits<u32>(op, {0, 6}));
    // spdlog::info("get_bits_result: {}", get_bits<u32>(op, {6, 11}));

    spdlog::info("opcd: {:#010b}", op);
    // op = ntohl(op);

    // first establish which format is used
    spdlog::info("opcd: {:#010b}", op);
    // spdlog::info("opcd: {:#010x}", (op >> 2) & 0x3f);
    // spdlog::info("rt: {:#010x}", (op >> 4) & 0x3f);
    // spdlog::info("ra: {:#010x}", (op & 0x3f) >> 2);
    // spdlog::info("xo: {:#010x}", (op & 0x3f) >> 2);

    u8 opcd = get_bits<u8, u32>(op, {0, 5});

    spdlog::info("x: {:#010x}", opcd);


        InstructionFormat format = InstructionFormat::UNDEFINED;

    format = opcode_info_map.at(opcd).instruction_format;

    switch (format) {
      case InstructionFormat::I_FORM: break;
      case InstructionFormat::B_FORM: break;
      case InstructionFormat::SC_FORM: break;
      case InstructionFormat::D_FORM: break;
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
              instruction_info.mnemnonic = fmt::format("stdu r{},{:#04x}(r{})",instruction_info.rs, instruction_info.ds, instruction_info.ra);
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

}  // namespace PPC