#include <asm_lexer.h>

using namespace std;

Token AsmLexer::nextToken() {
    Token token = Token(ASM_UNKNOWN);

    //skip white spaces
    skipWhitespaces();
    //skip comments
    skipComments();

    //verifying tokens.
    //first
}

void AsmLexer::skipWhitespaces() {
    while (!eof()) {
        const char c = peek();
        if (c == ' ' || c == '\t' || c == '\r' || c == '\f' || c == '\v') {
            nextChar();
            continue;
        }
        break;
    }
}

void AsmLexer::skipComments() {
    char c = peek();
    if (c != '#') {
        return;
    }
    c = nextChar();
    while (!eof() && c != '\n') {
        c = nextChar();
    }
}


char AsmLexer::peek() const {
    if (eof()) {
        return '\0';
    }
    return source[pos];
}

char AsmLexer::nextChar() {
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

bool AsmLexer::eof() const {
    return pos >= source.size();
}
