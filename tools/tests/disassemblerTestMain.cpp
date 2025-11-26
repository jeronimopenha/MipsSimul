#include <asm.h>
#include <disasm.h>
#include <common.h>

using namespace std;

int main() {
    constexpr int projectRootOffset = 1;

    const string rootPath = verifyPath(getProjectRoot(projectRootOffset));

    cout << rootPath << endl;

    auto files = getFilesListByExtension(rootPath + benchPath, benchAsmExt);

    for (const auto &[fst, snd]: files) {
        cout << fst << endl;

        ifstream in(fst);
        if (!in) {
            cerr << "Error while opening file " << fst << "\n";
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


        auto sym = buildSymbolTable(prog);
        auto code = generateCode(prog, sym);

        auto labels = makeLabels(code);

        uint32_t pc = basePc;
        for (auto w: code) {
            // print LABEL if it exists
            auto it = labels.find(pc);
            if (it != labels.end()) {
                cout << it->second << ":\n";
            }

            cout << hex << uppercase
                    << "0x" << setw(8) << setfill('0') << pc
                    << ":  " << disassembleWord(w, pc, &labels) << "\n";

            pc += 4;
        }
    }

    return 0;
}
