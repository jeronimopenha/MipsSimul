#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include <definitions.h>
//#include <asm_registers.h>
#include <asm_isa.h>

// Generate labels L0, L1, ... for all targets of beq/j/jal/bne
// code  = instruction vector in machine language
// basePc = initial address (ex: 0x00400000)
std::map<uint32_t, std::string> disasmMakeLabels(const std::vector<uint32_t> &code);

// disassemble an instruction
// w      = 32 bits word
// pc     = actual address of the instruction
// labels = optional;
std::string disassembleWord(uint32_t w, uint32_t pc, const std::map<uint32_t, std::string> *labels = nullptr);

static std::string r(int n);

static const AsmInstrDesc *findInstrByOpcodeFunct(uint8_t opcode, uint8_t funct);

static const AsmInstrDesc *findInstrByOpcode(uint8_t opcode);

std::map<uint32_t, std::string> disasmMakeLabels(const std::vector<uint32_t> &code, uint32_t basePc);

#endif
