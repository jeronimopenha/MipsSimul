#include <asm.h>

using namespace std;


uint32_t asmEncodeR(const uint8_t rs, const uint8_t rt, const uint8_t rd, const uint8_t shamt, const uint8_t funct) {
    return (0u << 26) |
           (rs << 21) |
           (rt << 16) |
           (rd << 11) |
           (shamt << 6) |
           (funct);
}

uint32_t asmEncodeI(const uint8_t op, const uint8_t rs, const uint8_t rt, const int16_t imm) {
    return (op << 26) |
           (rs << 21) |
           (rt << 16) |
           static_cast<uint16_t>(imm);
}

uint32_t asmEncodeJ(const uint8_t op, const uint32_t target26) {
    return (op << 26) | (target26 & 0x03FFFFFFu);
}

vector<uint32_t> asmGenerateCode(const vector<AsmLine> &prog, const unordered_map<string, uint32_t> &sym) {
    vector<uint32_t> machine;
    machine.reserve(prog.size());

    uint32_t pc = baseAsmPc;

    for (const auto &line: prog) {
        if (!line.hasInstr) {
            continue; // only label
        }

        const MiniCInstruction &inst = line.instr;

        //pseudo instructions first
        if (inst.op == "nop") {
            // nop => add $zero,$zero,$zero
            uint32_t w = asmEncodeR(
                /*rs*/ 0,
                       /*rt*/ 0,
                       /*rd*/ 0,
                       /*shamt*/ 0,
                       /*funct*/ 0x20 // funct do ADD
            );
            machine.push_back(w);
            pc += 4;
            continue;
        }

        if (inst.op == "move") {
            if (inst.args.size() != 2) {
                throw runtime_error("move needs 2 operands");
            }

            const AsmOperand &dst = inst.args[0];
            const AsmOperand &src = inst.args[1];

            if (dst.kind != AsmOperand::Kind::REG ||
                src.kind != AsmOperand::Kind::REG) {
                throw runtime_error("move: both operands must be registers");
            }

            const int rd = regNumber(dst.label);
            const int rs = regNumber(src.label);
            constexpr int rt = 0; // $zero

            uint32_t w = asmEncodeR(
                /*rs*/ rs,
                       /*rt*/ rt,
                       /*rd*/ rd,
                       /*shamt*/ 0,
                       /*funct*/ 0x20 // add
            );
            machine.push_back(w);
            pc += 4;
            continue;
        }

        //now, other instructions
        const AsmInstrDesc *desc = asmFindInstrByName(inst.op);
        if (!desc) {
            throw runtime_error("Opcode not supported: " + inst.op);
        }

        const uint8_t opcode = desc->opcode;
        const uint8_t funct = desc->funct;
        const AsmInstrType type = desc->type;
        const AsmInstrForm form = desc->form;

        uint32_t w = 0;

        // ---------------- R-TYPE ----------------
        if (type == R) {
            if (form == RdRsRt) {
                // Ex.: add rd, rs, rt ; and, or, slt
                if (inst.args.size() != 3) {
                    throw runtime_error(desc->name + " needs 3 operands");
                }

                const int rd = regNumber(inst.args[0].label);
                const int rs = regNumber(inst.args[1].label);
                const int rt = regNumber(inst.args[2].label);

                w = asmEncodeR(rs, rt, rd, 0, funct);
            } else if (form == RdRtShamt) {
                // Ex.: sll rd, rt, shamt
                if (inst.args.size() != 3) {
                    throw runtime_error(desc->name + " needs 3 operands");
                }

                const int rd = regNumber(inst.args[0].label);
                const int rt = regNumber(inst.args[1].label);

                const AsmOperand &shOp = inst.args[2];
                if (shOp.kind != AsmOperand::Kind::IMM) {
                    throw runtime_error(desc->name + ": shamt must be immediate");
                }

                const int shamt = shOp.imm;

                w = asmEncodeR(0 /*rs*/, rt, rd, shamt, funct);
            } else if (form == Rs) {
                //jr
                if (inst.args.size() != 1) {
                    throw runtime_error(desc->name + " needs 1 operands");
                }

                const AsmOperand &opRs = inst.args[0];
                if (opRs.kind != AsmOperand::Kind::REG) {
                    throw runtime_error(desc->name + ": operand must be register");
                }

                const int rs = regNumber(opRs.label);
                constexpr int rt = 0;
                constexpr int rd = 0;
                constexpr int shamt = 0;

                w = asmEncodeR(rs, rt, rd, shamt, funct);
            } else {
                throw runtime_error("Unknown R-form for " + desc->name);
            }
        }

        // ---------------- I-TYPE ----------------
        else if (type == I) {
            if (form == RtRsImm) {
                // Ex.: addi rt, rs, imm
                if (inst.args.size() != 3) {
                    throw runtime_error(desc->name + " needs 3 operands");
                }

                const int rt = regNumber(inst.args[0].label);
                const int rs = regNumber(inst.args[1].label);

                const AsmOperand &immOp = inst.args[2];
                if (immOp.kind != AsmOperand::Kind::IMM) {
                    throw runtime_error(desc->name + ": third operand must be immediate");
                }

                const int16_t imm = static_cast<int16_t>(immOp.imm);

                w = asmEncodeI(opcode, rs, rt, imm);
            } else if (form == RtMem) {
                // Ex.: lw/sw rt, imm(rs)
                if (inst.args.size() != 2) {
                    throw runtime_error(desc->name + " needs 2 operands");
                }

                const int rt = regNumber(inst.args[0].label);
                const AsmOperand &mem = inst.args[1];

                if (mem.kind != AsmOperand::Kind::MEM) {
                    throw runtime_error(desc->name + ": second operand must be Mem");
                }

                const int rs = regNumber(mem.label); // base: "$gp", "$sp", etc.
                const int16_t imm = static_cast<int16_t>(mem.imm);

                w = asmEncodeI(opcode, rs, rt, imm);
            } else if (form == RsRtRel) {
                // Ex.: beq rs, rt, label
                if (inst.args.size() != 3) {
                    throw runtime_error(desc->name + " needs 3 operands");
                }

                const int rs = regNumber(inst.args[0].label);
                const int rt = regNumber(inst.args[1].label);
                const AsmOperand &lab = inst.args[2];

                if (lab.kind != AsmOperand::Kind::LABELREF) {
                    throw runtime_error(desc->name + ": third operand must be Label");
                }

                auto itL = sym.find(lab.label);
                if (itL == sym.end()) {
                    throw runtime_error("Branch label not found: " + lab.label);
                }

                const uint32_t target = itL->second;
                const int32_t diff = static_cast<int32_t>(target) - static_cast<int32_t>(pc + 4);
                const int16_t offset = static_cast<int16_t>(diff / 4);

                w = asmEncodeI(opcode, rs, rt, offset);
            } else if (form == RtImm) {
                //lui
            } else {
                throw runtime_error("Unknown I-form for " + desc->name);
            }
        }

        // ---------------- J-TYPE ----------------
        else if (type == J) {
            if (form != Jump) {
                throw runtime_error("Unknown J-form for " + desc->name);
            }

            if (inst.args.size() != 1) {
                throw runtime_error(desc->name + " needs 1 operand");
            }

            const AsmOperand &a0 = inst.args[0];
            if (a0.kind != AsmOperand::Kind::LABELREF) {
                throw runtime_error(desc->name + ": operand must be Label");
            }

            auto itL = sym.find(a0.label);
            if (itL == sym.end()) {
                throw runtime_error("Jump label not found: " + a0.label);
            }

            const uint32_t addr = itL->second;
            const uint32_t target26 = (addr >> 2) & 0x03FFFFFFu;

            w = asmEncodeJ(opcode, target26);
        }

        machine.push_back(w);
        pc += 4;
    }

    return machine;
}

/*
 Step 1 – Build the simbol table (labels → address)

Assuming:

Instructions starts with base_pc = 0x00400000;
Each instruction ocupies 4 bytes;
Every line has hasInstr == true is instruction.
*/

//Look for every label changing it for a valid address
unordered_map<string, uint32_t> asmBuildSymbolTable(const vector<AsmLine> &prog) {
    unordered_map<string, uint32_t> sym;
    uint32_t pc = baseAsmPc;

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
