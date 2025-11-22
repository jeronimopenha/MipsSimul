#ifndef MIPSSIMUL_ASSEMBLER_H
#define MIPSSIMUL_ASSEMBLER_H

#include <definitions.h>
#include <parser.h>

inline uint32_t base_pc = 0x00400000;

inline std::unordered_map<std::string, int> regMap = {
    {"$zero", 0}, {"$at", 1},
    {"$v0", 2}, {"$v1", 3},
    {"$a0", 4}, {"$a1", 5}, {"$a2", 6}, {"$a3", 7},
    {"$t0", 8}, {"$t1", 9}, {"$t2", 10}, {"$t3", 11},
    {"$t4", 12}, {"$t5", 13}, {"$t6", 14}, {"$t7", 15},
    {"$s0", 16}, {"$s1", 17}, {"$s2", 18}, {"$s3", 19},
    {"$s4", 20}, {"$s5", 21}, {"$s6", 22}, {"$s7", 23},
    {"$t8", 24}, {"$t9", 25}, {"$k0", 26}, {"$k1", 27},
    {"$gp", 28}, {"$sp", 29}, {"$fp", 30}, {"$ra", 31}
};

struct InstrInfo {
    uint8_t opcode;
    uint8_t funct; // só R-type usa
    char type; // 'R', 'I', 'J'
};

inline std::unordered_map<std::string, InstrInfo> instrMap = {
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


std::unordered_map<std::string, uint32_t> buildSymbolTable(const std::vector<Line> &prog);

int regNumber(const std::string &name);

uint32_t encodeR(uint8_t rs, uint8_t rt, uint8_t rd, uint8_t shamt, uint8_t funct);

uint32_t encodeI(uint8_t op, uint8_t rs, uint8_t rt, int16_t imm);

uint32_t encodeJ(uint8_t op, uint32_t target26);

std::vector<uint32_t> generateCode(const std::vector<Line> &prog, const std::unordered_map<std::string, uint32_t> &sym);

#endif //MIPSSIMUL_ASSEMBLER_H
