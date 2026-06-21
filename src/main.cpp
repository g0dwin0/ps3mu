
#include "cli11/CLI11.hpp"
#include "common.hpp"
#include "frontend/window.hpp"
#include "ppc/parser.hpp"
#include "sysinstance.hpp"

int handle_args(int& argc, char** argv, std::string& filename) {
  CLI::App app{"", "ps3mu"};
  app.add_option("-f,--folder", filename, "path to game folder/executable")->required();

  CLI11_PARSE(app, argc, argv);
  return 0;
}

int main(int argc, char** argv) {
  std::string filename = {};
  handle_args(argc, argv, filename);

  // SysInstance SysInstance = {};

  PPC::InstructionInfo instruction_info{};

  PPC::decode(0xf821ff81, instruction_info);

  auto str = fmt::format("opcd: {:#08X}", instruction_info.opcode);

  spdlog::info(str);
  // Frontend f{&SysInstance};

  std::vector<u8> file = read_file(filename);
  // agb.bus.pak->load_data(file);
  // SDL_SetWindowTitle(f.window, std::format("ps3mu | {}", agb.pak.info.game_title).c_str());

  // while (f.state.running) {
  //   f.handle_events();
  //   f.render_frame();
  // }

  // f.shutdown();

  // system.join();

  return 0;
}
