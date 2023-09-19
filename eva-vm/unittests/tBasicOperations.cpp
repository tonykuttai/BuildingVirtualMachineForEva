#include <gtest/gtest.h>
#include "../src/vm/EvaVM.hpp"

class EvaVMTest : public ::testing::Test {
protected:
  EvaVM vm;
};

// TEST_F(EvaVMTest, NumberConstants) {
//   // Create a simple program that multiplies two numbers and subtracts a third number.
//   std::string program = R"(
//     42 * 10 - 3 // this is a dummy insert.
//   )";

//   // Execute the program and get the result.
//   auto result = vm.exec(program);

//   // Assert that the result is 419.
//   EXPECT_EQ(AS_NUMBER(result), 20.0);
// }

TEST_F(EvaVMTest, StringConstants) {
  // Create a simple program that multiplies two numbers and subtracts a third number.
  std::string program = R"(
    "Hello World" // this is a dummy insert.
  )";

  // Execute the program and get the result.
  auto result = vm.exec(program);

  // Assert that the result is 419.
  EXPECT_EQ(AS_CPPSTRING(result), "Hello World");
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}