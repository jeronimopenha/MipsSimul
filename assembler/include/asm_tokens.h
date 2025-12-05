#ifndef ASSEMBLER_TOKENS_H
#define ASSEMBLER_TOKENS_H

#include <definitions.h>


enum AsmTokenKind {
    TOK_IDENT, // opcodes, labels, regs without $
    TOK_REG, // tokens with initial $
    TOK_INT_LIT, // immediate (decimal or hex)
    TOK_COMMA, // ,
    TOK_COLON, // :
    TOK_L_PAREN, // (
    TOK_R_PAREN, // )
    TOK_NEWLINE, // end of line
    TOK_DIRECT_DATA, // .Data
    TOK_DIRECT_TEXT, //.Text
    TOK_EOF, // End of file
    TOK_UNKNOWN // special for errors
};

/*
struct AsmToken {
    AsmTokenKind kind;
    std::string lexeme; // original text
    int line; // line of the file to detect error
};*/

std::string asmTokenKindToString(AsmTokenKind k);

#endif
