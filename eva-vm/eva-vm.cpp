/**
 * Eva VM Main executable
*/

#include "src/vm/EvaVM.hpp"
#include "src/Logger.hpp"

int main(int argc, char const *argv[]){

    EvaVM vm;
    auto result = vm.exec(R"(
        (+ 3 2)
    )");
    logE(result);

    result = vm.exec(R"(
        (+ "Hello " "World")
    )");
    logE(result);
    return 0;
}