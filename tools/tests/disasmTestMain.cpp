#include <asm.h>
#include <disasm.h>
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
        vector<Token> tokens;
        while (true) {
            Token t = lex.nextToken();
            tokens.push_back(t);
            if (t.kind == ASM_EOF) {
                break;
            }
        }

        AsmParser p(tokens);
        auto prog = p.parseProgram();


        auto sym = asmBuildSymbolTable(prog);
        auto code = asmGenerateCode(prog, sym);

        auto labels = disasmMakeLabels(code);

        uint32_t pc = baseAsmPc;
        for (auto w: code) {
            // print LABEL if it exists
            auto it = labels.find(pc);
            if (it != labels.end()) {
                cout << it->second << ":\n";
            }

            cout << hex << uppercase
                    << "0x" << setw(8) << setfill('0') << pc
                    << ":  " << disassembleWord(w, pc, &labels) << "\n";

            pc += 4;
        }
    }

    return 0;
}
