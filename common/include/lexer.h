#ifndef COMMON_LEXER_H
#define COMMON_LEXER_H

#include <definitions.h>
#include <token.h>

#include <utility>

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

    [[nodiscard]] char peek() const;

    [[nodiscard]] char peekNext() const;

    char nextChar();

    [[nodiscard]] bool eof() const;

    void skipWhitespace();

    // hooks for languages
    [[nodiscard]] virtual bool isIdentStart(char c) const;

    [[nodiscard]] virtual bool isIdentChar(char c) const;

    [[nodiscard]] virtual bool isNumberStart(char c) const;

    [[nodiscard]] virtual bool isIntDNumber(char c) const;

    [[nodiscard]] virtual bool isIntXNumber(char c) const;

    virtual void skipComments() = 0;

    [[nodiscard]] virtual int getEofKind() const = 0;

    virtual Token makeIdentifierOrKeyword(const std::string &lexeme, int startLine, int startCol) = 0;

    virtual Token makeNumberToken(const std::string &lexeme, int startLine, int startCol) = 0;

    virtual Token makeOperatorOrPunctToken(std::string first, int startLine, int startCol) = 0;
};

#endif
