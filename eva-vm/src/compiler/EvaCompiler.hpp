/**
 * Eva Compiler
 */

#pragma once
#include "../bytecode/OpCode.hpp"
#include "../parser/EvaParser.h"
#include "../vm/EvaValue.hpp"

// Allocates new constants in the pool
#define ALLOC_CONST(tester, converter, allocator, value) \
  do {                                                   \
    for (auto i = 0; i < co->constants.size(); i++) {    \
      if (!tester(co->constants[i])) {                   \
        continue;                                        \
      }                                                  \
      if (converter(co->constants[i]) == value) {        \
        return i;                                        \
      }                                                  \
    }                                                    \
    co->constants.push_back(allocator(value));           \
  } while (false)

#define GEN_BINARY_OP(op) \
  do {                    \
    gen(exp.list[1]);     \
    gen(exp.list[2]);     \
    emit(op);             \
  } while (false)

/**
 * Compiler class, emits byte code, records constant pool, vars etc.
 */
class EvaCompiler {
 public:
  EvaCompiler() {}

  // Main Compile API
  CodeObject* compile(const Exp& exp) {
    // Allocate new code object:
    co = AS_CODE(ALLOC_CODE("main"));

    // Generate recursively from top-level
    gen(exp);

    // Explicit VM-stop marker.
    emit(OP_HALT);

    return co;
  }

  // Main compile loop
  void gen(const Exp& exp) {
    ExpType tagType;
    switch (exp.type) {
      // Numbers
      case ExpType::NUMBER:
        emit(OP_CONST);
        emit(numericConstIdx(exp.number));
        break;

      // Strings
      case ExpType::STRING:
        emit(OP_CONST);
        emit(stringConstIdx(exp.string));
        break;
        
      // Lists
      case ExpType::LIST:
        // auto tag = exp.list[0];
        tagType = exp.list[0].type;
        // Special cases
        if (tagType == ExpType::SYMBOL) {
          auto op = exp.list[0].string;

          // Binary MATH Operations
          if (op == "+") {
            GEN_BINARY_OP(OP_ADD);
          } else if(op == "-"){
            GEN_BINARY_OP(OP_SUB);
          } else if(op == "*"){
            GEN_BINARY_OP(OP_MUL);
          } else if(op == "/"){
            GEN_BINARY_OP(OP_DIV);
          }
        }
        break;

      default:
        break;
    }
  }

 private:
  // Allocates a string constant
  size_t stringConstIdx(const std::string& value) {
    ALLOC_CONST(IS_STRING, AS_CPPSTRING, ALLOC_STRING, value);
    return co->constants.size() - 1;
  }
  // Allocates a numeric constant
  size_t numericConstIdx(double value) {
    ALLOC_CONST(IS_NUMBER, AS_NUMBER, NUMBER, value);
    return co->constants.size() - 1;
  }
  // Emits data to the bytecode
  void emit(uint8_t code) { co->code.push_back(code); }
  // Compiling code object
  CodeObject* co;
};
