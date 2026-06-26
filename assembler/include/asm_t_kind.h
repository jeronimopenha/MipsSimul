#ifndef ASSEMBLER_TOKENS_H
#define ASSEMBLER_TOKENS_H

#include <definitions.h>

enum AsmTokenKind {
    ASM_IDENT, // add, main, loop, x, y
    ASM_REG, // $t0, $zero, $s1
    ASM_FP_REG, // $f0, $f2, $f31

    ASM_INT_LIT, // 10, -5, 0x1000
    ASM_FLOAT_LIT, // 2.5, -3.14
    ASM_STRING_LIT, // "texto"

    ASM_DIRECTIVE, // .text, .data, .word, .float, .space

    ASM_COMMA, // ,
    ASM_COLON, // :
    ASM_L_PAREN, // (
    ASM_R_PAREN, // )

    ASM_NEWLINE,
    ASM_EOF,
    ASM_UNKNOWN
};


std::string asmTokenKindToString(int k);

#endif
