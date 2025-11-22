#include <assembler.h>

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
    uint32_t pc = base_pc;

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

int regNumber(const string &name) {
    const auto it = regMap.find(name);
    if (it == regMap.end()) {
        throw runtime_error("Unknowing register: " + name);
    }
    return it->second;
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

vector<uint32_t> generateCode(const vector<Line> &prog, const unordered_map<string, uint32_t> &sym) {
    vector<uint32_t> machine;
    machine.reserve(prog.size());

    uint32_t pc = base_pc;

    for (const auto &line: prog) {
        if (!line.hasInstr) {
            // linha só com label / vazia → não gera word
            continue;
        }

        const Instruction &inst = line.instr;
        auto it = instrMap.find(inst.op);
        if (it == instrMap.end()) {
            throw runtime_error("Opcode não suportado: " + inst.op);
        }
        auto [opcode, funct, type] = it->second;

        uint32_t w = 0;

        if (type == 'R') {
            // R-type: add, and, or, slt, sll
            if (inst.op == "sll") {
                // sll rd, rt, shamt
                if (inst.args.size() != 3)
                    throw runtime_error("sll needs 3 operands");
                const int rd = regNumber(inst.args[0].label);
                const int rt = regNumber(inst.args[1].label);
                const int sh = inst.args[2].imm;
                w = encodeR(0 /*rs*/, rt, rd, sh, funct);
            } else {
                // add/and/or/slt: op rd, rs, rt
                if (inst.args.size() != 3)
                    throw runtime_error(inst.op + " needs 3 operands");
                const int rd = regNumber(inst.args[0].label);
                const int rs = regNumber(inst.args[1].label);
                const int rt = regNumber(inst.args[2].label);
                w = encodeR(rs, rt, rd, 0, funct);
            }
        } else if (type == 'I') {
            // I-type: addi, lw, sw, beq
            if (inst.op == "addi") {
                // addi rt, rs, imm
                if (inst.args.size() != 3)
                    throw runtime_error("addi needs 3 operands");
                const int rt = regNumber(inst.args[0].label);
                const int rs = regNumber(inst.args[1].label);
                int16_t imm = static_cast<int16_t>(inst.args[2].imm);
                w = encodeI(opcode, rs, rt, imm);
            } else if (inst.op == "lw" || inst.op == "sw") {
                // lw/sw rt, imm(base)
                if (inst.args.size() != 2)
                    throw runtime_error("lw/sw needs 2 operands");
                // args[0] = Reg, args[1] = Mem
                const int rt = regNumber(inst.args[0].label);
                const Operand &mem = inst.args[1];
                if (mem.kind != Operand::Kind::Mem) {
                    throw runtime_error("lw/sw: second operand must be Mem");
                }
                const int rs = regNumber(mem.label); // label = "$gp", "$sp", etc.
                const int16_t imm = static_cast<int16_t>(mem.imm);
                w = encodeI(opcode, rs, rt, imm);
            } else if (inst.op == "beq") {
                // beq rs, rt, label
                if (inst.args.size() != 3)
                    throw runtime_error("beq needs 3 operands");
                const int rs = regNumber(inst.args[0].label);
                const int rt = regNumber(inst.args[1].label);
                const Operand &op3 = inst.args[2];
                if (op3.kind != Operand::Kind::LabelRef)
                    throw runtime_error("beq: third operand must be a Label");
                auto itL = sym.find(op3.label);
                if (itL == sym.end())
                    throw runtime_error("Branch Label not found: " + op3.label);
                const uint32_t target = itL->second;
                // offset = (target - (pc+4))/4
                const int32_t diff = static_cast<int32_t>(target) - static_cast<int32_t>(pc + 4);
                const int16_t offset = static_cast<int16_t>(diff / 4);
                w = encodeI(opcode, rs, rt, offset);
            } else
                throw runtime_error("I-type not implemented yet: " + inst.op);
        } else if (type == 'J') {
            // J-type: j, jal label
            if (inst.args.size() != 1)
                throw runtime_error(inst.op + " needs 1 operand");
            const Operand &a0 = inst.args[0];
            if (a0.kind != Operand::Kind::LabelRef)
                throw runtime_error(inst.op + ": Operand must be a label");
            auto itL = sym.find(a0.label);
            if (itL == sym.end())
                throw runtime_error("Jump Label not fount: " + a0.label);
            const uint32_t addr = itL->second;
            const uint32_t target26 = (addr >> 2) & 0x03FFFFFFu;
            w = encodeJ(opcode, target26);
        }

        machine.push_back(w);
        pc += 4;
    }

    return machine;
}
