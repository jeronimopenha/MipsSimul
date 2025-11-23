#include <tools_cli.h>

bool parseCli(const int argc, char **argv, CliOptions &opt) {
    bool hasTool = false;
    for (int i = 1; i < argc; ++i) {
        std::string a = argv[i];
        if (a == "-h" || a == "--help") {
            return false;
        }
        if ((a == "-t" || a == "--tool") && i + 1 < argc) {
            std::string t = argv[++i];
            if (t == "lex") {
                opt.tool = Tool::Lexer;
                hasTool = true;
            } else if (t == "parse") {
                opt.tool = Tool::Parser;
                hasTool = true;
            } else if (t == "asm") {
                opt.tool = Tool::Assembler;
                hasTool = true;
            } else if (t == "disasm") {
                opt.tool = Tool::Disassembler;
                hasTool = true;
            } else {
                std::cerr << "Unknown tool: " << t << "\n";
                return false;
            }
        } else if (a == "-i" && i + 1 < argc) {
            opt.inputFile = argv[++i];
        } else if (a == "-o" && i + 1 < argc) {
            opt.outputFile = argv[++i];
        } else {
            std::cerr << "Unknown arg: " << a << "\n";
            return false;
        }
    }
    if (!hasTool) {
        std::cerr << "Missing -t {lex|parse|asm|disasm}\n";
        return false;
    }
    return true;
}
