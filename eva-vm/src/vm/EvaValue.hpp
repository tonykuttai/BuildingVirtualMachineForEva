/**
 * EvaValue
*/

#pragma once
#include <string>

// Eva value type
enum class EvaValueType {
    NUMBER,
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
        union{
            double number;
            Object* object;
        };
};


// String Object
struct StringObject : public Object {
    StringObject(const std::string &str) : Object(ObjectType::STRING), string(str) {}
    std::string string;
};

// Code Object
// Contains compiling bytecode, locals and other state needed for function execution.
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
#define NUMBER(value) ((EvaValue){EvaValueType::NUMBER, {value}})
#define ALLOC_STRING(value) ((EvaValue){EvaValueType::OBJECT, .object = (Object*) new StringObject(value)})
#define ALLOC_CODE(value) ((EvaValue){EvaValueType::OBJECT, .object = (Object*) new CodeObject(value)})

// Accessors
#define AS_NUMBER(evaValue) ((double)(evaValue).number)
#define AS_STRING(evaValue) ((StringObject*)(evaValue).object)
#define AS_CODE(evaValue) ((CodeObject*)(evaValue).object)
#define AS_OBJECT(evaValue) ((Object*)(evaValue).object)
#define AS_CPPSTRING(evaValue) (AS_STRING(evaValue)->string)

// Predicate functions
#define IS_NUMBER(evaValue) ((evaValue).type == EvaValueType::NUMBER)
#define IS_OBJECT(evaValue) ((evaValue).type == EvaValueType::OBJECT)
#define IS_OBJECT_TYPE(evaValue, objectType) \
        (IS_OBJECT(evaValue) && AS_OBJECT(evaValue)->type == objectType)
#define IS_STRING(evaValue) \
        (IS_OBJECT(evaValue) && IS_OBJECT_TYPE(evaValue, ObjectType::STRING))
#define IS_CODE(evaValue) \
        (IS_OBJECT(evaValue) && IS_OBJECT_TYPE(evaValue, ObjectType::CODE))