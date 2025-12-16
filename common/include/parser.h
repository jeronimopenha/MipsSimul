#ifndef COMMON_PARSER_H
#define COMMON_PARSER_H

#include <definitions.h>
#include <token.h>

class Parser {
protected:
    const std::vector<Token> &tokens;
    size_t pos = 0;

    [[nodiscard]] const Token &previous() const;

    const Token &previousTwo() const;

    [[nodiscard]] bool check(int kind) const;

    // match com vários kinds de uma vez (igual eu usei no parser de expressões)
    bool match(std::initializer_list<int> kinds);

public:
    explicit Parser(const std::vector<Token> &toks)
        : tokens(toks) {
    }

    [[nodiscard]] const Token &peek() const;

    [[nodiscard]] Token peekNext() const;

    const Token &get();

    bool match(int kind);

    const Token &expect(int kind, const std::string &msg);

    [[nodiscard]] bool eof() const;

    static void error(const Token &token, const std::string &msg);
};


#endif
