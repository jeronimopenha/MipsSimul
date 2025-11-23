#ifndef MIPSSIMUL_DISASSEMBLER_H
#define MIPSSIMUL_DISASSEMBLER_H

#include <../definitions.h>

struct RDesc {
    const std::string name;
    uint8_t funct;

    enum class Form { RdRsRt, RdRtShamt } form;
};

static const RDesc R_TABLE[] = {
    {"add", 0x20, RDesc::Form::RdRsRt},
    {"sub", 0x22, RDesc::Form::RdRsRt},
    {"and", 0x24, RDesc::Form::RdRsRt},
    {"or", 0x25, RDesc::Form::RdRsRt},
    {"slt", 0x2A, RDesc::Form::RdRsRt},
    {"sll", 0x00, RDesc::Form::RdRtShamt},
};

struct IDesc {
    const std::string name;
    uint8_t opcode;

    enum class Form { RtRsImm, RtMemRsImm, RsRtRel } form;

    // RtMemRsImm = lw/sw ; RsRtRel = beq/bne
};

static const IDesc I_TABLE[] = {
    {"addi", 0x08, IDesc::Form::RtRsImm},
    {"lw", 0x23, IDesc::Form::RtMemRsImm},
    {"sw", 0x2B, IDesc::Form::RtMemRsImm},
    {"beq", 0x04, IDesc::Form::RsRtRel},
    // {"bne", 0x05, IDesc::Form::RsRtRel},
};

struct JDesc {
    const std::string name;
    uint8_t opcode;
};

static const JDesc J_TABLE[] = {
    {"j", 0x02},
    {"jal", 0x03},
};

const RDesc *findR(uint8_t funct);

const IDesc *findI(uint8_t op);

const JDesc *findJ(uint8_t op);

std::string disassembleWord(uint32_t w, uint32_t pc, const std::map<uint32_t, std::string> *labels = nullptr);

#endif //MIPSSIMUL_DISASSEMBLER_H
