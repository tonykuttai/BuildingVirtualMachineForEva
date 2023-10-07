/**
 * Eva Compiler
 */

#pragma once
#include "../bytecode/OpCode.hpp"
#include "../parser/EvaParser.h"
#include "../vm/EvaValue.hpp"

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

      default:
        break;
    }
  }

 private:
  // Allocates a string constant
  size_t stringConstIdx(const std::string& value) {
    for (auto i = 0; i < co->constants.size(); i++) {
      if (!IS_STRING(co->constants[i])) {
        continue;
      }
      if (AS_CPPSTRING(co->constants[i]) == value) {
        return i;
      }
    }
    co->constants.push_back(ALLOC_STRING(value));
    return co->constants.size() - 1;
  }
  // Allocates a numeric constant
  size_t numericConstIdx(double value) {
    for (auto i = 0; i < co->constants.size(); i++) {
      if (!IS_NUMBER(co->constants[i])) {
        continue;
      }
      if (AS_NUMBER(co->constants[i]) == value) {
        return i;
      }
    }
    co->constants.push_back(NUMBER(value));
    return co->constants.size() - 1;
  }
  // Emits data to the bytecode
  void emit(uint8_t code) { co->code.push_back(code); }
  // Compiling code object
  CodeObject* co;
};
