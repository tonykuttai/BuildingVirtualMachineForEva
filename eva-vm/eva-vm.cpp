/**
 * Eva VM Main executable
 */

#include "src/Logger.hpp"
#include "src/vm/EvaVM.hpp"

int main(int argc, char const *argv[]) {
  std::string program = R"(
    (> 5 2)
  )";
  EvaVM vm;
  auto result = vm.exec(program);
  logE(result);
  return 0;
}