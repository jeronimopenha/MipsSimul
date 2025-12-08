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
    const std::unordered_map<std::string, AsmTokenKind> punctMap = {
        {",", ASM_COMMA},
        {":", ASM_COLON},
        {"(", ASM_L_PAREN},
        {")", ASM_R_PAREN},
        {"\n", ASM_NEWLINE}
    };

    const std::unordered_map<std::string, AsmTokenKind> keywordMap = {
        {".data", ASM_DIRECT_DATA},
        {".text", ASM_DIRECT_TEXT},
    };

    void skipComments() override;

    [[nodiscard]] int getEofKind() const override;

    [[nodiscard]] bool isIdentStart(char c) const override;

    [[nodiscard]] bool isIdentChar(char c) const override;

    Token makeIdentifierOrKeyword(const std::string &lexeme, int startLine, int startCol) override;

    Token makeNumberToken(const std::string &lexeme, int startLine, int startCol) override;

    Token makeOperatorOrPunctToken(std::string first, int startLine, int startCol) override;
};

#endif
