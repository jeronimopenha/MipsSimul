#include <asm_parser.h>

#include "disasm.h"

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

//TODO implement with "match()"
/*
 operand ::= REGISTER
          | NUMBER
          | memaddr
          | IDENT
*/
AsmOperand AsmParser::parseOperand() {
    if (match(ASM_REG)) {
        const Token r = previous();
        AsmOperand op = {AsmOperand::Kind::REG, -1, -1, -1, r.lexeme,};
        return op;
    }


    if (match(ASM_MINUS)) {
        if (!check(ASM_INT_LIT)) {
            error(peekNext(), "integer expected");
        }
    }

    if (match(ASM_INT_LIT)) {
        // can be immediate or memaddr number
        const Token num = previous();

        int imm = stoi( // parse num -> int
            num.lexeme,
            nullptr,
            (
                num.lexeme.rfind("0x", 0) == 0 ||
                num.lexeme.rfind("0X", 0) == 0
            )
                ? 16
                : 10
        );

        if (previousTwo().kind == ASM_MINUS) {
            imm *= -1;
        }

        if (check(ASM_L_PAREN)) {
            // is mem addr: num "(" reg ")"
            get(); // consume "("
            const Token r = expect(ASM_REG, "address register");
            expect(ASM_R_PAREN, "')'");


            AsmOperand op = {
                AsmOperand::Kind::MEM,
                -1,
                imm,
                -1, // map later
                r.lexeme // name of the reg base
            };
            return op;
        }

        // just immediate
        AsmOperand op = {
            AsmOperand::Kind::IMM,
            -1,
            imm,
            -1, // map later
        };

        return op;
    }

    if (match(ASM_L_PAREN)) {
        // memaddr with no offset: "($sp)"
        const Token r = expect(ASM_REG, "address register");
        expect(ASM_R_PAREN, "')'");

        AsmOperand op = {
            AsmOperand::Kind::MEM,
            -1,
            0,
            -1,
            r.lexeme

        };
        return op;
    }

    if (match(ASM_IDENT)) {
        // can be a label for jump and branche
        const Token id = previous();

        AsmOperand op = {
            AsmOperand::Kind::LABELREF,
            -1,
            -1,
            -1,
            id.lexeme

        };
        return op;
    }


    error(peek(), "Unexpected operand");
    /*cerr << "Unexpected operand on line" << t.line << ", column " << t.col << " with lexeme: '" << t.lexeme <<"'" << "\n";
    throw runtime_error("parsing errorg: operand");*/
    return {};
}
