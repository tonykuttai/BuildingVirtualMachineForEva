/**
 * Eva Virtual Machine
 */

#pragma once

#include <array>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "../Logger.hpp"
#include "../bytecode/OpCode.hpp"
#include "../parser/EvaParser.h"
#include "../compiler/EvaCompiler.hpp"
#include "EvaValue.hpp"

using syntax::EvaParser;

class EvaValue;

#define READ_BYTE() *ip++
#define GET_CONST() co->constants[READ_BYTE()]
#define STACK_LIMIT 512
#define BINARY_OP(op)            \
  do {                           \
    auto op2 = AS_NUMBER(pop()); \
    auto op1 = AS_NUMBER(pop()); \
    push(NUMBER(op1 op op2));    \
  } while (false)

class EvaVM {
 private:
  /* data */
 public:
  EvaVM()
      : parser(std::make_unique<EvaParser>()),
        compiler(std::make_unique<EvaCompiler>()) {
    // Init the stack
    sp = stack.begin();
  }

  // push : Pushes value to the stack
  void push(const EvaValue& aValue) {
    if ((sp - stack.begin()) == STACK_LIMIT) {
      DIE("push(): Stack overflow");
    }
    *sp = aValue;
    sp++;
  }
  // pop: Pos a value from the stack
  EvaValue pop() {
    if (sp == stack.begin()) {
      DIE("pop(): empty stack.");
    }
    --sp;
    return *sp;
  }
  EvaValue exec(const std::string& program) {
    // 1. Parse the program
    auto ast = parser->parse(program);

    // 2. compile program to Eva bytecode
    co = compiler->compile(ast);
    
    // Set instruction pointer to the beginning
    ip = &co->code[0];
    
    // Init the stack
    sp = stack.begin();

    return eval();
  }

  // Main eval loop
  EvaValue eval() {
    for (;;) {
      auto opcode = READ_BYTE();
      // log(opcode);
      switch (opcode) {
        case OP_HALT:
          return pop();
        // Constants
        case OP_CONST:
          push(GET_CONST());
          break;
        case OP_ADD: {
          // BINARY_OP(+);
          auto op2 = pop();
          auto op1 = pop();

          if (IS_NUMBER(op1) && IS_NUMBER(op2)) {
            // Numeric addition
            auto v1 = AS_NUMBER(op1);
            auto v2 = AS_NUMBER(op2);
            push(NUMBER(v1 + v2));
          } else if (IS_STRING(op1) && IS_STRING(op2)) {
            // String concatenation
            auto s1 = AS_CPPSTRING(op1);
            auto s2 = AS_CPPSTRING(op2);
            push(ALLOC_STRING(s1 + s2));
          }
          break;
        }
        case OP_SUB:
          BINARY_OP(-);
          break;
        case OP_MUL:
          BINARY_OP(*);
          break;
        case OP_DIV:
          BINARY_OP(/);
          break;
          // default:
          //     ss << "0x" << std::hex << std::setw(2) << std::setfill('0') <<
          //     opcode << std::endl; DIE("Unknown opcode: " + ss.str());
      }
    }
  }

  // Parser
  std::unique_ptr<EvaParser> parser;
  // Compiler
  std::unique_ptr<EvaCompiler> compiler;

  // Instruction Pointer (aka Program counter)
  uint8_t* ip;
  
  // Stack pointer
  EvaValue* sp;
  // Stack
  std::array<EvaValue, STACK_LIMIT> stack;
  // Stringstream
  std::stringstream ss;
  // Code object
  CodeObject* co;
};