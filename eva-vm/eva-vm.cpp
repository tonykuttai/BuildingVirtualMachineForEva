/**
 * Eva VM Main executable
*/

#include "src/vm/EvaVM.hpp"
#include "src/Logger.hpp"

int main(int argc, char const *argv[]){

    EvaVM vm;
    auto result = vm.exec(R"(
        42
    )");
    log(result.number);
    std::cout << "Verified !" << std::endl;
    return 0;
}