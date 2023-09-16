/**
 * Eva Virtual Machine
*/

#pragma once

#include <iostream>

#include <array>
#include <string>
#include <vector>
#include <sstream>

#include "../bytecode/OpCode.hpp"
#include "../Logger.hpp"
#include "EvaValue.hpp"

#define READ_BYTE() *ip++
#define GET_CONST() constants[READ_BYTE()]
#define STACK_LIMIT 512

class EvaVM{
private:
    /* data */
public:
    EvaVM(){
        sp = stack.begin();
    }

    // push : Pushes value to the stack
    void push(const EvaValue& aValue){
        if((sp - stack.begin()) == STACK_LIMIT ){
            DIE("push(): Stack overflow");
        }
        *sp = aValue;
        sp++;
    }
    // pop: Pos a value from the stack
    EvaValue pop(){
        if(sp == stack.begin()){
            DIE("pop(): empty stack.");
        }
        --sp;
        return *sp;
    }
    EvaValue exec(const std::string &program){
        // 1. Parse the program
        // auto ast = parser->parse(program)

        // 2. compile program to Eva bytecode
        // code = compiler->compile(ast);

        constants.push_back(NUMBER(42));

        code = {OP_CONST, 0, OP_HALT};
        ip = &code[0];

        return eval();
    }

    // Main eval loop
    EvaValue eval(){
        for(;;){
            auto opcode = READ_BYTE();
            log(opcode);
            switch (opcode) {
                case OP_HALT:
                    return pop();
                // Constants
                case OP_CONST:
                    push(GET_CONST());
                    break;
                default:
                    ss << "0x" << std::hex << std::setw(2) << std::setfill('0') << opcode << std::endl;
                    DIE("Unknown opcode: " + ss.str());

            }
        }
    }

    // Instruction Pointer (aka Program counter)
    uint8_t* ip;
    // Contant Pool
    std::vector<EvaValue> constants;
    // ByteCode
    std::vector<uint8_t> code;
    // Stack pointer
    EvaValue* sp;
    // Stack
    std::array<EvaValue, STACK_LIMIT> stack;
    // Stringstream
    std::stringstream ss;
};