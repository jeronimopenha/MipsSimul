#ifndef MINIC_LEXER_H
#define MINIC_LEXER_H

#include <definitions.h>
#include <lexer.h>
#include <token.h>
#include <minic_t_kind.h>

class MiniCLexer final : public Lexer {
public:
    explicit MiniCLexer(const std::string &s) : Lexer(s) {
    }

private:
    const std::unordered_map<std::string, MiniCTokenKind> oneCharMap = {
        //Arithmetic
        {"+", TOK_PLUS},
        {"-", TOK_MINUS},
        {"/", TOK_SLASH},
        {"*", TOK_STAR},
        {"%", TOK_PERCENT},

        //Assign
        {"=", TOK_ASSIGN},

        //Pointers
        {"&", TOK_AMP},

        //Delimitations
        {";", TOK_SEMI},
        {",", TOK_COMMA},
        {"(", TOK_LPAREN},
        {")", TOK_RPAREN},
        {"{", TOK_LBRACE},
        {"}", TOK_RBRACE},
        {"[", TOK_LBRACKET},
        {"]", TOK_RBRACKET},

        //logical operators
        {"!", TOK_NOT},

        // relational operators
        {"<", TOK_LT},
        {">", TOK_GT},

        //new line
        {"\n", TOK_NEWLN}
    };

    const std::unordered_map<std::string, MiniCTokenKind> twoCharMap = {
        // relational operators
        {"==", TOK_EQ},
        {"!=", TOK_NEQ},
        {"<=", TOK_LE},
        {">=", TOK_GE},

        //logical operators
        {"&&", TOK_AND_AND},
        {"||", TOK_OR_OR},
    };

    const std::unordered_map<std::string, MiniCTokenKind> keywordMap = {
        {"int", TOK_INT},
        {"float", TOK_FLOAT},
        {"void", TOK_VOID},
        {"if", TOK_IF},
        {"else", TOK_ELSE},
        {"while", TOK_WHILE},
        {"return", TOK_RETURN}
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
