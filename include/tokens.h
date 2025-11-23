#ifndef MIPSSIMUL_TOKENS_H
#define MIPSSIMUL_TOKENS_H

#include <definitions.h>

enum class TokenKind {
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

struct Token {
    TokenKind kind;
    std::string lexeme; // original text
    int line; // line of the file to detect error
};

#endif //MIPSSIMUL_TOKENS_H
