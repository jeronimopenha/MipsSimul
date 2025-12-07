#ifndef ASSEMBLER_REGISTERS_H
#define ASSEMBLER_REGISTERS_H

#include <definitions.h>

//extern const std::array<std::string, 32> REG_NAME;
const std::array<std::string, 32> REG_NAME = {
    "$zero",
    "$at",
    "$v0", "$v1",
    "$a0", "$a1", "$a2", "$a3",
    "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7",
    "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7",
    "$t8", "$t9",
    "$k0", "$k1",
    "$gp",
    "$sp",
    "$fp",
    "$ra"
};

int regNumber(const std::string &name);

std::string regNameFromNumber(int n);

#endif
