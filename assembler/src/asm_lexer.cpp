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
    return isalpha(static_cast<unsigned char>(c)) || c == '_' || c == '.';
}

bool AsmLexer::isIdentChar(const char c) const {
    return isalnum(static_cast<unsigned char>(c)) || c == '_' || c == '.';
}

bool AsmLexer::isNumberStart(const char c) const {
    return isdigit(static_cast<unsigned char>(c));
}

Token AsmLexer::makeIdentifierOrKeyword(const string &lexeme) {
    if (lexeme == ".data") return Token(TOK_DIRECT_DATA, lexeme, line, col);
    if (lexeme == ".text") return Token(TOK_DIRECT_TEXT, lexeme, line, col);
    // etc...
    return Token(TOK_UNKNOWN, lexeme, line, col);
}

Token AsmLexer::makeNumberToken(const string &lexeme) {
    return Token(TOK_UNKNOWN, lexeme, line, col);
}

Token AsmLexer::makeOperatorOrPunctToken(string first) {
    if (first == ",") return Token(TOK_COMMA, first, line, col);
    if (first == ":") return Token(TOK_COLON, first, line, col);
    if (first == "(") return Token(TOK_L_PAREN, first, line, col);
    if (first == ")") return Token(TOK_R_PAREN, first, line, col);
    return Token(TOK_UNKNOWN, first, line, col);
}

Token AsmLexer::nextToken() {
    return Lexer::nextToken();
    /*// register: starts with $
    if (c == '$') {
        string lex = "$";
        while (!eof() && isIdentChar(peek())) {
            lex.push_back(nextChar());
        }
        return AsmToken{AsmTokenKind::TOK_REG, lex, line};
    }

    // identifier: opcode, label, etc.
    if (isIdentStart(c)) {
        string lex(1, c);
        while (!eof() && isIdentChar(peek())) {
            lex.push_back(nextChar());
        }
        return AsmToken{AsmTokenKind::TOK_IDENT, lex, line};
    }

    // número: decimal ou hexa
    if (isdigit((unsigned char) c)) {
        string lex(1, c);
        // se começar com 0x ou 0X, aceita hexa
        if (c == '0' && !eof() && (peek() == 'x' || peek() == 'X')) {
            lex.push_back(nextChar()); // x ou X
            while (!eof() && isxdigit((unsigned char) peek())) {
                lex.push_back(nextChar());
            }
        } else {
            // decimal (pode ter mais dígitos)
            while (!eof() && isdigit((unsigned char) peek())) {
                lex.push_back(nextChar());
            }
        }
        return AsmToken{AsmTokenKind::TOK_INT_LIT, lex, line};
    }

    // se cair aqui, é algo que não tratamos ainda
    cerr << "Caractere inesperado '" << c << "' na linha " << line << "\n";
    return AsmToken{AsmTokenKind::TOK_EOF, "", line};*/
}
