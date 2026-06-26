#ifndef ASSEMBLER_LEXER_H
#define ASSEMBLER_LEXER_H

#include <definitions.h>
#include <token.h>
#include <asm_t_kind.h>
#include <utility>

class AsmLexer {
public:
    explicit AsmLexer(std::string src) : source(std::move(src)) {
    }

    virtual ~AsmLexer() = default;

    Token nextToken();

private:
    std::string source;
    size_t pos = 0;
    int line = 1;
    int col = 1;

    void skipWhitespaces();

    void skipComments();

    [[nodiscard]] char peek() const;

    char nextChar();

    [[nodiscard]] bool eof() const;

    Token readRegister();

    Token readDirective();

    Token readIdentifier();

    Token readNumber();

    char peekNext() const;

    Token singleCharToken(AsmTokenKind kind);


    //void lexIdentifierOrDirectiveOrRegister();
    //void lexNumber();
    //void lexString();        // opcional agora
    //void lexPunctuation();

    //void tokenize();


    /*
    [[nodiscard]] char peek() const;

    [[nodiscard]] char peekNext() const;

    char nextChar();

    [[nodiscard]] bool eof() const;



    // hooks for languages
    [[nodiscard]] virtual bool isIdentStart(char c) const;

    [[nodiscard]] virtual bool isIdentChar(char c) const;

    [[nodiscard]] virtual bool isNumberStart(char c) const;

    [[nodiscard]] virtual bool isIntDNumber(char c) const;

    [[nodiscard]] virtual bool isIntXNumber(char c) const;

    static void error(const Token &token, const std::string &msg);

    virtual void skipComments() = 0;

    [[nodiscard]] virtual int getEofKind() const = 0;

    Token makeIdentifierOrKeyword(const std::string &lexeme, int startLine, int startCol);

    Token makeNumberToken(const std::string &lexeme, int startLine, int startCol);

    Token makeOperatorOrPunctToken(std::string first, int startLine, int startCol);

    const std::unordered_map<std::string, AsmTokenKind> keywordMap = {
        {".data", ASM_DIRECTIVE},
        {".text", ASM_DIRECTIVE},
    };*/
};

#endif
