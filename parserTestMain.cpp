#include <parser.h>
#include <util.h>

using namespace std;

int main(int argc, char **argv) {
    /*if (argc < 2) {
        cerr << "Use: " << argv[0] << " archive.asm\n";
        return 1;
    }

    ifstream in(argv[1]);*/


    const string rootPath = verifyPath(getProjectRoot());

    cout << rootPath << endl;

    auto files = getFilesListByExtension(rootPath + benchPath, benchExt);

    for (const auto &[fst, snd]: files) {
        cout << fst << endl;

        std::ifstream in(fst);
        if (!in) {
            std::cerr << "Error while opening file " << argv[1] << "\n";
            return 1;
        }
        std::string src((std::istreambuf_iterator<char>(in)),
                        std::istreambuf_iterator<char>());

        Lexer lex(src);
        std::vector<Token> tokens;
        while (true) {
            Token t = lex.next();
            tokens.push_back(t);
            if (t.kind == TokenKind::Eof) break;
        }

        Parser p(tokens);
        auto prog = p.parseProgram();

        // Só pra testar: imprimir o que o parser entendeu
        for (auto &line: prog) {
            if (!line.label.empty()) {
                std::cout << line.label << ":\n";
            }
            if (line.hasInstr) {
                std::cout << "  " << line.instr.op;
                for (auto &a: line.instr.args) {
                    std::cout << "  [";
                    switch (a.kind) {
                        case Operand::Kind::Reg: std::cout << "Reg " << a.label;
                            break;
                        case Operand::Kind::Imm: std::cout << "Imm " << a.imm;
                            break;
                        case Operand::Kind::Mem: std::cout << "Mem off=" << a.imm << " base=" << a.label;
                            break;
                        case Operand::Kind::LabelRef: std::cout << "Label " << a.label;
                            break;
                    }
                    std::cout << "]";
                }
                std::cout << "\n";
            }
        }
    }


    return 0;
}
