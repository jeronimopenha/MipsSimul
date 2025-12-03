#include <asm_tokens.h>

using namespace std;

string asmTokenKindToString(const AsmTokenKind k) {
    switch (k) {
        case AsmTokenKind::Identifier: return "IDENT";
        case AsmTokenKind::Register: return "REG";
        case AsmTokenKind::Number: return "NUM";
        case AsmTokenKind::Comma: return "COMMA";
        case AsmTokenKind::Colon: return "COLON";
        case AsmTokenKind::LParen: return "LPAR";
        case AsmTokenKind::RParen: return "RPAR";
        case AsmTokenKind::Newline: return "NEWLN";
        case AsmTokenKind::Eof: return "EOF";
        default: return "UNKNOWN";
    }
}
