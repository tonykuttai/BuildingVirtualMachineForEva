/**
 * Eva Virtual Machine
*/

#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "../bytecode/OpCode.hpp"
#include "../Logger.hpp"

#define READ_BYTE() *ip++

class EvaVM{
private:
    /* data */
public:
    EvaVM(){

    }
    void exec(const std::string &program){
        // 1. Parse the program
        // auto ast = parser->parse(program)

        // 2. compile program to Eva bytecode
        // code = compiler->compile(ast);

        code = {OP_HALT};
        ip = &code[0];

        return eval();
    }

    // Main eval loop
    void eval(){
        for(;;){
            auto opcode = READ_BYTE();
            switch (opcode) {
                case OP_HALT:
                    return;
                default:
                    DIE << "Unknown opcode: " << std::hex << opcode << std::endl;

            }
        }
    }

    // Instruction POinter (aka Program counter)
    uint8_t* ip;

    // ByteCode
    std::vector<uint8_t> code;
};