#include <definitions.h>
#include <assembler_cli.h>
#include <asm_lexer.h>
#include <asm_parser.h>
#include <asm.h>
#include <disasm.h>
#include <asm_t_kind.h>

using namespace std;

int runLexer(const istream &in, ostream &out) {
    stringstream buffer;
    buffer << in.rdbuf();
    const string src = buffer.str();

    AsmLexer lex(src);
    while (true) {
        Token t = lex.nextToken();
        out << t.line << ":\t" << asmTokenKindToString(t.kind) << "\t" << t.lexeme << "\n";
        if (t.kind == ASM_EOF) break;
    }
    return 0;
}

int runParser(const istream &in, ostream &out) {
    stringstream buffer;
    buffer << in.rdbuf();
    const string src = buffer.str();

    AsmLexer lex(src);
    vector<Token> tokens;
    while (true) {
        Token t = lex.nextToken();
        tokens.push_back(t);
        if (t.kind == ASM_EOF) break;
    }

    AsmParser p(tokens);
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
                    case AsmOperand::Kind::REG: out << "Reg " << a.label;
                        break;
                    case AsmOperand::Kind::IMM: out << "Imm " << a.imm;
                        break;
                    case AsmOperand::Kind::MEM: out << "Mem off=" << a.imm << " base=" << a.label;
                        break;
                    case AsmOperand::Kind::LABELREF: out << "Label " << a.label;
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
        AsmLexer lex(src);
        vector<Token> tokens;
        while (true) {
            Token t = lex.nextToken();
            tokens.push_back(t);
            if (t.kind == ASM_EOF) break;
        }

        AsmParser p(tokens);
        auto prog = p.parseProgram();

        auto sym = asmBuildSymbolTable(prog);
        auto code = asmGenerateCode(prog, sym);

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

    auto labels = disasmMakeLabels(code);

    uint32_t pc = baseAsmPc;
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
