
#include "cli11/CLI11.hpp"
#include "common.hpp"
#include "frontend/window.hpp"
#include "ppc/parser.hpp"
#include "self/self.hpp"
#include "sysinstance.hpp"
#include "vmm/vmm.hpp"

int handle_args(int& argc, char** argv, std::string& filename) {
  CLI::App app{"", "ps3mu"};
  app.add_option("-f,--folder", filename, "path to game folder/executable")->required();

  CLI11_PARSE(app, argc, argv);
  return 0;
}

int main(int argc, char** argv) {
  std::string filename = {};
  handle_args(argc, argv, filename);

  // PPC::InstructionInfo instructionInfo;

  std::vector<u8> file = read_file(filename);
  auto executableInfo  = parse_self(file);

  SysInstance instance;

  load_self(file, executableInfo, instance.bus.memory);

  fmt::println("e_entry: {:#016x}", executableInfo.elf_header.e_entry);

  u32 entry_point = instance.cell_be.ppe.fetch(executableInfo.elf_header.e_entry);

  instance.cell_be.ppe.nia = entry_point;

  Frontend f{&instance};

  while (f.state.running) {
    f.handle_events();
    f.render_frame();
  }

  f.shutdown();

  //   vmm.get_page_number(0x000000ffffff90);

  // vmm.
  // system.join();

  // 0x000000ffffff90 == 0x0000000

  return 0;
}
