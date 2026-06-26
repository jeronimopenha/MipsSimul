#ifndef MIPSSIMUL_COMMON_TOKEN_H
#define MIPSSIMUL_COMMON_TOKEN_H

#include <definitions.h>

struct Token {
    int kind;
    std::string lexeme = "";
    int line = 0;
    int col = 0;

    int32_t intValue = 0;
    double floatValue = 0.0;

    Token() {

    }

    explicit Token(const int kind) : kind(kind) {
    }

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
