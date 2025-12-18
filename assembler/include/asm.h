#ifndef ASSEMBLER_ASSEMBLER_H
#define ASSEMBLER_ASSEMBLER_H

#include <definitions.h>
#include <asm_parser.h>

struct AsmInstrInfo {
    uint8_t opcode;
    uint8_t funct; // only R-type
    char type; // 'R', 'I', 'J'
};


std::unordered_map<std::string, uint32_t> asmBuildSymbolTable(const std::vector<AsmLine> &prog);

uint32_t asmEncodeR(uint8_t rs, uint8_t rt, uint8_t rd, uint8_t shamt, uint8_t funct);

uint32_t asmEncodeI(uint8_t op, uint8_t rs, uint8_t rt, int16_t imm);

uint32_t asmEncodeJ(uint8_t op, uint32_t target26);

std::vector<uint32_t> asmGenerateCode(const std::vector<AsmLine> &prog, const std::unordered_map<std::string, uint32_t> &sym);

#endif
