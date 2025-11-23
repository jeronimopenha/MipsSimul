#include <definitions.h>
#include <tools_cli.h>
#include <lexer.h>
#include <parser.h>
#include <assembler.h>
#include <disassembler.h>

using namespace std;

int runLexer(const istream &in, ostream &out) {
    stringstream buffer;
    buffer << in.rdbuf();
    const string src = buffer.str();

    Lexer lex(src);
    while (true) {
        Token t = lex.next();
        out << t.line << ":\t" << tokenKindToString(t.kind) << "\t" << t.lexeme << "\n";
        if (t.kind == TokenKind::Eof) break;
    }
    return 0;
}

int runParser(const istream &in, ostream &out) {
    stringstream buffer;
    buffer << in.rdbuf();
    const string src = buffer.str();

    Lexer lex(src);
    vector<Token> toks;
    while (true) {
        Token t = lex.next();
        toks.push_back(t);
        if (t.kind == TokenKind::Eof) break;
    }

    Parser p(toks);
    const auto prog = p.parseProgram();

    // print representation
    for (const auto &line: prog) {
        if (!line.label.empty()) {
            out << line.label << ":\n";
        }
        if (line.hasInstr) {
            const auto &inst = line.instr;
            out << "  " << inst.op;
            for (const auto &a: inst.args) {
                out << "  [";
                switch (a.kind) {
                    case Operand::Kind::Reg: out << "Reg " << a.label;
                        break;
                    case Operand::Kind::Imm: out << "Imm " << a.imm;
                        break;
                    case Operand::Kind::Mem: out << "Mem off=" << a.imm << " base=" << a.label;
                        break;
                    case Operand::Kind::LabelRef: out << "Label " << a.label;
                        break;
                }
                out << "]";
            }
            out << "\n";
        }
    }
    return 0;
}

int runAssembler(const istream &in, ostream &out) {
    stringstream buffer;
    buffer << in.rdbuf();
    string src = buffer.str();

    try {
        Lexer lex(src);
        vector<Token> toks;
        while (true) {
            Token t = lex.next();
            toks.push_back(t);
            if (t.kind == TokenKind::Eof) break;
        }

        Parser p(toks);
        auto prog = p.parseProgram();

        auto sym = buildSymbolTable(prog);
        auto code = generateCode(prog, sym);

        out << uppercase << hex;
        for (auto w: code) {
            out << "0x" << setw(8) << setfill('0') << w << "\n";
        }
    } catch (const exception &e) {
        cerr << "Assembler error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}

int runDisassembler(istream &in, ostream &out) {
    vector<uint32_t> code;
    string line;
    while (getline(in, line)) {
        if (line.empty()) continue;
        if (line.rfind("0x", 0) == 0 || line.rfind("0X", 0) == 0) {
            line = line.substr(2);
        }
        stringstream ss(line);
        uint32_t w;
        ss >> hex >> w;
        if (ss.fail()) {
            cerr << "Invalid hex line: " << line << "\n";
            return 1;
        }
        code.push_back(w);
    }

    auto labels = makeLabels(code);

    uint32_t pc = basePc;
    for (const auto w: code) {
        auto it = labels.find(pc);
        if (it != labels.end()) {
            out << it->second << ":\n";
        }
        out << hex << uppercase
                << "0x" << setw(8) << setfill('0') << pc
                << ":  " << disassembleWord(w, pc, &labels) << "\n";
        pc += 4;
    }
    return 0;
}

int main(int argc, char **argv) {
    CliOptions opt;
    if (!parseCli(argc, argv, opt)) {
        cerr << "Usage: mips_tool -t {lex|parse|asm|disasm} [-i file] [-o file]\n";
        return 1;
    }

    // input
    istream *in = &cin;
    ifstream fin;
    if (!opt.inputFile.empty()) {
        fin.open(opt.inputFile);
        if (!fin) {
            cerr << "Error opening input: " << opt.inputFile << "\n";
            return 1;
        }
        in = &fin;
    }

    // output
    ostream *out = &cout;
    ofstream fout;
    if (!opt.outputFile.empty()) {
        fout.open(opt.outputFile);
        if (!fout) {
            cerr << "Error opening output: " << opt.outputFile << "\n";
            return 1;
        }
        out = &fout;
    }

    switch (opt.tool) {
        case Tool::Lexer:
            return runLexer(*in, *out);
        case Tool::Parser:
            return runParser(*in, *out);
        case Tool::Assembler:
            return runAssembler(*in, *out);
        case Tool::Disassembler:
            return runDisassembler(*in, *out);
    }
    return 0;
}
