#ifndef MINIC_T_KIND_H
#define MINIC_T_KIND_H

#include <definitions.h>

enum MiniCTokenKind {
    //keywords
    TOK_INT, // reserved  - int
    TOK_FLOAT, // reserved  - float
    TOK_VOID, // reserved  - void
    TOK_IF, // reserved  - if
    TOK_ELSE, // reserved  - else
    TOK_WHILE, // reserved  - while
    TOK_RETURN, // reserved  - return

    //identificator
    TOK_IDENT, // everything that is not a keyword and other tokens

    //literals
    TOK_INT_LIT, // int literals
    TOK_HEX_LIT, // int literals
    TOK_FLOAT_LIT, // float literals

    //arithmetic operators
    TOK_PLUS, // +
    TOK_MINUS, // -
    TOK_STAR, //*
    TOK_SLASH, // /
    TOK_PERCENT, //%

    // relational operators
    TOK_EQ, // ==
    TOK_NEQ, // !=
    TOK_LE, // <=
    TOK_GE, //>=
    TOK_LT, //<
    TOK_GT, //>

    //logical operators
    TOK_AND_AND, // &&
    TOK_OR_OR, //||
    TOK_NOT, //!

    //assign
    TOK_ASSIGN, // =

    //pointers
    TOK_AMP, // &

    // Delimitations
    TOK_SEMI, // ;
    TOK_COMMA, // ,
    TOK_LPAREN, // (
    TOK_RPAREN, // )
    TOK_LBRACE, // {
    TOK_RBRACE, // }
    TOK_LBRACKET, // [
    TOK_RBRACKET, // ]

    //Specials
    TOK_NEWLN, //New Line
    TOK_EOF, // End of file
    TOK_UNKNOWN
};

std::string minicTokenKindToString( MiniCTokenKind k);

#endif //MIPSSIMUL_MINIC_T_KIND_H
