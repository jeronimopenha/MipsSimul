#include <lexer.h>

using namespace std;

Lexer::Lexer(const string &s) : src(s) {
}

bool Lexer::eof() const {
    return pos >= src.size();
}

char Lexer::peek() const {
    if (eof())
        return '\0';
    return src[pos];
}

char Lexer::get() {
    if (eof())
        return '\0';
    const char c = src[pos++];
    if (c == '\n')
        line++;
    return c;
}

bool Lexer::isIdentStart(char c) {
    return isalpha((unsigned char) c) || c == '_' || c == '.';
}

bool Lexer::isIdentChar(char c) {
    return isalnum((unsigned char) c) || c == '_' || c == '.';
}

Token Lexer::next() {
    // ignire spaces and tabs
    while (!eof()) {
        const char c = peek();
        if (c == ' ' || c == '\t' || c == '\r') {
            get();
            continue;
        }
        // If comment (#) goes until the end of the line
        if (c == '#') {
            while (!eof() && get() != '\n') {
            }
            return Token{TokenKind::Newline, "\\n", line};
        }
        break;
    }

    if (eof()) {
        return Token{TokenKind::Eof, "", line};
    }

    char c = get();

    // quebra de linha
    if (c == '\n') {
        return Token{TokenKind::Newline, "\\n", line - 1};
    }

    // pontuações simples
    if (c == ',')
        return Token{TokenKind::Comma, ",", line};
    if (c == ':')
        return Token{TokenKind::Colon, ":", line};
    if (c == '(')
        return Token{TokenKind::LParen, "(", line};
    if (c == ')')
        return Token{TokenKind::RParen, ")", line};

    // register: starts with $
    if (c == '$') {
        string lex = "$";
        while (!eof() && isIdentChar(peek())) {
            lex.push_back(get());
        }
        return Token{TokenKind::Register, lex, line};
    }

    // identifier: opcode, label, etc.
    if (isIdentStart(c)) {
        string lex(1, c);
        while (!eof() && isIdentChar(peek())) {
            lex.push_back(get());
        }
        return Token{TokenKind::Identifier, lex, line};
    }

    // número: decimal ou hexa
    if (isdigit((unsigned char) c)) {
        string lex(1, c);
        // se começar com 0x ou 0X, aceita hexa
        if (c == '0' && !eof() && (peek() == 'x' || peek() == 'X')) {
            lex.push_back(get()); // x ou X
            while (!eof() && isxdigit((unsigned char) peek())) {
                lex.push_back(get());
            }
        } else {
            // decimal (pode ter mais dígitos)
            while (!eof() && isdigit((unsigned char) peek())) {
                lex.push_back(get());
            }
        }
        return Token{TokenKind::Number, lex, line};
    }

    // se cair aqui, é algo que não tratamos ainda
    cerr << "Caractere inesperado '" << c << "' na linha " << line << "\n";
    return Token{TokenKind::Eof, "", line};
}
