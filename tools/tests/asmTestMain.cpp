#include <asm.h>
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
        vector<MiniCToken> tokens;
        while (true) {
            MiniCToken t = lex.next();
            tokens.push_back(t);
            if (t.kind == TokenKind::Eof) {
                break;
            }
        }

        Parser p(tokens);
        auto prog = p.parseProgram();


        auto sym = asmBuildSymbolTable(prog);
        auto code = asmGenerateCode(prog, sym);

        cout << uppercase << hex;
        for (auto w: code) {
            cout << "0x" << setw(8) << setfill('0') << w << "\n";
        }
    }

    return 0;
}
