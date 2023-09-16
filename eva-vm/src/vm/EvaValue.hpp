/**
 * EvaValue
*/

#pragma once

// Eva value type
enum class EvaValueType {
    NUMBER,
};

// Eva Value

class EvaValue {
    public:        
        EvaValueType type;
        union{
            double number;
        };
};

#define NUMBER(value) ((EvaValue){EvaValueType::NUMBER, value})