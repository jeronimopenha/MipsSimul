#ifndef ASSEMBLER_REGISTERS_H
#define ASSEMBLER_REGISTERS_H

#include <definitions.h>

extern const std::array<std::string, 32> REG_NAME;

int regNumber(const std::string &name);

std::string regNameFromNumber(int n);

#endif
