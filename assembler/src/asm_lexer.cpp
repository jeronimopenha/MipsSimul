#include <asm_lexer.h>

using namespace std;

AsmLexer::AsmLexer(string s) : Lexer(move(s)) {
}

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
    return isalpha(static_cast<unsigned char>(c)) || c == '_' || c == '.' || c == '$';
}

bool AsmLexer::isIdentChar(const char c) const {
    return isalnum(static_cast<unsigned int>(c)) || c == '_' || c == '.' || c == '$';
}

bool AsmLexer::isNumberStart(const char c) const {
    return isdigit(static_cast<unsigned char>(c));
}

Token AsmLexer::makeIdentifierOrKeyword(const string &lexeme) {
    if (lexeme == ".data") return Token(TOK_DIRECT_DATA, lexeme, line, col);
    if (lexeme == ".text") return Token(TOK_DIRECT_TEXT, lexeme, line, col);
    if (lexeme.at(0) == '$')return Token(TOK_REG, lexeme, line, col);
    return Token(TOK_IDENT, lexeme, line, col);
}

Token AsmLexer::makeNumberToken(const string &lexeme) {
    if (lexeme.find('.') != std::string::npos) {
        Token t = Token(TOK_UNKNOWN, lexeme, line, col);
        return t;
    }
    for (const auto c: lexeme) {
        if (isdigit(static_cast<unsigned char>(c))) {
            string lex(1, c);
            if (c == '0' && !eof() && (peek() == 'x' || peek() == 'X')) {
                lex.push_back(nextChar()); // x or X - hex
                while (!eof() && isxdigit(static_cast<unsigned char>(peek()))) {
                    lex.push_back(nextChar());
                }
            } else {
                // decimal
                while (!eof() && isdigit(static_cast<unsigned char>(peek()))) {
                    lex.push_back(nextChar());
                }
            }
            Token t = Token{TOK_INT_LIT, lexeme, line, col};
            return t;
        }
    }
    Token t = Token(TOK_UNKNOWN, lexeme, line, col);
    return t;
}

Token AsmLexer::makeOperatorOrPunctToken(string first) {
    if (first == ",") return Token(TOK_COMMA, first, line, col);
    if (first == ":") return Token(TOK_COLON, first, line, col);
    if (first == "(") return Token(TOK_L_PAREN, first, line, col);
    if (first == ")") return Token(TOK_R_PAREN, first, line, col);
    if (first == "\n") return Token(TOK_NEWLINE, "\\n", line, col);
    return Token(TOK_UNKNOWN, first, line, col);
}
