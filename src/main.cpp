
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
  auto stack_mapping = vmm.map(0x00000000fff00000, PPC64_PAGE_SIZE * 0xF);
  if (!stack_mapping) {
    FATAL("failed to allocate stack");
  }
  stack_mapping->print();

  spdlog::info("mapped stack");

  u64 entry_point = instance.cell_be.ppe.fetch(executableInfo.elf_header.e_entry);
  u32 toc         = instance.cell_be.ppe.fetch(executableInfo.elf_header.e_entry + 4);

  spdlog::info("e_entry: {:#016x} toc: {:#016x}", entry_point, toc);
  instance.cell_be.ppe.set_gpr(1, 0x000000fffe0000);
  instance.cell_be.ppe.set_gpr(2, toc);
  instance.cell_be.ppe.set_gpr(3, 0);
  instance.cell_be.ppe.set_gpr(4, 0x000000fffb0000);
  instance.cell_be.ppe.set_gpr(5, 0x000000fffb0000);


  // instance.cell_be.ppe.nia = entry_point;
  // instance.cell_be.ppe.cia = entry_point;
  instance.cell_be.ppe.nia = 0x10000;
  instance.cell_be.ppe.cia = 0x10000;

  Frontend f{&instance};

  while (f.state.running) {
    f.handle_events();
    f.render_frame();
  }

  f.shutdown();
  return 0;
}
