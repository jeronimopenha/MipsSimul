#include <asm_parser.h>

using namespace std;

Parser::Parser(const vector<Token> &tokens) : tokens(tokens) {
}

const Token &Parser::peek() {
    static Token eofToken{TOK_EOF, "", -1,-1};
    if (pos >= tokens.size()) {
        return eofToken;
    }
    return tokens[pos];
}

const Token &Parser::get() {
    const Token &t = peek();
    if (pos < tokens.size()) {
        pos++;
    }
    return t;
}

bool Parser::match(const AsmTokenKind k) {
    if (peek().kind == k) {
        get();
        return true;
    }
    return false;
}

const Token &Parser::expect(const AsmTokenKind k, const string &msg) {
    if (peek().kind != k) {
        cerr << "Error on line " << peek().line
                << ": expected " << msg << "\n";
        throw runtime_error("parsing error");
    }
    return get();
}

//program    ::= { line }
vector<AsmLine> Parser::parseProgram() {
    vector<AsmLine> prog;
    while (true) {
        if (peek().kind == TOK_EOF) {
            break;
        }
        // multiple Newlines
        if (peek().kind == TOK_NEWLINE) {
            get();
            continue;
        }
        prog.push_back(parseLine());
    }
    return prog;
}

//line ::= [ label ":" ] [ instruction ] NEWLINE
AsmLine Parser::parseLine() {
    AsmLine line;
    // [label ":" ] ?
    if (peek().kind == TOK_IDENT) {
        // can be label OR opcode
        const Token ident = peek();
        // Look to the other token to decide
        if (tokens.size() > pos + 1 && tokens[pos + 1].kind == TOK_COLON) {
            // is labeled
            get(); // consume IDENT
            get(); // consume COLON
            line.label = ident.lexeme;
        }
    }

    // [instruction] ?
    // if the next token is IDENT or REGISTER etc, try instruction
    if (peek().kind == TOK_IDENT) {
        line.hasInstr = true;
        line.instr = parseInstruction();
    }

    // consume at least a NEWLINE (could be more)
    while (peek().kind == TOK_NEWLINE) {
        get();
        break;
    }

    return line;
}

//instruction ::= IDENT operand_list
AsmInstruction Parser::parseInstruction() {
    AsmInstruction inst;
    const Token opTok = expect(TOK_IDENT, "opcode");
    inst.op = opTok.lexeme;
    inst.line = opTok.line;

    // if line ends after the opcode (ex: "nop")
    if (peek().kind == TOK_NEWLINE || peek().kind == TOK_EOF) {
        return inst;
    }

    //parse the operands list
    inst.args = parseOperandList();
    return inst;
}

//operand_list ::= operand { "," operand }
vector<AsmOperand> Parser::parseOperandList() {
    vector<AsmOperand> args;
    args.push_back(parseOperand());

    while (match(TOK_COMMA)) {
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
AsmOperand Parser::parseOperand() {
    const Token& t = peek();
    AsmOperand op;

    switch (t.kind) {
    case TOK_REG: {
        const Token r = get();
        op.kind = AsmOperand::Kind::Reg;
        op.reg = -1;         // lets map this later (semantic phase)
        // we can store the name here to map later
        op.label = r.lexeme;
        break;
    }

    case TOK_INT_LIT: {
        // can be immediate or memaddr number
        const Token num = get();

        if (peek().kind ==TOK_L_PAREN) {
            // is memaddr: num "(" reg ")"
            get(); // consume "("
            const Token r = expect(TOK_REG, "address register");
            expect(TOK_R_PAREN, "')'");

            op.kind = AsmOperand::Kind::Mem;
            // parse num -> int
            op.imm = stoi(num.lexeme, nullptr,
                               (num.lexeme.rfind("0x",0)==0 || num.lexeme.rfind("0X",0)==0) ? 16 : 10);
            op.baseReg = -1;  // map later
            op.label = r.lexeme; // name of the reg base
        } else {
            // just immediate
            op.kind = AsmOperand::Kind::Imm;
            op.imm = stoi(num.lexeme, nullptr,
                               (num.lexeme.rfind("0x",0)==0 || num.lexeme.rfind("0X",0)==0) ? 16 : 10);
        }
        break;
    }

    case TOK_L_PAREN: {
        // memaddr with no offset: "($sp)"
        get(); // consume "("
        const Token r = expect(TOK_REG, "address register");
        expect(TOK_R_PAREN, "')'");

        op.kind = AsmOperand::Kind::Mem;
        op.imm = 0;
        op.baseReg = -1;     // map later
        op.label = r.lexeme; // nome of the base reg
        break;
    }

    case TOK_IDENT: {
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
