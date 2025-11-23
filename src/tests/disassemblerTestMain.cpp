#include <assembler.h>
#include <disassembler.h>
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
            if (t.kind == TokenKind::Eof) {
                break;
            }
        }

        Parser p(tokens);
        auto prog = p.parseProgram();


        auto sym = buildSymbolTable(prog);
        auto code = generateCode(prog, sym);

        auto labels = makeLabels(code);

        uint32_t pc = basePc;
        for (auto w : code) {
            // print LABEL if it exists
            auto it = labels.find(pc);
            if (it != labels.end()) {
                std::cout << it->second << ":\n";
            }

            std::cout << std::hex << std::uppercase
                      << "0x" << std::setw(8) << std::setfill('0') << pc
                      << ":  " << disassembleWord(w, pc, &labels) << "\n";

            pc += 4;
        }
    }

    return 0;
}
