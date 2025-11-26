#include <asm_tokens.h>

using namespace std;

string tokenKindToString(const TokenKind k) {
    switch (k) {
        case TokenKind::Identifier: return "IDENT";
        case TokenKind::Register: return "REG";
        case TokenKind::Number: return "NUM";
        case TokenKind::Comma: return "COMMA";
        case TokenKind::Colon: return "COLON";
        case TokenKind::LParen: return "LPAR";
        case TokenKind::RParen: return "RPAR";
        case TokenKind::Newline: return "NEWLN";
        case TokenKind::Eof: return "EOF";
        default: return "UNKNOWN";
    }
}
