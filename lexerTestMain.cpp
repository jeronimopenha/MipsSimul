#include <lexer.h>
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

        ifstream in(fst);

        if (!in) {
            cerr << "Opening error " << argv[1] << "\n";
            return 1;
        }

        string src((istreambuf_iterator<char>(in)),
                   istreambuf_iterator<char>());

        Lexer lex(src);

        while (true) {
            Token t = lex.next();
            if (t.kind == TokenKind::Eof) break;

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