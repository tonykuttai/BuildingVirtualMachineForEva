/**
 * Eva VM Main executable
*/

#include "src/vm/EvaVM.hpp"
#include "src/Logger.hpp"

int main(int argc, char const *argv[]){

    EvaVM vm;
    auto result = vm.exec(R"(
        (+ "Hello, " "World!")
    )");
    log(AS_NUMBER(result));
    std::cout << "Verified !" << std::endl;
    return 0;
}