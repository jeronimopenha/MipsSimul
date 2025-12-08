#include <minic_lexer.h>

using namespace std;

void MiniCLexer::skipComments() {
    while (!eof()) {
        char c = peek();
        if (c == '/') {
            if (eof()) {
                return;
            }
            nextChar();
            c = peek();
            if (c == '/') {
                while (!eof()) {
                    c = peek();
                    if (c == '\n') {
                        break;
                    }
                    nextChar();
                }
            } else if (c == '*') {
                char c2 = c;
                nextChar();
                while (!eof()) {
                    c = peek();

                    if (c2 == '*' && c == '/') {
                        nextChar();
                        break;
                    }
                    c2 = nextChar();
                }
            }
        } else {
            break;
        }
    }
}

int MiniCLexer::getEofKind() const {
    return TOK_EOF;
}

bool MiniCLexer::isIdentStart(const char c) const {
    return Lexer::isIdentStart(c) || c == '$';
}

bool MiniCLexer::isIdentChar(const char c) const {
    return Lexer::isIdentChar(c) || c == '$';
}

Token MiniCLexer::makeIdentifierOrKeyword(const string &lexeme, const int startLine, const int startCol) {
    static const std::unordered_map<std::string, MiniCTokenKind> keywordMap = {
        {"int", TOK_INT},
        {"float", TOK_FLOAT},
        {"void", TOK_VOID},
        {"if", TOK_IF},
        {"else", TOK_ELSE},
        {"while", TOK_WHILE},
        {"return", TOK_RETURN}
    };

    Token t(TOK_IDENT, lexeme, startLine, startCol);

    auto it = keywordMap.find(lexeme);
    if (it != keywordMap.end()) {
        t = Token(it->second, lexeme, startLine, startCol);
    }


    return t;
}

Token MiniCLexer::makeNumberToken(const string &lexeme, int startLine, int startCol) {
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

Token MiniCLexer::makeOperatorOrPunctToken(const string first, int startLine, int startCol) {
    Token t(TOK_UNKNOWN, first, startLine, startCol);
    /*//arithmetic operators
        case TOK_PLUS: return "PLUS"; // +
        case TOK_MINUS: return "MINUS"; // -
        case TOK_STAR: return "STAR"; //*
        case TOK_SLASH: return "SLASH"; // /
        case TOK_PERCENT: return "PERCENT"; //%
        // relational operators
        case TOK_EQ: return "EQUAL"; // ==
        case TOK_NEQ: return "N_EQEUAL"; // Not Equal
        case TOK_LE: return "LESS_EQUAL_THAN"; // Less or equal than
        case TOK_GE: return "GRATER_EQUAL_THAN"; //Greater or equal than
        case TOK_LT: return "LESS_THAN"; //Less than
        case TOK_GT: return "GREATER_THAN"; //Greater than
        //logical operators
        case TOK_AND_AND: return "AND_AND"; // &&
        case TOK_OR_OR: return "OR_OR"; //||
        case TOK_NOT: return "NOT"; //!
        //assign
        case TOK_ASSIGN: return "ASSIGN"; // =
        //pointers
        case TOK_AMP: return "AMP"; // &
        // Delimitations
        case TOK_SEMI: return "SEMICOLON"; // ;
        case TOK_COMMA: return "COMMA"; // ,
        case TOK_LPAREN: return "LPAREN"; // (
        case TOK_RPAREN: return "RPAREN"; // )
        case TOK_LBRACE: return "LBRACE"; // {
        case TOK_RBRACE: return "RBRACE"; // }
        case TOK_LBRACKET: return "LBRACKET"; // [
        case TOK_RBRACKET: return "RBRACKET"; // ]*/
    static const std::unordered_map<std::string, MiniCTokenKind> punctMap = {
        {"+", TOK_PLUS},
        {"-", TOK_MINUS},
        {"/", TOK_SLASH},
        {"*", TOK_STAR},
        {"%", TOK_PERCENT},

        {",", TOK_COMMA}
    };

    auto it = punctMap.find(first);
    if (it != punctMap.end()) {
        t = Token(it->second, (first == "\n" ? "\\n" : first), startLine, startCol);
    }
    return t;
}
