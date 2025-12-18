#ifndef COMMON_PARSER_H
#define COMMON_PARSER_H

#include <definitions.h>
#include <token.h>

class Parser {
protected:
    const std::vector<Token> &tokens;
    size_t pos = 0;

    [[nodiscard]] const Token &previous() const;

    [[nodiscard]] const Token &previousTwo() const;

    [[nodiscard]] bool check(int kind) const;

    // match with more than one Token Kinds
    bool match(std::initializer_list<int> kinds);

    [[nodiscard]] Token peekNext() const;

    const Token &get();


    const Token &expect(int kind, const std::string &msg);

    [[nodiscard]] bool eof() const;

    static void error(const Token &token, const std::string &msg);

public:
    virtual ~Parser() = default;

    explicit Parser(const std::vector<Token> &tokens)
        : tokens(tokens) {
    }

    bool match(int kind);

    [[nodiscard]] const Token &peek() const;
};


#endif
