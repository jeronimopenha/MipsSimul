#include <minic_lexer.h>

using namespace std;

void MiniCLexer::skipComments() {
    while (!eof()) {
        char c = peek();
        char c2 = peekNext();
        if (c == '/') {
            if (c2 == '/') {
                nextChar();
                nextChar();
                while (!eof()) {
                    c = peek();
                    if (c == '\n') {
                        break;
                    }
                    nextChar();
                }
            } else if (c2 == '*') {
                nextChar();
                nextChar();
                while (!eof()) {
                    c = peek();
                    c2 = peekNext();

                    if (c == '*' && c2 == '/') {
                        nextChar();
                        nextChar();
                        break;
                    }
                    nextChar();
                }
            } else {
                //It is not a comment
                break;
            }
        } else {
            //it does not begin with '/'
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
    Token t(TOK_IDENT, lexeme, startLine, startCol);

    auto it = keywordMap.find(lexeme);
    if (it != keywordMap.end()) {
        t = Token(it->second, lexeme, startLine, startCol);
    }


    return t;
}

Token MiniCLexer::makeNumberToken(const string &lexeme, const int startLine, const int startCol) {
    const bool hasDot = (lexeme.find('.') != std::string::npos);
    const bool hasE = (lexeme.find('e') != std::string::npos || lexeme.find('E') != std::string::npos);
    const bool hasX = (lexeme.size() >= 2 && lexeme[0] == '0' && (lexeme[1] == 'x' || lexeme[1] == 'X'));
    const bool hasF = (!lexeme.empty() && (lexeme.back() == 'f' || lexeme.back() == 'F'));


    //normal int
    Token t(TOK_INT_LIT, lexeme, startLine, startCol);
    try {
        if (hasX) {
            // int hex
            t.kind = TOK_HEX_LIT;
            t.intValue = static_cast<int32_t>(stoll(lexeme, nullptr, 16));
        } else if (hasDot || hasE || hasF) {
            //float
            t.kind = TOK_FLOAT_LIT;
            std::string floatLex = lexeme;
            if (hasF) {
                floatLex.pop_back(); // remove 'f' or 'F'
            }

            t.floatValue = std::stod(floatLex);
        } else {
            t.kind = TOK_INT_LIT;
            t.intValue = static_cast<int32_t>(stoll(lexeme, nullptr, 10));
        }
    } catch ([[maybe_unused]] const std::exception &e) {
        error(t, "Invalid numeric literal");
    }

    return t;
}

Token MiniCLexer::makeOperatorOrPunctToken(const string first, const int startLine, const int startCol) {
    Token t(TOK_UNKNOWN, first, startLine, startCol);

    if (!eof()) {
        const char c2 = peek();
        const std::string two = first + c2;

        auto it2 = twoCharMap.find(two);
        if (it2 != twoCharMap.end()) {
            nextChar();
            t = Token(it2->second, two, startLine, startCol);
            return t;
        }
    }

    auto it1 = oneCharMap.find(first);
    if (it1 != oneCharMap.end()) {
        t = Token(it1->second, first == "\n" ? "\\n" : first, startLine, startCol);
    }

    return t;
}
