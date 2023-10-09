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
#include "../compiler/EvaCompiler.hpp"
#include "../parser/EvaParser.h"
#include "EvaValue.hpp"

using syntax::EvaParser;

class EvaValue;

#define READ_BYTE() *ip++
// Read a short word (2 bytes)
#define READ_SHORT() (ip += 2, (uint16_t)((ip[-2] << 8) | ip[-1]))
// Converts bytecode index to a pointer
#define TO_ADDRESS(index) (&co->code[index])
#define GET_CONST() (co->constants[READ_BYTE()])
#define STACK_LIMIT 512
#define BINARY_OP(op)            \
  do {                           \
    auto op2 = AS_NUMBER(pop()); \
    auto op1 = AS_NUMBER(pop()); \
    push(NUMBER(op1 op op2));    \
  } while (false)

#define COMPARE_VALUES(op, v1, v2) \
  do {                             \
    bool res;                      \
    switch (op) {                  \
      case 0:                      \
        res = v1 < v2;             \
        break;                     \
      case 1:                      \
        res = v1 > v2;             \
        break;                     \
      case 2:                      \
        res = v1 == v2;            \
        break;                     \
      case 3:                      \
        res = v1 >= v2;            \
        break;                     \
      case 4:                      \
        res = v1 <= v2;            \
        break;                     \
      case 5:                      \
        res = v1 != v2;            \
        break;                     \
    }                              \
    push(BOOLEAN(res));            \
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
        case OP_COMPARE: {
          auto op = READ_BYTE();
          auto op2 = pop();
          auto op1 = pop();
          if (IS_NUMBER(op1) && IS_NUMBER(op2)) {
            auto v1 = AS_NUMBER(op1);
            auto v2 = AS_NUMBER(op2);
            COMPARE_VALUES(op, v1, v2);
          } else if (IS_STRING(op1) && IS_STRING(op2)) {
            auto v1 = AS_CPPSTRING(op1);
            auto v2 = AS_CPPSTRING(op2);
            COMPARE_VALUES(op, v1, v2);
          }
          break;
        }
        case OP_JMP_IF_FALSE: {
          // Conditional Jump
          auto cond = AS_BOOLEAN(pop());
          auto address = READ_SHORT();
          if(!cond){
             ip = TO_ADDRESS(address);
          }
          break;
        }
        case OP_JMP:{
          // un conditional jump
          ip = TO_ADDRESS(READ_SHORT());
          break;
        }
        default:
          ss << "Unknown Opcode: " << std::hex << std::setw(2)
             << std::setfill('0') << opcode << std::endl;
          DIE(ss.str());
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