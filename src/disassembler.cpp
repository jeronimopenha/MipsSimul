#include <../../include/disassembler.h>

using namespace std;

const RDesc *findR(const uint8_t funct) {
    for (const auto &d: R_TABLE) {
        if (d.funct == funct) return &d;
    }
    return nullptr;
}

const IDesc *findI(const uint8_t op) {
    for (const auto &d: I_TABLE) {
        if (d.opcode == op) return &d;
    }
    return nullptr;
}

const JDesc *findJ(const uint8_t op) {
    for (const auto &d: J_TABLE) {
        if (d.opcode == op) return &d;
    }
    return nullptr;
}

std::string disassembleWord(const uint32_t w, const uint32_t pc, const std::map<uint32_t, std::string> *labels) {
    uint8_t op = (w >> 26) & 0x3F;
    uint8_t rs = (w >> 21) & 0x1F;
    uint8_t rt = (w >> 16) & 0x1F;
    uint8_t rd = (w >> 11) & 0x1F;
    uint8_t shamt = (w >> 6) & 0x1F;
    uint8_t funct = (w >> 0) & 0x3F;
    int16_t imm16 = (int16_t) (w & 0xFFFF);
    uint32_t idx = w & 0x03FFFFFF;

    std::ostringstream out;

    if (op == 0x00) {
        // R-type
        const RDesc *d = findR(funct);
        if (!d) {
            out << ".word ...";
            return out.str();
        }
        if (funct == 0x00 && rs == 0 && rt == 0 && rd == 0 && shamt == 0) {
            out << "sll $zero, $zero, 0"; // nop
        } else if (d->form == RDesc::Form::RdRsRt) {
            out << d->name << " " << regName(rd)
                    << ", " << regName(rs)
                    << ", " << regName(rt);
        } else {
            // RdRtShamt (sll)
            out << d->name << " " << regName(rd)
                    << ", " << regName(rt)
                    << ", " << (int) shamt;
        }
    } else if (auto j = findJ(op)) {
        // J-type
        uint32_t target = (pc & 0xF0000000u) | (idx << 2);
        out << j->name << " ";
        if (labels && labels->count(target)) {
            out << labels->at(target);
        } else {
            out << "0x" << std::hex << std::uppercase
                    << std::setw(8) << std::setfill('0') << target;
        }
    } else if (auto i = findI(op)) {
        // I-type
        if (i->Form == IDesc::Form::RtRsImm) {
            out << i->name << " " << regName(rt) << ", " << regName(rs)
                    << ", " << (int) imm16;
        } else if (i->Form == IDesc::Form::RtMemRsImm) {
            out << i->name << " " << regName(rt) << ", " << (int) imm16
                    << "(" << regName(rs) << ")";
        } else {
            // RsRtRel
            uint32_t target = pc + 4 + ((int32_t) imm16 << 2);
            out << i->name << " " << regName(rs) << ", " << regName(rt) << ", ";
            if (labels && labels->count(target)) {
                out << labels->at(target);
            } else {
                out << "0x" << std::hex << std::uppercase
                        << std::setw(8) << std::setfill('0') << target;
            }
        }
    } else {
        out << ".word 0x" << std::hex << std::uppercase
                << std::setw(8) << std::setfill('0') << w;
    }

    return out.str();
}
