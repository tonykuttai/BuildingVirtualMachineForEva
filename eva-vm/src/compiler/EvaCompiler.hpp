/**
 * Eva Compiler
 */

#pragma once
#include <map>

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
          } else if (op == "-") {
            GEN_BINARY_OP(OP_SUB);
          } else if (op == "*") {
            GEN_BINARY_OP(OP_MUL);
          } else if (op == "/") {
            GEN_BINARY_OP(OP_DIV);
          } else if (compareOps_.count(op) != 0) {
            // compare operations: (> 5 10)
            gen(exp.list[1]);
            gen(exp.list[2]);
            emit(OP_COMPARE);
            emit(compareOps_[op]);
          } else if (op == "if") {
            // If conditional branch
            // (if <test> <consequent> <alternate>)
            gen(exp.list[1]);
            emit(OP_JMP_IF_FALSE);
            // use 2-byte addresses
            emit(0);  // Use 0 for now
            emit(0);
            auto elseJmpAddr = getOffSet() - 2; // remember the placeholder to patch the addresses later
            // Emit <consequent>
            gen(exp.list[2]);
            emit(OP_JMP);
            // 2-byte addresses Placeholders            
            emit(0);
            emit(0);
            auto endAddr = getOffSet() - 2; // remember the placeholder to patch the addresses later
            // Patch the else branch address
            auto elseBranchAddr = getOffSet();
            patchJumpAddress(elseJmpAddr, elseBranchAddr);
            // Emit the <alternate> if we have it
            if(exp.list.size() == 4){
              gen(exp.list[3]);
            }
            // Path the end
            auto endBranchAddr = getOffSet();
            patchJumpAddress(endAddr, endBranchAddr);
          }
        }
        break;

      // Symbols (variables, operators)
      case ExpType::SYMBOL:
        // Boolean
        if (exp.string == "true" || exp.string == "false") {
          emit(OP_CONST);
          emit(booleanConstIdx(exp.string == "true" ? true : false));
        } else {
          // TODO: variables
        }
        break;

      default:
        break;
    }
  }

 private:
  // Returns current bytecode offset
  size_t getOffSet() { return co->code.size(); }
  // Allocates a boolean constant
  size_t booleanConstIdx(bool value) {
    ALLOC_CONST(IS_BOOLEAN, AS_BOOLEAN, BOOLEAN, value);
    return co->constants.size() - 1;
  }

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
  // write byte at offset
  void writeByteAtOffset(size_t offset, uint8_t value){
    co->code[offset] = value;
  }
  // Patches Jump address
  void patchJumpAddress(size_t offset, uint16_t value){
    writeByteAtOffset(offset, (value >> 8) & 0xff); // isolate high byte and write 
    writeByteAtOffset(offset + 1, value & 0xff); //isolate low byte and write
  }
  // Compiling code object
  CodeObject* co;
  // Compare ops map
  static std::map<std::string, uint8_t> compareOps_;
};

std::map<std::string, uint8_t> EvaCompiler::compareOps_ = {
    {"<", 0}, {">", 1}, {"==", 2}, {">=", 3}, {"<=", 4}, {"!=", 5}};
