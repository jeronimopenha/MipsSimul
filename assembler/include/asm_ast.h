#ifndef ASSEMBLER_AST_H
#define ASSEMBLER_AST_H

#include <definitions.h>

struct AsmOperand {
    enum Kind {
        REG,
        IMM,
        MEM,
        LABELREF
    } kind;

    int reg = 0; // if register
    int imm = 0; // if immediate or offset
    int baseReg = 0; // if Mem (imm(baseReg))
    std::string label; // if LabelRef
};

struct MiniCInstruction {
    std::string op; // "addi", "lw", "j", ...
    std::vector<AsmOperand> args;
    int line = 0;
};

struct AsmLine {
    std::string label; // "" if empty
    bool hasInstr = false;
    MiniCInstruction instr;
};

#endif
