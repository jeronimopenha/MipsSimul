#include <common.h>
#include <asm_lexer.h>

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
            cerr << "Opening error " << fst << "\n";
            return 1;
        }

        string src((istreambuf_iterator<char>(in)),
                   istreambuf_iterator<char>());

        AsmLexer lex(src);


        Token t;
        while (t.kind != ASM_EOF) {
            t = lex.nextToken();

            cout << t.tokenToString();
        }
    }


    return 0;
}
