#ifndef COMMON_PARSER_H
#define COMMON_PARSER_H

#include <definitions.h>
#include <token.h>

class Parser {
protected:
    const std::vector<Token> &tokens;
    size_t pos = 0;

    const Token &previous() const;

    const Token &previousTwo() const;

    bool check(int kind);

    // match with more than one Token Kinds
    bool match(std::initializer_list<int> kinds);

    const Token peekNext();

    const Token &get();


    const Token &expect(int kind, const std::string &msg);

    bool eof() const;

    static void error(const Token &token, const std::string &msg);

public:
    virtual ~Parser() = default;

    explicit Parser(const std::vector<Token> &toks)
        : tokens(toks) {
    }

    bool match(int kind);

    const Token &peek();
};


#endif
