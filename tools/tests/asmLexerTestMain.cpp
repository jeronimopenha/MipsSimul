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
            AsmToken t = lex.next();
            if (t.kind == AsmTokenKind::Eof) {
                break;
            }

            cout << t.line << ":\t";

            switch (t.kind) {
                case AsmTokenKind::Identifier: cout << "IDENT\t";
                    break;
                case AsmTokenKind::Register: cout << "REG  \t";
                    break;
                case AsmTokenKind::Number: cout << "NUM  \t";
                    break;
                case AsmTokenKind::Comma: cout << "COMMA\t";
                    break;
                case AsmTokenKind::Colon: cout << "COLON\t";
                    break;
                case AsmTokenKind::LParen: cout << "LPAR \t";
                    break;
                case AsmTokenKind::RParen: cout << "RPAR \t";
                    break;
                case AsmTokenKind::Newline: cout << "NEWLN\t";
                    break;
                default: cout << "???  \t";
                    break;
            }
            cout << t.lexeme << "\n";
        }
    }


    return 0;
}
