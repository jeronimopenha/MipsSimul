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



        while (true) {
            Token t = lex.nextToken();
            if (t.kind == TOK_EOF) {
                break;
            }

            cout << t.line << ":\t";

            cout << asmTokenKindToString(static_cast<AsmTokenKind>(t.kind)) << "\t";
            cout << t.lexeme << "\n";
        }
    }


    return 0;
}
