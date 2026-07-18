
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
  SysInstance instance;

  std::vector<u8> file = read_file(filename);
  auto executableInfo  = parse_exec(file);

  load_self(file, executableInfo, instance.bus.memory);

  if (!vmm.map(0xFFFFFFFFFFF00000, 0x100000)) {
    FATAL("failed to allocate stack");
  }
  fmt::println("mapped stack");

  u64 entry_point = instance.cell_be.ppe.fetch(executableInfo.elf_header.e_entry);
  u32 toc         = instance.cell_be.ppe.fetch(executableInfo.elf_header.e_entry + 4);

  fmt::println("e_entry: {:#016x} toc: {:#016x}", entry_point, toc);
  instance.cell_be.ppe.set_reg(2, toc);

  instance.cell_be.ppe.nia = entry_point;
  instance.cell_be.ppe.cia = entry_point;

  Frontend f{&instance};

  while (f.state.running) {
    f.handle_events();
    f.render_frame();
  }

  f.shutdown();
  return 0;
}
