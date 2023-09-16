/**
 * Eva VM Main executable
*/

#include "src/vm/EvaVM.hpp"

int main(int argc, char const *argv[]){

    EvaVM vm;
    vm.exec(R"(
        42
    )");
    return 0;
}