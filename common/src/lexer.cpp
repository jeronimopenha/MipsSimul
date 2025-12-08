#include <lexer.h>

using namespace std;

char Lexer::peek() const {
    if (eof()) {
        return '\0';
    }
    return source[pos];
}

char Lexer::peekNext() const {
    if (eof()) {
        return '\0';
    }
    if (pos + 1 >= source.size()) {
        return '\0';
    }
    return source[pos + 1];
}

char Lexer::nextChar() {
    if (eof()) {
        return '\0';
    }
    const char c = source[pos++];
    col++;
    if (c == '\n') {
        col = 0;
        line++;
    }
    return c;
}

bool Lexer::eof() const {
    return pos >= source.size();
}

void Lexer::skipWhitespace() {
    while (!eof()) {
        const char c = peek();
        if (c == ' ' || c == '\t' || c == '\r' || c == '\f' || c == '\v') {
            nextChar();
            continue;
        }
        break;
    }
}

bool Lexer::isIdentStart(const char c) const {
    return isalpha(static_cast<unsigned char>(c)) || c == '_';
}

bool Lexer::isIdentChar(const char c) const {
    return isalnum(c) || c == '_';
}

bool Lexer::isNumberStart(const char c) const {
    return isdigit(static_cast<unsigned char>(c));
}

bool Lexer::isIntDNumber(const char c) const {
    return isdigit(static_cast<unsigned char>(c));
}

bool Lexer::isIntXNumber(const char c) const {
    return isxdigit(static_cast<unsigned char>(c));
}

Token Lexer::nextToken() {
    // spaces
    skipWhitespace();
    // no comments - implemented on heirs
    skipComments();

    if (eof()) {
        Token t(getEofKind(), "", line, col);
        return t;
    }

    // Identifier or keyword
    if (isIdentStart(peek())) {
        const int startLine = line;
        const int startCol = col;

        string lex;
        while (!eof() && isIdentChar(peek())) {
            const char charTemp = nextChar();
            lex.push_back(charTemp);
        }
        // The heir choose if it is an identifier or a keyword
        return makeIdentifierOrKeyword(lex, startLine, startCol);
    }

    // numbers (int or float)
    if (isNumberStart(peek()) || (peek() == '.' && isdigit(peekNext()))) {
        const int startLine = line;
        const int startCol = col;

        string lex;
        bool hasDot = false;
        bool hasE = false;
        bool hasX = false;

        while (!eof()) {
            const char d = peek();
            //digit or hex digit in case of X or x exists
            if (isNumberStart(d) || (hasX && isxdigit(d))) {
                lex.push_back(nextChar());
            } else if (d == '.' && !hasDot && !hasX && !hasE) {
                hasDot = true;
                lex.push_back(nextChar());
            } else if ((d == 'e' || d == 'E') && !hasE && !hasX) {
                //handle the scientific numbers
                hasE = true;
                lex.push_back(nextChar());

                //with  + ou - or not
                if (!eof()) {
                    const char sign = peek();
                    if (sign == '+' || sign == '-') {
                        lex.push_back(nextChar());
                    }
                }
            } else if ((d == 'x' || d == 'X') && !hasX && !hasDot && !hasE) {
                //hex numbers
                hasX = true;
                lex.push_back(nextChar());
            } else {
                break;
            }
        }
        //handle the F or f char
        if (!eof()) {
            const char d = peek();
            if ((d == 'f' || d == 'F') && (hasDot || hasE)) {
                // só faz sentido 'f' se isso já for um float
                lex.push_back(nextChar());
            }
        }
        // Then heir decides if this is int float or whatever
        return makeNumberToken(lex, startLine, startCol);
    }

    // Operadores / pontuação
    const int startLine = line;
    const int startCol = col;

    string lex;
    lex.push_back(nextChar());
    return makeOperatorOrPunctToken(lex, startLine, startCol);
}
