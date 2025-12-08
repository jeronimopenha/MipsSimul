#ifndef ASSEMBLER_LEXER_H
#define ASSEMBLER_LEXER_H

#include <definitions.h>
#include <lexer.h>
#include <token.h>
#include <asm_t_kind.h>

class AsmLexer final : public Lexer {
public:
    explicit AsmLexer(const std::string &s) : Lexer(s) {
    }

private:
    void skipComments() override;

    [[nodiscard]] int getEofKind() const override;

    [[nodiscard]] bool isIdentStart(char c) const override;

    [[nodiscard]] bool isIdentChar(char c) const override;

    Token makeIdentifierOrKeyword(const std::string &lexeme, int startLine, int startCol) override;

    Token makeNumberToken(const std::string &lexeme, int startLine, int startCol) override;

    Token makeOperatorOrPunctToken(std::string first, int startLine, int startCol) override;
};

#endif
