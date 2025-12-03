#ifndef ASSEMBLER_ISA_H
#define ASSEMBLER_ISA_H

#include <definitions.h>

enum class AsmInstrType { R, I, J };

enum class AsmInstrForm {
    RdRsRt,
    RdRtShamt,
    RtRsImm,
    RtMem,
    RsRtRel,
    Jump
};

struct AsmInstrDesc {
    std::string name;
    AsmInstrType type;
    AsmInstrForm form;
    uint8_t opcode;
    uint8_t funct; // R-type only
};

const std::vector<AsmInstrDesc> ISA_TABLE = {
    {"add", AsmInstrType::R, AsmInstrForm::RdRsRt, 0x00, 0x20},
    {"and", AsmInstrType::R, AsmInstrForm::RdRsRt, 0x00, 0x24},
    {"or", AsmInstrType::R, AsmInstrForm::RdRsRt, 0x00, 0x25},
    {"slt", AsmInstrType::R, AsmInstrForm::RdRsRt, 0x00, 0x2A},
    {"sll", AsmInstrType::R, AsmInstrForm::RdRtShamt, 0x00, 0x00},

    {"addi", AsmInstrType::I, AsmInstrForm::RtRsImm, 0x08, 0x00},
    {"lw", AsmInstrType::I, AsmInstrForm::RtMem, 0x23, 0x00},
    {"sw", AsmInstrType::I, AsmInstrForm::RtMem, 0x2B, 0x00},
    {"beq", AsmInstrType::I, AsmInstrForm::RsRtRel, 0x04, 0x00},

    {"j", AsmInstrType::J, AsmInstrForm::Jump, 0x02, 0},
    {"jal", AsmInstrType::J, AsmInstrForm::Jump, 0x03, 0},
};

const AsmInstrDesc* asmFindInstrByName(const std::string& name);

#endif
