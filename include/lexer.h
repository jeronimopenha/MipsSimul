#ifndef MIPSSIMUL_LEXER_H
#define MIPSSIMUL_LEXER_H

#include <definitions.h>

enum class TokenKind {
    Identifier, // opcodes, labels, regs without $
    Register, // tokens with initial $
    Number, // imediates (decimal or hexa)
    Comma, // ,
    Colon, // :
    LParen, // (
    RParen, // )
    Newline, // end of line
    Eof // End of file
};

struct Token {
    TokenKind kind;
    std::string lexeme; // original text
    int line; // line of the file to detect error
};

struct Lexer {
    std::string src;
    size_t pos = 0;
    int line = 1;

    Lexer(const std::string &s);

    [[nodiscard]] bool eof() const;

    [[nodiscard]] char peek() const;

    char get();

    static bool isIdentStart(char c);

    static bool isIdentChar(char c);

    Token next();
};

/*
    bool eof() const {
        return pos >= src.size();
    }

    char peek() const {
        if (eof()) return '\0';
        return src[pos];
    }

    char get() {
        if (eof()) return '\0';
        char c = src[pos++];
        if (c == '\n') line++;
        return c;
    }

    static bool isIdentStart(char c) {
        return isalpha((unsigned char) c) || c == '_' || c == '.';
    }

    static bool isIdentChar(char c) {
        return isalnum((unsigned char) c) || c == '_' || c == '.';
    }

    Token next() {
        // pular espaços e tabs
        while (!eof()) {
            char c = peek();
            if (c == ' ' || c == '\t' || c == '\r') {
                get();
                continue;
            }
            // comentário: # até fim da linha
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

        // registrador: começa com $
        if (c == '$') {
            string lex = "$";
            while (!eof() && isIdentChar(peek())) {
                lex.push_back(get());
            }
            return Token{TokenKind::Register, lex, line};
        }

        // identificador: opcode, label, etc.
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
    }*/


#endif //MIPSSIMUL_LEXER_H
