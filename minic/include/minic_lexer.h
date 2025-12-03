#ifndef MINIC_LEXER_H
#define MINIC_LEXER_H

#include <common.h>
#include <asm_tokens.h>

class AsmLexer {
public:
    explicit AsmLexer(std::string s);

    MiniCToken next();

private:
    std::string src;
    size_t pos = 0;
    int line = 1;

    [[nodiscard]] bool eof() const;

    [[nodiscard]] char peek() const;

    char get();

    static bool isIdentStart(char c);

    static bool isIdentChar(char c);
};

#endif
