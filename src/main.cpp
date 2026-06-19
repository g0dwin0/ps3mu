
#include "common.hpp"
#include "ppc/parser.hpp"
#include "sysinstance.hpp"
#include "cli11/CLI11.hpp"
#include "frontend/window.hpp"

int handle_args(int& argc, char** argv, std::string& filename) {
  CLI::App app{"", "ps3mu"};
  app.add_option("-f,--file", filename, "path to executable")->required();

  CLI11_PARSE(app, argc, argv);
  return 0;
}

int main(int argc, char** argv) {
  std::string filename = {};
  handle_args(argc, argv, filename);
  
  // SysInstance SysInstance = {};


  

  auto result = PPC::decode(0xf821ff81);

  auto str = fmt::format("opcd: %04X", result.opcode);
  
  spdlog::info(str);
  // Frontend f{&SysInstance};
  
  // std::vector<u8> file = read_file(filename);
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
