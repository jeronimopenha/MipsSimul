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
            tokens.push_back(t);
            if (t.kind == MiniCTokenKind::TOK_EOF) {
                break;
            }
        }

        MiniCParser parser(tokens);
        //FIXME fix this in future
        //auto prog = p.parseExpr();
        while (parser.peek().kind != TOK_EOF) {
            if (parser.match(TOK_NEWLN))continue;
            unique_ptr<ExprNode> prog = parser.parseExpr();
            prog->dump(0);
        }
        int a = 1;

        /*for (auto &line: prog) {
            if (!line.label.empty()) {
                cout << line.label << ":\n";
            }
            if (line.hasInstr) {
                cout << "  " << line.instr.op;
                for (auto &a: line.instr.args) {
                    cout << "  [";
                    switch (a.kind) {
                        case AsmOperand::Kind::Reg: cout << "Reg " << a.label;
                            break;
                        case AsmOperand::Kind::Imm: cout << "Imm " << a.imm;
                            break;
                        case AsmOperand::Kind::Mem: cout << "Mem off=" << a.imm << " base=" << a.label;
                            break;
                        case AsmOperand::Kind::LabelRef: cout << "Label " << a.label;
                            break;
                    }
                    cout << "]";
                }
                cout << "\n";
            }
        }*/
    }


    return 0;
}
