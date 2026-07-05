
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

}  // namespace PPC