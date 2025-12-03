#include <common.h>
#include <minic_lexer.h.h>

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

        Lexer lex(src);

        while (true) {
            Token t = lex.next();
            if (t.kind == TokenKind::Eof) {
                break;
            }

            cout << t.line << ":\t";

            switch (t.kind) {
                case TokenKind::Identifier: cout << "IDENT\t";
                    break;
                case TokenKind::Register: cout << "REG  \t";
                    break;
                case TokenKind::Number: cout << "NUM  \t";
                    break;
                case TokenKind::Comma: cout << "COMMA\t";
                    break;
                case TokenKind::Colon: cout << "COLON\t";
                    break;
                case TokenKind::LParen: cout << "LPAR \t";
                    break;
                case TokenKind::RParen: cout << "RPAR \t";
                    break;
                case TokenKind::Newline: cout << "NEWLN\t";
                    break;
                default: cout << "???  \t";
                    break;
            }
            cout << t.lexeme << "\n";
        }
    }


    return 0;
}
