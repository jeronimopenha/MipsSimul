#include <asm_parser.h>
#include <common.h>

using namespace std;

int main() {
    constexpr int projectRootOffset = 1;

    const string rootPath = verifyPath(getProjectRoot(projectRootOffset));

    cout << rootPath << endl;

    auto files = getFilesListByExtension(rootPath + BenchAsmPath, benchAsmExt);

    for (const auto &[fst, snd]: files) {
        cout << fst << endl;

        ifstream in(fst);
        if (!in) {
            cerr << "Error while opening file " << fst << "\n";
            return 1;
        }
        string src((istreambuf_iterator<char>(in)),
                   istreambuf_iterator<char>());

        AsmLexer lex(src);
        vector<Token> tokens;
        while (true) {
            Token t = lex.nextToken();
            tokens.push_back(t);
            if (t.kind == ASM_EOF) {
                break;
            }
        }

        AsmParser p(tokens);
        auto prog = p.parseProgram();

        // Só pra testar: imprimir o que o parser entendeu
        for (auto &line: prog) {
            if (!line.label.empty()) {
                cout << line.label << ":\n";
            }
            if (line.hasInstr) {
                cout << "  " << line.instr.op;
                for (auto &a: line.instr.args) {
                    cout << "  [";
                    switch (a.kind) {
                        case AsmOperand::Kind::REG: cout << "Reg " << a.label;
                            break;
                        case AsmOperand::Kind::IMM: cout << "Imm " << a.imm;
                            break;
                        case AsmOperand::Kind::MEM: cout << "Mem off=" << a.imm << " base=" << a.label;
                            break;
                        case AsmOperand::Kind::LABELREF: cout << "Label " << a.label;
                            break;
                    }
                    cout << "]";
                }
                cout << "\n";
            }
        }
    }


    return 0;
}
