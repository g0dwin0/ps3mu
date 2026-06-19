
#include "ppc/parser.hpp"

#include <format>
#include <stdexcept>
#include <utility>

#include "common/defs.hpp"
#include "endian.h"
#include "ppc/ppc.hpp"
#include "spdlog/spdlog.h"

namespace PPC {

  constexpr u8 STORE_DOUBLEWORD = 0x3E;

  InstructionInfo decode(u32 op) {
    // first establish which format is used

    spdlog::info("x: {:#010x}", be32toh(op));
    spdlog::info("be: {:#010x}", (op & 0xff) >> 2);
    spdlog::info("le: {:#010x}", (op & 0xff) >> 2);
    spdlog::info("be: {:#010x}", (op & 0xff) >> 2);
    spdlog::info("le: {:#010x}", (op & 0xff) >> 2);

    u8 opcd = (u8)(op >> 26);

    spdlog::info("x: {:#010x}", opcd);

    InstructionFormat format = InstructionFormat::UNDEFINED;

    if (opcode_info_map.contains(opcd)) {
      format = opcode_info_map.at(opcd).instruction_format;
    }

    switch (format) {
      case InstructionFormat::I_FORM: break;
      case InstructionFormat::B_FORM: break;
      case InstructionFormat::SC_FORM: break;
      case InstructionFormat::D_FORM: break;
      case InstructionFormat::DS_FORM: {
        switch (opcd) {
          case STORE_DOUBLEWORD: {

            if (op & 1) {
              fmt::println("stdu");
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

    return {};
  };

}  // namespace PPC