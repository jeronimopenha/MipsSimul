#include <minic_t_kind.h>


using namespace std;

string minicTokenKindToString(const int k) {
    switch (k) {
        //reserved words
        case TOK_INT: return "INT"; // reserved  - int
        case TOK_FLOAT: return "FLOAT"; // reserved  - float
        case TOK_VOID: return "VOID"; // reserved  - void
        case TOK_IF: return "IF"; // reserved  - if
        case TOK_ELSE: return "ELSE"; // reserved  - else
        case TOK_WHILE: return "WHILE"; // reserved  - while
        case TOK_BREAK: return "BREAK"; // reserved  - break
        case TOK_CONTINUE: return "CONTINUE"; // reserved  - continue
        case TOK_RETURN: return "RETURN"; // reserved  - return
        //identification
        case TOK_IDENT: return "IDENT"; // everything that is not a keyword and other tokens
        //literals
        case TOK_INT_LIT: return "INT_LIT"; // int literals
        case TOK_HEX_LIT: return "HEX_LIT"; // int literals
        case TOK_FLOAT_LIT: return "FLOAT_LIT"; // float literals
        //arithmetic operators
        case TOK_PLUS: return "PLUS"; // +
        case TOK_MINUS: return "MINUS"; // -
        case TOK_STAR: return "STAR"; //*
        case TOK_SLASH: return "SLASH"; // /
        case TOK_PERCENT: return "PERCENT"; //%
        // relational operators
        case TOK_EQ: return "EQUAL"; // ==
        case TOK_NEQ: return "N_EQEUAL"; // !=
        case TOK_LE: return "LESS_EQUAL_THAN"; // <
        case TOK_GE: return "GRATER_EQUAL_THAN"; //>=
        case TOK_LT: return "LESS_THAN"; //<
        case TOK_GT: return "GREATER_THAN"; //>
        //logical operators
        case TOK_AND_AND: return "AND_AND"; // &&
        case TOK_OR_OR: return "OR_OR"; //||
        case TOK_NOT: return "NOT"; //!
        //assign
        case TOK_ASSIGN: return "ASSIGN"; // =
        //pointers
        case TOK_AMP: return "AMP"; // &
        // Delimitations
        case TOK_SEMI: return "SEMICOLON"; // ;
        case TOK_COMMA: return "COMMA"; // ,
        case TOK_LPAREN: return "LPAREN"; // (
        case TOK_RPAREN: return "RPAREN"; // )
        case TOK_LBRACE: return "LBRACE"; // {
        case TOK_RBRACE: return "RBRACE"; // }
        case TOK_LBRACKET: return "LBRACKET"; // [
        case TOK_RBRACKET: return "RBRACKET"; // ]
        //Specials
        case TOK_EOF: return "EOF"; // End of file
        case TOK_NEWLN: return "NEWLN"; // End of file
        default: return "UNKNOWN";
    }
}

string minicTokenKindToSimbol(const int k) {
    switch (k) {
        //reserved words
        case TOK_INT: return "INT"; // reserved  - int
        case TOK_FLOAT: return "FLOAT"; // reserved  - float
        case TOK_VOID: return "VOID"; // reserved  - void
        //identification
        case TOK_IDENT: return "IDENT"; // everything that is not a keyword and other tokens
        //arithmetic operators
        case TOK_PLUS: return "+"; // +
        case TOK_MINUS: return "-"; // -
        case TOK_STAR: return "*"; //*
        case TOK_SLASH: return "/"; // /
        case TOK_PERCENT: return "%"; //%
        // relational operators
        case TOK_EQ: return "=="; // ==
        case TOK_NEQ: return "!="; // !=
        case TOK_LE: return "<="; // <
        case TOK_GE: return ">="; //>=
        case TOK_LT: return "<"; //<
        case TOK_GT: return ">"; //>
        //logical operators
        case TOK_AND_AND: return "&&"; // &&
        case TOK_OR_OR: return "||"; //||
        case TOK_NOT: return "!"; //!
        //assign
        case TOK_ASSIGN: return "="; // =
        //pointers
        case TOK_AMP: return "&"; // &
        // Delimitations
        case TOK_SEMI: return "SEMICOLON"; // ;
        default: return "UNKNOWN";
    }
}
