#ifndef COMMON_TOKEN_H
#define COMMON_TOKEN_H

#include <definitions.h>

#include "asm_lexer.h"
#include "asm_tokens.h"

struct Token {
    int kind;
    std::string lexeme;
    int line;
    int col;

    int32_t intValue;
    double floatValue;

    Token(
        const int kind,
        std::string lexeme,
        const int line,
        const int col) : kind(kind),
                         lexeme(std::move(lexeme)),
                         line(line),
                         col(col) {
    }
};

#endif
