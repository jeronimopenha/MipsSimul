#include <minic_parser.h>
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
            cerr << "Error while opening file " << fst << "\n";
            return 1;
        }
        string src((istreambuf_iterator<char>(in)),
                   istreambuf_iterator<char>());

        MiniCLexer lex(src);
        vector<Token> tokens;
        while (true) {
            Token t = lex.nextToken();
            if (t.kind == TOK_NEWLN) continue;
            tokens.push_back(t);
            if (t.kind == TOK_EOF) {
                break;
            }
        }

        MiniCParser parser(tokens);
        //FIXME fix this in future
        //auto prog = p.parseExpr();
        vector<unique_ptr<AstNode> > prog = parser.parseProgram();
        for (const auto &line: prog) {
            line->dump(0);
        }
    }


    return 0;
}
