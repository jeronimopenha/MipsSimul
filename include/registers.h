#ifndef MIPSSIMUL_REGISTERS_H
#define MIPSSIMUL_REGISTERS_H

#include <definitions.h>

extern const std::array<std::string, 32> REG_NAME;

int regNumber(const std::string &name);

#endif //MIPSSIMUL_REGISTERS_H
