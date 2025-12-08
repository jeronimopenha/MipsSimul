#include <asm_t_kind.h>

using namespace std;

string asmTokenKindToString(const AsmTokenKind k) {
    switch (k) {
        case ASM_IDENT: return "IDENT";
        case ASM_REG: return "REG";
        case ASM_INT_LIT: return "NUM";
        case ASM_COMMA: return "COMMA";
        case ASM_COLON: return "COLON";
        case ASM_L_PAREN: return "LPAR";
        case ASM_R_PAREN: return "RPAR";
        case ASM_NEWLINE: return "NEWLN";
        case ASM_EOF: return "EOF";
        default: return "UNKNOWN";
    }
}
