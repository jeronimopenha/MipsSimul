#include <asm_parser.h>

using namespace std;

//program    ::= { line }
vector<AsmLine> AsmParser::parseProgram() {
    vector<AsmLine> prog;
    while (true) {
        if (peek().kind == ASM_EOF) {
            break;
        }
        // multiple Newlines
        if (peek().kind == ASM_NEWLINE) {
            get();
            continue;
        }
        prog.push_back(parseLine());
    }
    return prog;
}

//line ::= [ label ":" ] [ instruction ] NEWLINE
AsmLine AsmParser::parseLine() {
    AsmLine line;
    // [label ":" ] ?
    if (peek().kind == ASM_IDENT) {
        // can be label OR opcode
        const Token ident = peek();
        // Look to the other token to decide
        if (tokens.size() > pos + 1 && tokens[pos + 1].kind == ASM_COLON) {
            // is labeled
            get(); // consume IDENT
            get(); // consume COLON
            line.label = ident.lexeme;
        }
    }

    // [instruction] ?
    // if the next token is IDENT or REGISTER etc, try instruction
    if (peek().kind == ASM_IDENT) {
        line.hasInstr = true;
        line.instr = parseInstruction();
    }

    // consume at least a NEWLINE (could be more)
    while (peek().kind == ASM_NEWLINE) {
        get();
        break;
    }

    return line;
}

//instruction ::= IDENT operand_list
MiniCInstruction AsmParser::parseInstruction() {
    MiniCInstruction inst;
    const Token opTok = expect(ASM_IDENT, "opcode");
    inst.op = opTok.lexeme;
    inst.line = opTok.line;

    // if line ends after the opcode (ex: "nop")
    if (peek().kind == ASM_NEWLINE || peek().kind == ASM_EOF) {
        return inst;
    }

    //parse the operands list
    inst.args = parseOperandList();
    return inst;
}

//operand_list ::= operand { "," operand }
vector<AsmOperand> AsmParser::parseOperandList() {
    vector<AsmOperand> args;
    args.push_back(parseOperand());

    while (match(ASM_COMMA)) {
        args.push_back(parseOperand());
    }
    return args;
}

/*
 operand ::= REGISTER
          | NUMBER
          | memaddr
          | IDENT
*/
AsmOperand AsmParser::parseOperand() {
    const Token &t = peek();
    AsmOperand op;

    switch (t.kind) {
        case ASM_REG: {
            const Token r = get();
            op.kind = AsmOperand::Kind::Reg;
            op.reg = -1; // lets map this later (semantic phase)
            // we can store the name here to map later
            op.label = r.lexeme;
            break;
        }

        case ASM_INT_LIT: {
            // can be immediate or memaddr number
            const Token num = get();

            if (peek().kind == ASM_L_PAREN) {
                // is memaddr: num "(" reg ")"
                get(); // consume "("
                const Token r = expect(ASM_REG, "address register");
                expect(ASM_R_PAREN, "')'");

                op.kind = AsmOperand::Kind::Mem;
                // parse num -> int
                op.imm = stoi(num.lexeme, nullptr,
                              (num.lexeme.rfind("0x", 0) == 0 || num.lexeme.rfind("0X", 0) == 0) ? 16 : 10);
                op.baseReg = -1; // map later
                op.label = r.lexeme; // name of the reg base
            } else {
                // just immediate
                op.kind = AsmOperand::Kind::Imm;
                op.imm = stoi(num.lexeme, nullptr,
                              (num.lexeme.rfind("0x", 0) == 0 || num.lexeme.rfind("0X", 0) == 0) ? 16 : 10);
            }
            break;
        }

        case ASM_L_PAREN: {
            // memaddr with no offset: "($sp)"
            get(); // consume "("
            const Token r = expect(ASM_REG, "address register");
            expect(ASM_R_PAREN, "')'");

            op.kind = AsmOperand::Kind::Mem;
            op.imm = 0;
            op.baseReg = -1; // map later
            op.label = r.lexeme; // nome of the base reg
            break;
        }

        case ASM_IDENT: {
            // can be a label for jump and branche
            const Token id = get();
            op.kind = AsmOperand::Kind::LabelRef;
            op.label = id.lexeme;
            break;
        }

        default:
            cerr << "Unexpected operand on line " << t.line << "\n";
            throw runtime_error("parsing errorg: operand");
    }

    return op;
}
