#ifndef ASSEMBLER_TOKENS_H
#define ASSEMBLER_TOKENS_H

#include <definitions.h>

enum class AsmTokenKind {
    Identifier, // opcodes, labels, regs without $
    Register, // tokens with initial $
    Number, // immediate (decimal or hex)
    Comma, // ,
    Colon, // :
    LParen, // (
    RParen, // )
    Newline, // end of line
    Eof // End of file
};

struct AsmToken {
    AsmTokenKind kind;
    std::string lexeme; // original text
    int line; // line of the file to detect error
};

std::string asmTokenKindToString(AsmTokenKind k);

#endif
