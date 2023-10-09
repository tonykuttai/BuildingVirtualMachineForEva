#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "Utilities/testutility.hpp"

using ::testing::StartsWith;

#include "../src/vm/EvaVM.hpp"

class EvaVMControlFlow : public ::testing::Test {
 protected:
  EvaVM vm;
  evatest::testutility tu{"console.log"};
  
  void consoleLogContainsExpected(const std::string& expected) {
    const std::string consoleLogActual = tu.readConsoleLogFile();
    // EXPECT_THAT(consoleLogActual, ::testing::ContainsRegex(expected));
    EXPECT_THAT(consoleLogActual, ::testing::HasSubstr(expected));
    tu.clearConsoleLog();
  }
};

TEST_F(EvaVMControlFlow, ControlFlowBranchInstructions) {
  std::string program = R"(
    (if (> 5 10) 1 2)
  )";
  // Execute the program and get the result.
  auto result = vm.exec(program);
  EXPECT_EQ(AS_NUMBER(result), 2);

  program = R"(
    (if (< 5 10) 1 2)
  )";
  // Execute the program and get the result.
  result = vm.exec(program);
  EXPECT_EQ(AS_NUMBER(result), 1);

  program = R"(
    (if (== 10 10) 1 2)
  )";
  // Execute the program and get the result.
  result = vm.exec(program);
  EXPECT_EQ(AS_NUMBER(result), 1);

  program = R"(
    (if (<= 9 10) 1 2)
  )";
  // Execute the program and get the result.
  result = vm.exec(program);
  EXPECT_EQ(AS_NUMBER(result), 1);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}