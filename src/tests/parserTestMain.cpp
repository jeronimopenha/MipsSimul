#include <parser.h>
#include <common.h>

using namespace std;

int main(int argc, char **argv) {
    /*if (argc < 2) {
        cerr << "Use: " << argv[0] << " archive.asm\n";
        return 1;
    }

    ifstream in(argv[1]);*/


    const string rootPath = verifyPath(getProjectRoot());

    cout << rootPath << endl;

    auto files = getFilesListByExtension(rootPath + benchPath, benchAsmExt);

    for (const auto &[fst, snd]: files) {
        cout << fst << endl;

        ifstream in(fst);
        if (!in) {
            cerr << "Error while opening file " << argv[1] << "\n";
            return 1;
        }
        string src((istreambuf_iterator<char>(in)),
                        istreambuf_iterator<char>());

        Lexer lex(src);
        vector<Token> tokens;
        while (true) {
            Token t = lex.next();
            tokens.push_back(t);
            if (t.kind == TokenKind::Eof) {
                break;
            }
        }

        Parser p(tokens);
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
                        case Operand::Kind::Reg: cout << "Reg " << a.label;
                            break;
                        case Operand::Kind::Imm: cout << "Imm " << a.imm;
                            break;
                        case Operand::Kind::Mem: cout << "Mem off=" << a.imm << " base=" << a.label;
                            break;
                        case Operand::Kind::LabelRef: cout << "Label " << a.label;
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
