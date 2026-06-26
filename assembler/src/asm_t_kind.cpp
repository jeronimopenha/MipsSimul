#include <asm_t_kind.h>

using namespace std;

string asmTokenKindToString(const int k) {
    switch (k) {
        case ASM_IDENT: return "ASM_IDENT";
        case ASM_REG: return "ASM_REG";
        case ASM_FP_REG: return "ASM_FP_REG";
        case ASM_INT_LIT: return "ASM_INT_LIT";
        case ASM_FLOAT_LIT: return "ASM_FLOAT_LIT";
        case ASM_STRING_LIT: return "ASM_STRING_LIT";
        case ASM_DIRECTIVE: return "ASM_DIRECTIVE";
        case ASM_COMMA: return "ASM_COMMA";
        case ASM_COLON: return "ASM_COLON";
        case ASM_L_PAREN: return "ASM_L_PAREN";
        case ASM_R_PAREN: return "ASM_R_PAREN";
        case ASM_NEWLINE: return "ASM_NEWLINE";
        case ASM_EOF: return "ASM_EOF";
        default: return "UNKNOWN";
    }
}
