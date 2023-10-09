/**
 * EvaValue
 */

#pragma once
#include <iostream>
#include <sstream>
#include <string>

#include "../Logger.hpp"

// Eva value type
enum class EvaValueType {
  NUMBER,
  BOOLEAN,
  OBJECT,
};

// Object type
enum class ObjectType {
  STRING,
  CODE,
};

// Base Object
struct Object {
  Object(ObjectType type) : type(type) {}
  ObjectType type;
};

// Eva Value
class EvaValue {
 public:
  EvaValueType type;
  union {
    double number;
    bool boolean;
    Object* object;
  };
};

// String Object
struct StringObject : public Object {
  StringObject(const std::string& str)
      : Object(ObjectType::STRING), string(str) {}
  std::string string;
};

// Code Object
// Contains compiling bytecode, locals and other state needed for function
// execution.
struct CodeObject : public Object {
  CodeObject(const std::string& name) : Object(ObjectType::CODE), name(name) {}
  // Name of the code unit (usually function name)
  std::string name;
  // Contant Pool
  std::vector<EvaValue> constants;
  // ByteCode
  std::vector<uint8_t> code;
};

// Constructors
#define NUMBER(value) ((EvaValue){EvaValueType::NUMBER, {.number = value}})
#define BOOLEAN(value) ((EvaValue){EvaValueType::BOOLEAN, {.boolean = (value)}})
#define ALLOC_STRING(value) \
  ((EvaValue){EvaValueType::OBJECT, .object = (Object*)new StringObject(value)})
#define ALLOC_CODE(value) \
  ((EvaValue){EvaValueType::OBJECT, .object = (Object*)new CodeObject(value)})

// Accessors
#define AS_NUMBER(evaValue) ((double)(evaValue).number)
#define AS_BOOLEAN(evaValue) ((bool)(evaValue).boolean)
#define AS_STRING(evaValue) ((StringObject*)(evaValue).object)
#define AS_CODE(evaValue) ((CodeObject*)(evaValue).object)
#define AS_OBJECT(evaValue) ((Object*)(evaValue).object)
#define AS_CPPSTRING(evaValue) (AS_STRING(evaValue)->string)

// Predicate functions
#define IS_NUMBER(evaValue) ((evaValue).type == EvaValueType::NUMBER)
#define IS_BOOLEAN(evaValue) ((evaValue).type == EvaValueType::BOOLEAN)
#define IS_OBJECT(evaValue) ((evaValue).type == EvaValueType::OBJECT)
#define IS_OBJECT_TYPE(evaValue, objectType) \
  (IS_OBJECT(evaValue) && AS_OBJECT(evaValue)->type == objectType)
#define IS_STRING(evaValue) \
  (IS_OBJECT(evaValue) && IS_OBJECT_TYPE(evaValue, ObjectType::STRING))
#define IS_CODE(evaValue) \
  (IS_OBJECT(evaValue) && IS_OBJECT_TYPE(evaValue, ObjectType::CODE))

// String representation used in constants for debug
std::string evaValueToTypeString(const EvaValue& evaValue) {
  if (IS_NUMBER(evaValue)) {
    return "NUMBER";
  } else if (IS_BOOLEAN(evaValue)) {
    return "BOOLEAN";
  } else if (IS_STRING(evaValue)) {
    return "STRING";
  } else if (IS_CODE(evaValue)) {
    return "CODE";
  } else {
    std::stringstream ss;
    ss << "evaValueToTypeString: unknown type" << (int)evaValue.type;
    DIE(ss.str());
  }
  return "";  // unreachable
}

// String representation used in constants for debug
std::string evaValueToConstantString(const EvaValue& evaValue) {
  std::stringstream ss;
  if (IS_NUMBER(evaValue)) {
    ss << evaValue.number;
  } else if (IS_BOOLEAN(evaValue)) {
    ss << std::boolalpha << evaValue.boolean;
    ss << std::noboolalpha;
  } else if (IS_STRING(evaValue)) {
    ss << '"' << AS_CPPSTRING(evaValue) << '"';
  } else if (IS_CODE(evaValue)) {
    auto code = AS_CODE(evaValue);
    ss << "code " << code << " : " << code->name;
  } else {
    ss << "evaValueToConstantString: unknown type" << (int)evaValue.type;
    DIE(ss.str());
  }
  return ss.str();
}
// Output Stream
std::ostream& operator<<(std::ostream& os, const EvaValue& evaValue) {
  return os << "EvaValue (" << evaValueToTypeString(evaValue)
            << "): " << evaValueToConstantString(evaValue);
}