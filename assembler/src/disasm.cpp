#include <disasm.h>
#include "asm_registers.h"

using namespace std;

static string r(int n) {
    return regNameFromNumber(n);
}

static const AsmInstrDesc *findInstrByOpcodeFunct(const uint8_t opcode, const uint8_t funct) {
    for (const auto &d: ISA_TABLE) {
        if (d.type == AsmInstrType::R) {
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

static const AsmInstrDesc *findInstrByOpcode(const uint8_t opcode) {
    for (const auto &d: ISA_TABLE)
        if (d.opcode == opcode && d.type != AsmInstrType::R) {
            return &d;
        }

    return nullptr;
}

std::map<uint32_t, std::string> disasmMakeLabels(const std::vector<uint32_t> &code) {
    std::map<uint32_t, std::string> labels;
    uint32_t pc = baseAsmPc;
    int count = 0;

    for (const auto w: code) {
        const uint8_t op = (w >> 26) & 0x3F;
        //const uint8_t rs = (w >> 21) & 0x1F;
        //const uint8_t rt = (w >> 16) & 0x1F;
        //const uint8_t rd = (w >> 11) & 0x1F;
        //const uint8_t shamt = (w >> 6) & 0x1F;
        const uint8_t funct = (w >> 0) & 0x3F;
        const auto imm16 = static_cast<int16_t>(w & 0xFFFF);
        const uint32_t idx = w & 0x03FFFFFF;

        const AsmInstrDesc *d = nullptr;

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

        if (d->type == AsmInstrType::I && d->form == AsmInstrForm::RsRtRel) {
            // beq / bne style
            const uint32_t target = pc + 4 + (static_cast<int32_t>(imm16) << 2);
            if (!labels.count(target)) {
                labels[target] = "L" + std::to_string(count++);
            }
        } else if (d->type == AsmInstrType::J && d->form == AsmInstrForm::Jump) {
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
    const auto imm16 = static_cast<int16_t>(w & 0xFFFF);
    const uint32_t idx = w & 0x03FFFFFF;

    std::ostringstream out;

    const AsmInstrDesc *d = nullptr;

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
    if (d->type == AsmInstrType::R) {
        if (d->form == AsmInstrForm::RdRsRt) {
            // add/and/or/slt ...
            out << d->name << " " << r(rd) << ", " << r(rs) << ", " << r(rt);
        } else if (d->form == AsmInstrForm::RdRtShamt) {
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
    else if (d->type == AsmInstrType::I) {
        if (d->form == AsmInstrForm::RtRsImm) {
            // addi rt, rs, imm
            out << d->name << " " << r(rt) << ", " << r(rs) << ", " << static_cast<int>(imm16);
        } else if (d->form == AsmInstrForm::RtMem) {
            // lw/sw rt, imm(rs)
            out << d->name << " " << r(rt) << ", " << static_cast<int>(imm16) << "(" << r(rs) << ")";
        } else if (d->form == AsmInstrForm::RsRtRel) {
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
    else if (d->type == AsmInstrType::J) {
        if (d->form == AsmInstrForm::Jump) {
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
