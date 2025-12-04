#ifndef COMMON_LEXER_H
#define COMMON_LEXER_H

#include <definitions.h>
#include <token.h>

class Lexer {
public:
    explicit Lexer(std::string src) : source(std::move(src)) {
    }

    virtual ~Lexer() = default;

    Token nextToken();

protected:
    std::string source;
    size_t pos = 0;
    int line = 1;
    int col = 1;

    char peek() const;

    char nextChar();

    bool eof() const;

    void skipWhitespace();


    // hooks for languages
    virtual bool isIdentStart(char c) const = 0;

    virtual bool isIdentChar(char c) const = 0;

    virtual bool isNumberStart(char c) const = 0;

    virtual void skipComments() =0;

    virtual int getEofKind() const = 0;

    virtual Token makeIdentifierOrKeyword(const std::string &lexeme) = 0;

    virtual Token makeNumberToken(const std::string &lexeme) = 0;

    virtual Token makeOperatorOrPunctToken(std::string first) = 0;
};

#endif
