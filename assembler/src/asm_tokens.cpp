#include <asm_tokens.h>

using namespace std;

string asmTokenKindToString(const AsmTokenKind k) {
    switch (k) {
        case AsmTokenKind::TOK_IDENT: return "IDENT";
        case AsmTokenKind::TOK_REG: return "REG";
        case AsmTokenKind::TOK_INT_LIT: return "NUM";
        case AsmTokenKind::TOK_COMMA: return "COMMA";
        case AsmTokenKind::TOK_COLON: return "COLON";
        case AsmTokenKind::TOK_L_PAREN: return "LPAR";
        case AsmTokenKind::TOK_R_PAREN: return "RPAR";
        case AsmTokenKind::TOK_NEWLINE: return "NEWLN";
        case AsmTokenKind::TOK_EOF: return "EOF";
        default: return "UNKNOWN";
    }
}
