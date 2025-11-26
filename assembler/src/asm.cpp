#include <asm.h>

using namespace std;

/*
 Step 1 – Build the simbol table (labels → address)

Assuming:

Instructions starts with base_pc = 0x00400000;
Each instruction ocupies 4 bytes;
Every line has hasInstr == true is instruction.
*/

//Look for every label changing it for a valid address
unordered_map<string, uint32_t> buildSymbolTable(const vector<Line> &prog) {
    unordered_map<string, uint32_t> sym;
    uint32_t pc = basePc;

    for (const auto &line: prog) {
        if (!line.label.empty()) {
            if (sym.count(line.label)) {
                throw runtime_error("Repeated label: " + line.label);
            }
            sym[line.label] = pc;
        }

        if (line.hasInstr) {
            pc += 4; // next instruction
        }
    }

#ifdef DEBUG
    for (auto &[name, addr]: sym) {
        cout << name << " -> 0x" << hex << addr << "\n";
    }
#endif

    return sym;
}


uint32_t encodeR(const uint8_t rs, const uint8_t rt, const uint8_t rd, const uint8_t shamt, const uint8_t funct) {
    return (0u << 26) |
           (rs << 21) |
           (rt << 16) |
           (rd << 11) |
           (shamt << 6) |
           (funct);
}

uint32_t encodeI(const uint8_t op, const uint8_t rs, const uint8_t rt, const int16_t imm) {
    return (op << 26) |
           (rs << 21) |
           (rt << 16) |
           static_cast<uint16_t>(imm);
}

uint32_t encodeJ(const uint8_t op, const uint32_t target26) {
    return (op << 26) | (target26 & 0x03FFFFFFu);
}

vector<uint32_t> generateCode(const vector<Line> &prog,
                              const unordered_map<string, uint32_t> &sym) {
    vector<uint32_t> machine;
    machine.reserve(prog.size());

    uint32_t pc = basePc;

    for (const auto &line: prog) {
        if (!line.hasInstr) {
            continue; // only label
        }


        const Instruction &inst = line.instr;
        const InstrDesc *desc = findInstrByName(inst.op);
        if (!desc) {
            throw runtime_error("Opcode not supported: " + inst.op);
        }

        const uint8_t opcode = desc->opcode;
        const uint8_t funct = desc->funct;
        const InstrType type = desc->type;
        const InstrForm form = desc->form;

        uint32_t w = 0;

        // ---------------- R-TYPE ----------------
        if (type == InstrType::R) {
            if (form == InstrForm::RdRsRt) {
                // Ex.: add rd, rs, rt ; and, or, slt
                if (inst.args.size() != 3) {
                    throw runtime_error(desc->name + " needs 3 operands");
                }

                const int rd = regNumber(inst.args[0].label);
                const int rs = regNumber(inst.args[1].label);
                const int rt = regNumber(inst.args[2].label);

                w = encodeR(rs, rt, rd, 0, funct);
            } else if (form == InstrForm::RdRtShamt) {
                // Ex.: sll rd, rt, shamt
                if (inst.args.size() != 3) {
                    throw runtime_error(desc->name + " needs 3 operands");
                }

                const int rd = regNumber(inst.args[0].label);
                const int rt = regNumber(inst.args[1].label);

                const Operand &shOp = inst.args[2];
                if (shOp.kind != Operand::Kind::Imm) {
                    throw runtime_error(desc->name + ": shamt must be immediate");
                }

                const int shamt = shOp.imm;

                w = encodeR(0 /*rs*/, rt, rd, shamt, funct);
            } else {
                throw runtime_error("Unknown R-form for " + desc->name);
            }
        }

        // ---------------- I-TYPE ----------------
        else if (type == InstrType::I) {
            if (form == InstrForm::RtRsImm) {
                // Ex.: addi rt, rs, imm
                if (inst.args.size() != 3) {
                    throw runtime_error(desc->name + " needs 3 operands");
                }

                const int rt = regNumber(inst.args[0].label);
                const int rs = regNumber(inst.args[1].label);

                const Operand &immOp = inst.args[2];
                if (immOp.kind != Operand::Kind::Imm) {
                    throw runtime_error(desc->name + ": third operand must be immediate");
                }

                const int16_t imm = static_cast<int16_t>(immOp.imm);

                w = encodeI(opcode, rs, rt, imm);
            } else if (form == InstrForm::RtMem) {
                // Ex.: lw/sw rt, imm(rs)
                if (inst.args.size() != 2) {
                    throw runtime_error(desc->name + " needs 2 operands");
                }

                const int rt = regNumber(inst.args[0].label);
                const Operand &mem = inst.args[1];

                if (mem.kind != Operand::Kind::Mem) {
                    throw runtime_error(desc->name + ": second operand must be Mem");
                }

                const int rs = regNumber(mem.label); // base: "$gp", "$sp", etc.
                const int16_t imm = static_cast<int16_t>(mem.imm);

                w = encodeI(opcode, rs, rt, imm);
            } else if (form == InstrForm::RsRtRel) {
                // Ex.: beq rs, rt, label
                if (inst.args.size() != 3) {
                    throw runtime_error(desc->name + " needs 3 operands");
                }

                const int rs = regNumber(inst.args[0].label);
                const int rt = regNumber(inst.args[1].label);
                const Operand &lab = inst.args[2];

                if (lab.kind != Operand::Kind::LabelRef) {
                    throw runtime_error(desc->name + ": third operand must be Label");
                }

                auto itL = sym.find(lab.label);
                if (itL == sym.end()) {
                    throw runtime_error("Branch label not found: " + lab.label);
                }

                const uint32_t target = itL->second;
                const int32_t diff = static_cast<int32_t>(target) - static_cast<int32_t>(pc + 4);
                const int16_t offset = static_cast<int16_t>(diff / 4);

                w = encodeI(opcode, rs, rt, offset);
            } else {
                throw runtime_error("Unknown I-form for " + desc->name);
            }
        }

        // ---------------- J-TYPE ----------------
        else if (type == InstrType::J) {
            if (form != InstrForm::Jump) {
                throw runtime_error("Unknown J-form for " + desc->name);
            }

            if (inst.args.size() != 1) {
                throw runtime_error(desc->name + " needs 1 operand");
            }

            const Operand &a0 = inst.args[0];
            if (a0.kind != Operand::Kind::LabelRef) {
                throw runtime_error(desc->name + ": operand must be Label");
            }

            auto itL = sym.find(a0.label);
            if (itL == sym.end()) {
                throw runtime_error("Jump label not found: " + a0.label);
            }

            const uint32_t addr = itL->second;
            const uint32_t target26 = (addr >> 2) & 0x03FFFFFFu;

            w = encodeJ(opcode, target26);
        }

        machine.push_back(w);
        pc += 4;
    }

    return machine;
}
