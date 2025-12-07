#ifndef COMMON_PARSER_H
#define COMMON_PARSER_H

#include <definitions.h>
#include <token.h>

class Parser {
protected:
    const std::vector<Token> &tokens;
    size_t pos = 0;

public:
    explicit Parser(const std::vector<Token> &toks)
        : tokens(toks) {
    }

    [[nodiscard]] const Token &peek() const;

    const Token &get();

    bool match(int kind);

    const Token &expect(int kind, const std::string &msg);

    [[nodiscard]] bool eof() const;
};


#endif
