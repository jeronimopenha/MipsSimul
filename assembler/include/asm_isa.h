#ifndef ASSEMBLER_ISA_H
#define ASSEMBLER_ISA_H

#include <definitions.h>

enum class InstrType { R, I, J };

enum class InstrForm {
    RdRsRt,
    RdRtShamt,
    RtRsImm,
    RtMem,
    RsRtRel,
    Jump
};

struct InstrDesc {
    std::string name;
    InstrType type;
    InstrForm form;
    uint8_t opcode;
    uint8_t funct; // R-type only
};

const std::vector<InstrDesc> ISA_TABLE = {
    {"add", InstrType::R, InstrForm::RdRsRt, 0x00, 0x20},
    {"and", InstrType::R, InstrForm::RdRsRt, 0x00, 0x24},
    {"or", InstrType::R, InstrForm::RdRsRt, 0x00, 0x25},
    {"slt", InstrType::R, InstrForm::RdRsRt, 0x00, 0x2A},
    {"sll", InstrType::R, InstrForm::RdRtShamt, 0x00, 0x00},

    {"addi", InstrType::I, InstrForm::RtRsImm, 0x08, 0x00},
    {"lw", InstrType::I, InstrForm::RtMem, 0x23, 0x00},
    {"sw", InstrType::I, InstrForm::RtMem, 0x2B, 0x00},
    {"beq", InstrType::I, InstrForm::RsRtRel, 0x04, 0x00},

    {"j", InstrType::J, InstrForm::Jump, 0x02, 0},
    {"jal", InstrType::J, InstrForm::Jump, 0x03, 0},
};

const InstrDesc* findInstrByName(const std::string& name);

#endif
