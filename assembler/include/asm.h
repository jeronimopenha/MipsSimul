#ifndef ASSEMBLER_ASSEMBLER_H
#define ASSEMBLER_ASSEMBLER_H

#include <definitions.h>
#include <asm_parser.h>
#include <asm_registers.h>
#include <asm_isa.h>

struct AsmInstrInfo {
    uint8_t opcode;
    uint8_t funct; // only R-type
    char type; // 'R', 'I', 'J'
};

inline std::unordered_map<std::string, AsmInstrInfo> AsminstrMap = {
    {"add", {0x00, 0x20, 'R'}},
    {"and", {0x00, 0x24, 'R'}},
    {"or", {0x00, 0x25, 'R'}},
    {"slt", {0x00, 0x2A, 'R'}},
    {"sll", {0x00, 0x00, 'R'}}, // sll tem funct=0

    {"addi", {0x08, 0x00, 'I'}},
    {"lw", {0x23, 0x00, 'I'}},
    {"sw", {0x2B, 0x00, 'I'}},
    {"beq", {0x04, 0x00, 'I'}},
    // (se quiser, depois adiciona "bne")

    {"j", {0x02, 0x00, 'J'}},
    {"jal", {0x03, 0x00, 'J'}},
};

std::unordered_map<std::string, uint32_t> asmBuildSymbolTable(const std::vector<AsmLine> &prog);

uint32_t asmEncodeR(uint8_t rs, uint8_t rt, uint8_t rd, uint8_t shamt, uint8_t funct);

uint32_t asmEncodeI(uint8_t op, uint8_t rs, uint8_t rt, int16_t imm);

uint32_t asmEncodeJ(uint8_t op, uint32_t target26);

std::vector<uint32_t> asmGenerateCode(const std::vector<AsmLine> &prog, const std::unordered_map<std::string, uint32_t> &sym);

#endif
