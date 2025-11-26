#ifndef MIPSSIMUL_TOOLS_CLI_H
#define MIPSSIMUL_TOOLS_CLI_H

#include <definitions.h>

enum class Tool {
    Lexer,
    Parser,
    Assembler,
    Disassembler
};

struct CliOptions {
    Tool tool = Tool::Assembler; // default
    std::string inputFile;
    std::string outputFile;
};

bool parseCli(int argc, char **argv, CliOptions &opt);

#endif //MIPSSIMUL_TOOLS_CLI_H
