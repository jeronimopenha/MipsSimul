#include <common.h>
#include <minic_lexer.h>

using namespace std;

int main() {
    constexpr int projectRootOffset = 1;

    const string rootPath = verifyPath(getProjectRoot(projectRootOffset));

    cout << rootPath << endl;

    auto files = getFilesListByExtension(rootPath + BenchMiniCPath, benchMiniCExt);

    for (const auto &[fst, snd]: files) {
        cout << fst << endl;

        ifstream in(fst);

        if (!in) {
            cerr << "Opening error " << fst << "\n";
            return 1;
        }

        string src((istreambuf_iterator<char>(in)),
                   istreambuf_iterator<char>());

        MiniCLexer lex(src);


        while (true) {
            Token t = lex.nextToken();
            if (t.kind == TOK_EOF) {
                break;
            }

            cout << t.line << ":\t";
            cout << t.col << ":\t";

            cout << minicTokenKindToString(static_cast<MiniCTokenKind>(t.kind)) << "\t";
            cout << t.lexeme << "\n";
        }
    }


    return 0;
}
