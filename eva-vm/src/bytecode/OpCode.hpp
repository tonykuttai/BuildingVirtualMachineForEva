#pragma once
// Instruction Set of Eva Virtual Machine

// Stop the program
#define OP_HALT 0x00
// Pushes a const onto the stack
#define OP_CONST 0x01
// Add instruction
#define OP_ADD 0x02
// Sub instruction
#define OP_SUB 0x03
// Multiplication instruction
#define OP_MUL 0x04
// Divison instruction
#define OP_DIV 0x05
// Comparison
#define OP_COMPARE 0x06
// Jump
#define OP_JMP_IF_FALSE 0x07
#define OP_JMP 0x08