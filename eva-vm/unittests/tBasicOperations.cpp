#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::StartsWith;

#include "../src/vm/EvaVM.hpp"

class EvaVMTest : public ::testing::Test {
 protected:
  EvaVM vm;
  // Helper function to read the contents of the console.log file
  std::string readConsoleLogFile(const std::string& filename) {
    std::ifstream logFile(filename);
    if (!logFile.is_open()) {
      throw std::runtime_error("Failed to open " + filename);
    }

    std::string content((std::istreambuf_iterator<char>(logFile)),
                        std::istreambuf_iterator<char>());
    logFile.close();
    return content;
  }
};

TEST_F(EvaVMTest, NumberConstants) {
  std::string program = R"(
    3
  )";

  // Execute the program and get the result.
  auto result = vm.exec(program);

  // Assert that the result is 3.
  EXPECT_EQ(AS_NUMBER(result), 3);
}

TEST_F(EvaVMTest, StringConstants) {
  // Create a simple program that multiplies two numbers and subtracts a third
  // number.
  std::string program = R"(
    "Hello World" // this is a dummy insert.
  )";

  // Execute the program and get the result.
  auto result = vm.exec(program);

  // Assert that the result is 419.
  EXPECT_EQ(AS_CPPSTRING(result), "Hello World");
}

TEST_F(EvaVMTest, BinaryOperations) {
  // Create a simple program that multiplies two numbers and subtracts a third
  // number.
  std::string program = R"(
    (+ 3 2)
  )";

  // Execute the program and get the result.
  auto result = vm.exec(program);

  // Assert that the result is 5.
  EXPECT_EQ(AS_NUMBER(result), 5);

  program = R"(
    (+ 3 (* 2 4))
  )";

  // Execute the program and get the result.
  result = vm.exec(program);

  // Assert that the result is 11.
  EXPECT_EQ(AS_NUMBER(result), 11);

  program = R"(
    (- 25 (+ 3 (* 2 4)))
  )";

  // Execute the program and get the result.
  result = vm.exec(program);

  // Assert that the result is 14.
  EXPECT_EQ(AS_NUMBER(result), 14);

  program = R"(
    (/ (- 25 (+ 3 (* 2 4))) 7)
  )";

  // Execute the program and get the result.
  result = vm.exec(program);

  // Assert that the result is 2.
  EXPECT_EQ(AS_NUMBER(result), 2);
}

TEST_F(EvaVMTest, LoggerTest) {
  std::string program = R"(
    3
  )";

  // Execute the program and get the result.
  auto result = vm.exec(program);

  logE(result);

  const std::string expected = "EvaValue (NUMBER): 3";
  const std::string consoleLogActual = readConsoleLogFile("console.log");

  // Assert that the result is 3.
  EXPECT_THAT(consoleLogActual, StartsWith(expected));
}

// TEST_F(EvaVMTest, LoggerCodeTest) {
//   std::string program = R"(
//     (if (> 5 10) 1 2)
//   )";

//   // Execute the program and get the result.
//   //auto result = vm.exec(program);

//   //logE(result);

//   // const std::string expected = "EvaValue (NUMBER): 3";
//   // const std::string consoleLogActual = readConsoleLogFile("console.log");

//   // Assert that the result is 3.
//   //EXPECT_THAT(consoleLogActual, StartsWith(expected));
// }

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}