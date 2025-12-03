#include <asm_parser.h>

using namespace std;

Parser::Parser(const vector<MiniCToken> &tokens) : tokens(tokens) {
}

const MiniCToken &Parser::peek() {
    static MiniCToken eofToken{TokenKind::Eof, "", -1};
    if (pos >= tokens.size()) {
        return eofToken;
    }
    return tokens[pos];
}

const MiniCToken &Parser::get() {
    const MiniCToken &t = peek();
    if (pos < tokens.size()) {
        pos++;
    }
    return t;
}

bool Parser::match(const TokenKind k) {
    if (peek().kind == k) {
        get();
        return true;
    }
    return false;
}

const MiniCToken &Parser::expect(const TokenKind k, const string &msg) {
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
        if (peek().kind == TokenKind::Eof) {
            break;
        }
        // multiple Newlines
        if (peek().kind == TokenKind::Newline) {
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
    if (peek().kind == TokenKind::Identifier) {
        // can be label OR opcode
        const MiniCToken ident = peek();
        // Look to the other token to decide
        if (tokens.size() > pos + 1 && tokens[pos + 1].kind == TokenKind::Colon) {
            // is labeled
            get(); // consume IDENT
            get(); // consume COLON
            line.label = ident.lexeme;
        }
    }

    // [instruction] ?
    // if the next token is IDENT or REGISTER etc, try instruction
    if (peek().kind == TokenKind::Identifier) {
        line.hasInstr = true;
        line.instr = parseInstruction();
    }

    // consume at least a NEWLINE (could be more)
    while (peek().kind == TokenKind::Newline) {
        get();
        break;
    }

    return line;
}

//instruction ::= IDENT operand_list
AsmInstruction Parser::parseInstruction() {
    AsmInstruction inst;
    const MiniCToken opTok = expect(TokenKind::Identifier, "opcode");
    inst.op = opTok.lexeme;
    inst.line = opTok.line;

    // if line ends after the opcode (ex: "nop")
    if (peek().kind == TokenKind::Newline || peek().kind == TokenKind::Eof) {
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

    while (match(TokenKind::Comma)) {
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
    const MiniCToken& t = peek();
    AsmOperand op;

    switch (t.kind) {
    case TokenKind::Register: {
        const MiniCToken r = get();
        op.kind = AsmOperand::Kind::Reg;
        op.reg = -1;         // lets map this later (semantic phase)
        // we can store the name here to map later
        op.label = r.lexeme;
        break;
    }

    case TokenKind::Number: {
        // can be immediate or memaddr number
        const MiniCToken num = get();

        if (peek().kind == TokenKind::LParen) {
            // is memaddr: num "(" reg ")"
            get(); // consume "("
            const MiniCToken r = expect(TokenKind::Register, "address register");
            expect(TokenKind::RParen, "')'");

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

    case TokenKind::LParen: {
        // memaddr with no offset: "($sp)"
        get(); // consume "("
        const MiniCToken r = expect(TokenKind::Register, "address register");
        expect(TokenKind::RParen, "')'");

        op.kind = AsmOperand::Kind::Mem;
        op.imm = 0;
        op.baseReg = -1;     // map later
        op.label = r.lexeme; // nome of the base reg
        break;
    }

    case TokenKind::Identifier: {
        // can be a label for jump and branche
        const MiniCToken id = get();
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
