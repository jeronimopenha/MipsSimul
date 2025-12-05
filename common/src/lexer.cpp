#include <lexer.h>

using namespace std;

char Lexer::peek() const {
    if (eof()) {
        return '\0';
    }
    return source[pos];
}

char Lexer::nextChar() {
    if (eof()) {
        return '\0';
    }
    const char c = source[pos++];
    if (c == '\n') {
        line++;
    }
    return c;
}

bool Lexer::eof() const {
    return pos >= source.size();
}

/*bool Lexer::isIdentStart(const char c) const{
    return isalpha(static_cast<unsigned char>(c)) || c == '_' ;
}

bool Lexer::isIdentChar(const char c) const{
    return isalnum(static_cast<unsigned char>(c)) || c == '_' ;
}

bool Lexer::isNumberStart(const char c) const {
    return isdigit(static_cast<unsigned char>(c)) || c == '.';
}*/

void Lexer::skipWhitespace() {
    while (!eof()) {
        const char c = peek();
        if (c == ' ' || c == '\t' || c == '\r') {
            nextChar();
            continue;
        }
        break;
    }
}

Token Lexer::nextToken() {
    // spaces
    skipWhitespace();
    // no comments - implemented on heirs
    skipComments();

    if (eof()) {
        return Token(getEofKind(), "", line, col);
    }

    // Identifier or keyword
    if (isIdentStart(peek())) {
        string lex;
        while (!eof() && isIdentChar(peek())) {
            const char charTemp = nextChar();
            lex.push_back(charTemp);
        }
        // The heir choose if it is an identifier or a keyword
        return makeIdentifierOrKeyword(lex);
    }

    // numbers (int or float)
    if (isNumberStart(peek())) {
        string lex;
        bool hasDot = false;

        while (!eof()) {
            const char d = peek();
            if (isdigit(static_cast<unsigned char>(d))) {
                lex.push_back(nextChar());
            } else if (d == '.' && !hasDot) {
                hasDot = true;
                lex.push_back(nextChar());
            } else {
                break;
            }
        }
        // The heir decides if this is int float or whatever
        return makeNumberToken(lex);
    }

    // Operadores / pontuação
    string lex;
    lex.push_back(nextChar());
    return makeOperatorOrPunctToken(lex);
}
