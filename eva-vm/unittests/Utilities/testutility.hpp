#include <string>
#include <fstream>

namespace evatest {

class testutility {
 private:
  std::string consoleLog;

 public:
  testutility(std::string name) : consoleLog{name} {}

  // Helper function to read the contents of the console.log file
  std::string readConsoleLogFile() {
    std::ifstream logFile(consoleLog);
    if (!logFile.is_open()) {
      throw std::runtime_error("Failed to open " + consoleLog);
    }

    std::string content((std::istreambuf_iterator<char>(logFile)),
                        std::istreambuf_iterator<char>());
    logFile.close();
    return content;
  }

  void clearConsoleLog() {
    std::ifstream logFile(consoleLog, std::ofstream::out);
    if (!logFile.is_open()) {
      throw std::runtime_error("Failed to open " + consoleLog);
    }
    logFile.close();
  }
};

}  // namespace evatest
