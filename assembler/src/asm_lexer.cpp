#include <asm_lexer.h>

using namespace std;

Token AsmLexer::nextToken() {
    //skip white spaces
    skipWhitespaces();
    //skip comments
    skipComments();

    Token token;

    //verifying tokens.
    if (eof()) {
        token = Token(ASM_EOF, "", line, col);
        return token;
    }

    const char c = peek();

    if (c == '$') {
        token = readRegister();
        return token;
    }

    if (c == '.')
        return readDirective();

    if (isalpha(c) || c == '_')
        return readIdentifier();

    if (c == '-' || isdigit(c))
        return readNumber();

    switch (c) {
        case '\n':
            return singleCharToken(ASM_NEWLINE);
        case ',':
            return singleCharToken(ASM_COMMA);
        case ':':
            return singleCharToken(ASM_COLON);
        case '(':
            return singleCharToken(ASM_L_PAREN);
        case ')':
            return singleCharToken(ASM_R_PAREN);
        default:
            nextChar();
            token = Token(ASM_UNKNOWN, string(1, c), line, col);
            return token;
    }
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

Token AsmLexer::readRegister() {
    const int startLine = line;
    const int startCol = col;

    string lexeme;

    lexeme.push_back(nextChar()); // consome '$'

    char c = peek();
    while (!eof() && (isalnum(c) || c == '_')) {
        lexeme.push_back(nextChar());
        c = peek();
    }

    if (lexeme.size() >= 3 && lexeme[1] == 'f' && isdigit(lexeme[2])) {
        Token token = Token(ASM_FP_REG, lexeme, startLine, startCol);
        return token;
    }
    Token token = Token(ASM_REG, lexeme, startLine, startCol);
    return token;
}

Token AsmLexer::readDirective() {
    const int startLine = line;
    const int startCol = col;

    string lexeme;
    lexeme.push_back(nextChar()); // consome '.'

    char c = peek();
    while (!eof() && (isalnum(c) || c == '_')) {
        lexeme.push_back(nextChar());
        c = peek();
    }

    if (lexeme.size() == 1) {
        Token token = Token(ASM_UNKNOWN, lexeme, startLine, startCol);
        return token;
    }

    Token token = Token(ASM_DIRECTIVE, lexeme, startLine, startCol);
    return token;
}

Token AsmLexer::readIdentifier() {
    const int startLine = line;
    const int startCol = col;

    string lexeme;

    char c = peek();
    while (!eof() && (isalnum(c) || c == '_' || c == '.')) {
        lexeme.push_back(nextChar());
        c = peek();
    }


    Token token = Token(ASM_IDENT, lexeme, startLine, startCol);
    return token;
}

Token AsmLexer::readNumber() {
    const int startLine = line;
    const int startCol = col;

    string lexeme;
    bool hasDot = false;

    if (peek() == '-') {
        lexeme.push_back(nextChar());
    }

    if (!isdigit(peek())) {
        return Token(ASM_UNKNOWN, lexeme, startLine, startCol);
    }

    if (peek() == '0' && (peekNext() == 'x' || peekNext() == 'X')) {
        lexeme.push_back(nextChar());
        lexeme.push_back(nextChar());

        if (!isxdigit(peek())) {
            return Token(ASM_UNKNOWN, lexeme, startLine, startCol);
        }

        while (!eof() && isxdigit(peek())) {
            lexeme.push_back(nextChar());
        }

        return Token(ASM_INT_LIT, lexeme, startLine, startCol);
    }

    while (!eof() && (isdigit(peek()) || peek() == '.')) {
        if (peek() == '.') {
            if (hasDot) {
                return Token(ASM_UNKNOWN, lexeme, startLine, startCol);
            }
            hasDot = true;
        }

        lexeme.push_back(nextChar());
    }

    return Token(hasDot ? ASM_FLOAT_LIT : ASM_INT_LIT, lexeme, startLine, startCol);
}

char AsmLexer::peekNext() const {
    if (eof()) {
        return '\0';
    }
    if (pos + 1 >= source.size()) {
        return '\0';
    }
    return source[pos + 1];
}

Token AsmLexer::singleCharToken(const AsmTokenKind kind) {
    const int startLine = line;
    const int startCol = col;
    string lexeme = "";
    char c = peek();
    if (c == '\n') {
        lexeme = "\\n";
    } else {
        lexeme.push_back(c);
    }
    nextChar();
    Token token = Token(kind, lexeme, startLine, startCol);
    return token;
}
