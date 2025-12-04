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
            if (t.kind == AsmTokenKind::TOK_EOF) {
                break;
            }

            cout << t.line << ":\t";

            switch (t.kind) {
                case AsmTokenKind::TOK_IDENT: cout << "IDENT\t";
                    break;
                case AsmTokenKind::TOK_REG: cout << "REG  \t";
                    break;
                case AsmTokenKind::TOK_INT_LIT: cout << "NUM  \t";
                    break;
                case AsmTokenKind::TOK_COMMA: cout << "COMMA\t";
                    break;
                case AsmTokenKind::TOK_COLON: cout << "COLON\t";
                    break;
                case AsmTokenKind::TOK_L_PAREN: cout << "LPAR \t";
                    break;
                case AsmTokenKind::TOK_R_PAREN: cout << "RPAR \t";
                    break;
                case AsmTokenKind::TOK_NEWLINE: cout << "NEWLN\t";
                    break;
                default: cout << "???  \t";
                    break;
            }
            cout << t.lexeme << "\n";
        }
    }


    return 0;
}
