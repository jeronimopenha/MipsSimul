#ifndef MIPSSIMUL_LEXER_H
#define MIPSSIMUL_LEXER_H

#include <definitions.h>

enum class TokenKind {
    Identifier, // opcodes, labels, regs without $
    Register, // tokens with initial $
    Number, // imediates (decimal or hexa)
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

struct Lexer {
    std::string src;
    size_t pos = 0;
    int line = 1;

    Lexer(const std::string &s);

    [[nodiscard]] bool eof() const;

    [[nodiscard]] char peek() const;

    char get();

    static bool isIdentStart(char c);

    static bool isIdentChar(char c);

    Token next();
};

#endif //MIPSSIMUL_LEXER_H
