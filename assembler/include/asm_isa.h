#ifndef ASSEMBLER_ISA_H
#define ASSEMBLER_ISA_H

#include <definitions.h>

enum class AsmInstrType {
    R,
    I,
    J,
    P
};

enum AsmInstrForm {
    RdRsRt,
    RdRtShamt,
    RtRsImm,
    RtMem,
    RsRtRel,
    RtImm,
    Rs,
    Jump,
    None,
    RdRs
};

struct AsmInstrDesc {
    std::string name;
    AsmInstrType type;
    AsmInstrForm form;
    uint8_t opcode;
    uint8_t funct; // R-type only
};

const std::vector<AsmInstrDesc> ISA_TABLE = {
    //Arithmetic / logic
    {"add", AsmInstrType::R, AsmInstrForm::RdRsRt, 0x00, 0x20},
    {"sub", AsmInstrType::R, AsmInstrForm::RdRsRt, 0x00, 0x22},
    {"and", AsmInstrType::R, AsmInstrForm::RdRsRt, 0x00, 0x24},
    {"or", AsmInstrType::R, AsmInstrForm::RdRsRt, 0x00, 0x25},
    {"xor", AsmInstrType::R, AsmInstrForm::RdRsRt, 0x00, 0x26},
    {"nor", AsmInstrType::R, AsmInstrForm::RdRsRt, 0x00, 0x27},

    //Comparison
    {"slt", AsmInstrType::R, AsmInstrForm::RdRsRt, 0x00, 0x2A},
    {"sltu", AsmInstrType::R, AsmInstrForm::RdRsRt, 0x00, 0x2B},

    //Shifts
    {"sll", AsmInstrType::R, AsmInstrForm::RdRtShamt, 0x00, 0x00},
    {"srl", AsmInstrType::R, AsmInstrForm::RdRtShamt, 0x00, 0x02},
    {"sra", AsmInstrType::R, AsmInstrForm::RdRtShamt, 0x00, 0x03},

    //Immediate
    {"addi", AsmInstrType::I, AsmInstrForm::RtRsImm, 0x08, 0x00},
    {"andi", AsmInstrType::I, AsmInstrForm::RtRsImm, 0x0c, 0x00},
    {"ori", AsmInstrType::I, AsmInstrForm::RtRsImm, 0x0d, 0x00},
    {"xori", AsmInstrType::I, AsmInstrForm::RtRsImm, 0x0e, 0x00},
    {"lui", AsmInstrType::I, AsmInstrForm::RtImm, 0x0f, 0x00},

    //Memory
    {"lw", AsmInstrType::I, AsmInstrForm::RtMem, 0x23, 0x00},
    {"sw", AsmInstrType::I, AsmInstrForm::RtMem, 0x2B, 0x00},
    {"lb", AsmInstrType::I, AsmInstrForm::RtMem, 0x20, 0x00},
    {"lbu", AsmInstrType::I, AsmInstrForm::RtMem, 0x24, 0x00},
    {"sb", AsmInstrType::I, AsmInstrForm::RtMem, 0x28, 0x00},

    //Branches and junps
    {"beq", AsmInstrType::I, AsmInstrForm::RsRtRel, 0x04, 0x00},
    {"bne", AsmInstrType::I, AsmInstrForm::RsRtRel, 0x05, 0x00},
    {"j", AsmInstrType::J, AsmInstrForm::Jump, 0x02, 0x00},
    {"jal", AsmInstrType::J, AsmInstrForm::Jump, 0x03, 0x00},
    {"jr", AsmInstrType::I, AsmInstrForm::Rs, 0x00, 0x08},

    //Pseudo
    {"move", AsmInstrType::P, AsmInstrForm::RdRs, 0x00, 0x00},
    {"nop", AsmInstrType::P, AsmInstrForm::None, 0x00, 0x00},
};

const AsmInstrDesc *asmFindInstrByName(const std::string &name);

#endif
