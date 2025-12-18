#ifndef ASSEMBLER_ISA_H
#define ASSEMBLER_ISA_H

#include <definitions.h>

enum AsmInstrType {
    R,
    I,
    J,
    P
};

enum AsmInstrForm {
    //R type
    RdRsRt,
    RdRtShamt,
    Rs,

    //I type
    RtRsImm,
    RtMem,
    RsRtRel,
    RtImm,

    //J type
    Jump,

    //Pseudo
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
    {"add", R, RdRsRt, 0x00, 0x20},
    {"sub", R, RdRsRt, 0x00, 0x22},
    {"and", R, RdRsRt, 0x00, 0x24},
    {"or", R, RdRsRt, 0x00, 0x25},
    {"xor", R, RdRsRt, 0x00, 0x26},
    {"nor", R, RdRsRt, 0x00, 0x27},

    //Comparison
    {"slt", R, RdRsRt, 0x00, 0x2A},
    {"sltu", R, RdRsRt, 0x00, 0x2B},

    //Shifts
    {"sll", R, RdRtShamt, 0x00, 0x00},
    {"srl", R, RdRtShamt, 0x00, 0x02},
    {"sra", R, RdRtShamt, 0x00, 0x03},

    //Immediate
    {"addi", I, RtRsImm, 0x08, 0x00},
    {"andi", I, RtRsImm, 0x0c, 0x00},
    {"ori", I, RtRsImm, 0x0d, 0x00},
    {"xori", I, RtRsImm, 0x0e, 0x00},
    {"lui", I, RtImm, 0x0f, 0x00},

    //Memory
    {"lw", I, RtMem, 0x23, 0x00},
    {"sw", I, RtMem, 0x2B, 0x00},
    {"lb", I, RtMem, 0x20, 0x00},
    {"lbu", I, RtMem, 0x24, 0x00},
    {"sb", I, RtMem, 0x28, 0x00},

    //Branches and jumps
    {"beq", I, RsRtRel, 0x04, 0x00},
    {"bne", I, RsRtRel, 0x05, 0x00},
    {"j", J, Jump, 0x02, 0x00},
    {"jal", J, Jump, 0x03, 0x00},
    {"jr", R, Rs, 0x00, 0x08},

    //Pseudo
    {"move", P, RdRs, 0x00, 0x00},
    {"nop", P, None, 0x00, 0x00},
};

const AsmInstrDesc *asmFindInstrByName(const std::string &name);

#endif
