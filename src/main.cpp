
#include "cli11/CLI11.hpp"
#include "common.hpp"
#include "frontend/window.hpp"
#include "ppc/parser.hpp"
#include "self/self.hpp"
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

  std::vector<u8> file = read_file(filename);

  parse_self(file);

  return 0;
}
