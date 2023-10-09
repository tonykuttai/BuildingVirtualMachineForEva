/**
 * Logger and error reporter
*/

// #include <sstream>

#pragma once

// class ErrorLogMessage : public std::basic_ostringstream<char> {
// private:
//     /* data */
// public:
//     ~ErrorLogMessage(){
//         fprintf(stderr, "Fatal error: %s\n", str().c_str());
//         exit(EXIT_FAILURE);
//     }
// };

// #define DIE ErrorLogMessage()
// #define log(value) std::cout<< #value << " = " << (value) << std::endl

#include <iostream>
#include <sstream>
#include <fstream>

class CustomLogger : public std::basic_ostringstream<char>  {
private:
    std::ofstream logFile;

public:
    CustomLogger(const std::string& filename) {
        logFile.close();
        logFile.open(filename, std::ofstream::out | std::ofstream::trunc);
        if (!logFile.is_open()) {
            std::cerr << "Error: Could not open log file." << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    ~CustomLogger() {
        logFile.close();        
    }

    void errorLogMessage(const std::string& msg){
        fprintf(stderr, "Fatal error: %s\n", str().c_str());
        exit(EXIT_FAILURE);
    }

    // template <typename T>
    // void log(const std::string& varName, const T& value) {
    //     if(varName == "opcode"){
    //         logFile << varName << " = 0x" << std::hex  << std::to_string(value) << std::endl;
    //         std::cout << varName << " = 0x" << std::hex  << std::to_string(value) << std::endl;
    //     }else{
    //         logFile << varName << " = " << std::to_string(value) << std::endl;
    //         std::cout << varName << " = " << std::to_string(value) << std::endl;
    //     }
    // }

    template <typename T>
    void logE(const T& evaValue){
        logFile << evaValue << std::endl;
        // std::cout << evaValue << std::endl;
        logFile.flush();
    }
};

// Create a global instance of the CustomLogger class
CustomLogger logger("console.log");

// Define a macro for logging
// #define log(value) logger.log(#value, value)
#define logE(value) logger.logE(value)
#define DIE(msg) logger.errorLogMessage(#msg)
