#include <asm_tokens.h>

using namespace std;

string asmTokenKindToString(const AsmTokenKind k) {
    switch (k) {
        case TOK_IDENT: return "IDENT";
        case TOK_REG: return "REG";
        case TOK_INT_LIT: return "NUM";
        case TOK_COMMA: return "COMMA";
        case TOK_COLON: return "COLON";
        case TOK_L_PAREN: return "LPAR";
        case TOK_R_PAREN: return "RPAR";
        case TOK_NEWLINE: return "NEWLN";
        case TOK_EOF: return "EOF";
        default: return "UNKNOWN";
    }
}
