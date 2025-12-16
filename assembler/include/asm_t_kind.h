#ifndef ASSEMBLER_TOKENS_H
#define ASSEMBLER_TOKENS_H

#include <definitions.h>


enum AsmTokenKind {
    ASM_IDENT, // opcodes, labels, regs without $
    ASM_REG, // tokens with initial $
    ASM_MINUS, // - Simbol used to make negative numbers
    ASM_INT_LIT, // immediate (decimal or hex)
    ASM_COMMA, // ,
    ASM_COLON, // :
    ASM_L_PAREN, // (
    ASM_R_PAREN, // )
    ASM_NEWLINE, // end of line
    ASM_DIRECT_DATA, // .Data
    ASM_DIRECT_TEXT, //.Text
    ASM_EOF, // End of file
    ASM_UNKNOWN // special for errors
};

std::string asmTokenKindToString(AsmTokenKind k);

#endif
