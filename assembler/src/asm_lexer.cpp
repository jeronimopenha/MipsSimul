#include <asm_lexer.h>

using namespace std;

void AsmLexer::skipComments() {
    while (!eof()) {
        const char c = peek();
        if (c == '#') {
            while (!eof() && peek() != '\n') {
                nextChar();
            }
        }
        break; // if c is not a comment, so return
    }
}

int AsmLexer::getEofKind() const {
    return TOK_EOF;
}

bool AsmLexer::isIdentStart(const char c) const {
    return Lexer::isIdentStart(c) || c == '$';
}

bool AsmLexer::isIdentChar(const char c) const {
    return Lexer::isIdentChar(c) || c == '$';
}

Token AsmLexer::makeIdentifierOrKeyword(const string &lexeme, const int startLine, const int startCol) {
    static const std::unordered_map<std::string, AsmTokenKind> keywordMap = {
        {".data", TOK_DIRECT_DATA},
        {".text", TOK_DIRECT_TEXT},
    };

    Token t(TOK_IDENT, lexeme, startLine, startCol);

    if (!lexeme.empty() && lexeme[0] == '$') {
        t = Token(TOK_REG, lexeme, startLine, startCol);
    } else {
        auto it = keywordMap.find(lexeme);
        if (it != keywordMap.end()) {
            t = Token(it->second, lexeme, startLine, startCol);
        }
    }

    return t;
}

Token AsmLexer::makeNumberToken(const string &lexeme, const int startLine, const int startCol) {
    Token t(TOK_UNKNOWN, lexeme, startLine, startCol);

    if (lexeme.find('.') == string::npos) {
        for (const auto c: lexeme) {
            if (isdigit(static_cast<unsigned char>(c))) {
                string lex(1, c);
                if (c == '0' && !eof() && (peek() == 'x' || peek() == 'X')) {
                    lex.push_back(nextChar()); // x or X - hex
                    while (!eof() && isIntXNumber(peek())) {
                        lex.push_back(nextChar());
                    }
                } else {
                    // decimal
                    while (!eof() && isIntDNumber(peek())) {
                        lex.push_back(nextChar());
                    }
                }
                t = Token{TOK_INT_LIT, lexeme, startLine, startCol};
                return t;
            }
        }
    }
    return t;
}

Token AsmLexer::makeOperatorOrPunctToken(const string first, int startLine, int startCol) {
    Token t(TOK_UNKNOWN, first, startLine, startCol);

    static const std::unordered_map<std::string, AsmTokenKind> punctMap = {
        {",", TOK_COMMA},
        {":", TOK_COLON},
        {"(", TOK_L_PAREN},
        {")", TOK_R_PAREN},
        {"\n", TOK_NEWLINE}
    };

    auto it = punctMap.find(first);
    if (it != punctMap.end()) {
        t = Token(it->second, (first == "\n" ? "\\n" : first), startLine, startCol);
    }

    return t;
}
