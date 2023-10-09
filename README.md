# BuildingVirtualMachineForEva
Building a Virtual Machine for Eva Programming Language

## Control Flow and Variables

### Control Flow | Branch Instruction
The if conditional statement
```
(if <test> 
    <consequent>
    <alternate>)
```

`(if (>5 10) 1 2)`

The construction of the If statement Machine code looks like this:
```
<test>
JMP_IF_FALSE
<alternate-begin-address>
<consequent>
JMP
<alternate-end-address>
<alternate>
...
```
For the example `(if (> 5 10) 1 2)`, the machine code emitted would be: The byte address 00 would be patched later to the correct address.
```
<test>          CONST 0 (5)
<test>          CONST 1 (10)
<test>          COMPARE 1 (>)
                JMP_IF_FALSE
                00
                00
<consequent>    CONST 2 (1)
                JMP
                00
                00
<alternate>     CONST 3 (2)
                ...
```

### Disassembler

![Disassembler](eva-vm/Resources/SnapShots/#2%20Disassembler.png)