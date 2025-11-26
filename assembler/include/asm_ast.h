#ifndef ASSEMBLER_AST_H
#define ASSEMBLER_AST_H
#include <definitions.h>

struct Operand {
    enum class Kind { Reg, Imm, Mem, LabelRef } kind;

    int reg = 0; // if register
    int imm = 0; // if immediate or offset
    int baseReg = 0; // if Mem (imm(baseReg))
    std::string label; // if LabelRef
};

struct Instruction {
    std::string op; // "addi", "lw", "j", ...
    std::vector<Operand> args;
    int line = 0;
};

struct Line {
    std::string label; // "" if empty
    bool hasInstr = false;
    Instruction instr;
};

#endif
