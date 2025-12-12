#ifndef COMMON_TOKEN_H
#define COMMON_TOKEN_H

#include <definitions.h>

struct Token {
    int kind;
    std::string lexeme;
    int line;
    int col;

    int32_t intValue = 0;
    double floatValue = 0.0;

    Token();

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
