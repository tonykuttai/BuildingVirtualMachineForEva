/**
 * Logger and error reporter
*/

#include <sstream>

#pragma once

class ErrorLogMessage : public std::basic_ostringstream<char> {
private:
    /* data */
public:
    ~ErrorLogMessage(){
        fprintf(stderr, "Fatal error: %s\n", str().c_str());
        exit(EXIT_FAILURE);
    }
};

#define DIE ErrorLogMessage()
#define log(value) std::cout<< #value << " = " << (value) << std::endl