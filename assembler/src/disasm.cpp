#include <disasm.h>

using namespace std;

static const string r(int n) {
    return regNameFromNumber(n);
}

static const InstrDesc *findInstrByOpcodeFunct(const uint8_t opcode, const uint8_t funct) {
    for (const auto &d: ISA_TABLE) {
        if (d.type == InstrType::R) {
            if (d.opcode == opcode && d.funct == funct) {
                return &d;
            }
        } else {
            if (d.opcode == opcode) {
                return &d;
            }
        }
    }
    return nullptr;
}

static const InstrDesc *findInstrByOpcode(const uint8_t opcode) {
    for (const auto &d: ISA_TABLE)
        if (d.opcode == opcode && d.type != InstrType::R) {
            return &d;
        }

    return nullptr;
}

std::map<uint32_t, std::string> makeLabels(const std::vector<uint32_t> &code) {
    std::map<uint32_t, std::string> labels;
    uint32_t pc = basePc;
    int count = 0;

    for (const auto w: code) {
        const uint8_t op = (w >> 26) & 0x3F;
        const uint8_t rs = (w >> 21) & 0x1F;
        const uint8_t rt = (w >> 16) & 0x1F;
        const uint8_t rd = (w >> 11) & 0x1F;
        const uint8_t shamt = (w >> 6) & 0x1F;
        const uint8_t funct = (w >> 0) & 0x3F;
        const int16_t imm16 = static_cast<int16_t>(w & 0xFFFF);
        const uint32_t idx = w & 0x03FFFFFF;

        const InstrDesc *d = nullptr;

        if (op == 0x00) {
            // R-type (It usually doesn't have a target label, except for jalr/jr, that is not added yet.)
            d = findInstrByOpcodeFunct(op, funct);
        } else {
            d = findInstrByOpcode(op);
        }


        if (!d) {
            pc += 4;
            continue;
        }

        if (d->type == InstrType::I && d->form == InstrForm::RsRtRel) {
            // beq / bne style
            const uint32_t target = pc + 4 + (static_cast<int32_t>(imm16) << 2);
            if (!labels.count(target)) {
                labels[target] = "L" + std::to_string(count++);
            }
        } else if (d->type == InstrType::J && d->form == InstrForm::Jump) {
            // j / jal
            const uint32_t target = (pc & 0xF0000000u) | (idx << 2);
            if (!labels.count(target)) {
                labels[target] = "L" + std::to_string(count++);
            }
        }

        pc += 4;
    }

    return labels;
}

std::string disassembleWord(const uint32_t w, const uint32_t pc, const std::map<uint32_t, std::string> *labels) {
    const uint8_t op = (w >> 26) & 0x3F;
    const uint8_t rs = (w >> 21) & 0x1F;
    const uint8_t rt = (w >> 16) & 0x1F;
    const uint8_t rd = (w >> 11) & 0x1F;
    const uint8_t shamt = (w >> 6) & 0x1F;
    const uint8_t funct = (w >> 0) & 0x3F;
    const int16_t imm16 = static_cast<int16_t>(w & 0xFFFF);
    const uint32_t idx = w & 0x03FFFFFF;

    std::ostringstream out;

    const InstrDesc *d = nullptr;

    if (op == 0x00) {
        d = findInstrByOpcodeFunct(op, funct);
    } else {
        d = findInstrByOpcode(op);
    }

    if (!d) {
        // Unknown instruction
        out << ".word 0x" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << w;
        return out.str();
    }

    // ---------------- R-TYPE ----------------
    if (d->type == InstrType::R) {
        if (d->form == InstrForm::RdRsRt) {
            // add/and/or/slt ...
            out << d->name << " " << r(rd) << ", " << r(rs) << ", " << r(rt);
        } else if (d->form == InstrForm::RdRtShamt) {
            // sll rd, rt, shamt (include nop)
            if (rs == 0 && rt == 0 && rd == 0 && shamt == 0) {
                // nop
                out << "sll $zero, $zero, 0";
            } else {
                out << d->name << " " << r(rd) << ", " << r(rt) << ", " << static_cast<int>(shamt);
            }
        } else {
            out << ".word 0x" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << w;
        }
    }

    // ---------------- I-TYPE ----------------
    else if (d->type == InstrType::I) {
        if (d->form == InstrForm::RtRsImm) {
            // addi rt, rs, imm
            out << d->name << " " << r(rt) << ", " << r(rs) << ", " << static_cast<int>(imm16);
        } else if (d->form == InstrForm::RtMem) {
            // lw/sw rt, imm(rs)
            out << d->name << " " << r(rt) << ", " << static_cast<int>(imm16) << "(" << r(rs) << ")";
        } else if (d->form == InstrForm::RsRtRel) {
            // beq/bne rs, rt, label/addr
            const uint32_t target = pc + 4 + (static_cast<int32_t>(imm16) << 2);
            out << d->name << " " << r(rs) << ", " << r(rt) << ", ";
            if (labels && labels->count(target)) {
                out << labels->at(target);
            } else {
                out << "0x" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << target;
            }
        } else {
            out << ".word 0x" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << w;
        }
    }

    // ---------------- J-TYPE ----------------
    else if (d->type == InstrType::J) {
        if (d->form == InstrForm::Jump) {
            uint32_t target = (pc & 0xF0000000u) | (idx << 2);
            out << d->name << " ";
            if (labels && labels->count(target)) {
                out << labels->at(target);
            } else {
                out << "0x" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << target;
            }
        } else {
            out << ".word 0x" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << w;
        }
    }

    return out.str();
}
