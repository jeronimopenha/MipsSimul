#ifndef ASSEMBLER_LEXER_H
#define ASSEMBLER_LEXER_H

#include <definitions.h>
#include <lexer.h>
#include <token.h>
#include <asm_tokens.h>

class AsmLexer : public Lexer {
public:
    explicit AsmLexer(std::string s);

private:
    void skipComments() override;

    int getEofKind() const override;

    bool isIdentStart(char c) const override;

    bool isIdentChar(char c) const override;

    bool isNumberStart(char c) const override;

    Token makeIdentifierOrKeyword(const std::string &lexeme) override;

    Token makeNumberToken(const std::string &lexeme) override;

    Token makeOperatorOrPunctToken(std::string first) override;
};

#endif
