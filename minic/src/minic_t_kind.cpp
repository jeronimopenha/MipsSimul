#include <minic_t_kind.h>


using namespace std;

string asmTokenKindToString(const MiniCTokenKind k) {
    switch (k) {
        case TOK_INT: return "INT"; // reserved  - int
        case TOK_FLOAT: return "FLOAT"; // reserved  - float
        case TOK_VOID: return "VOID"; // reserved  - void
        case TOK_IF: return "IF"; // reserved  - if
        case TOK_ELSE: return "ELSE"; // reserved  - else
        case TOK_WHILE: return "WHILE"; // reserved  - while
        case TOK_RETURN: return "RETURN"; // reserved  - return
        //identificator
        case TOK_IDENT: return "IDENT"; // everything that is not a keyword and other tokens
        //literals
        case TOK_INT_LIT: return "INT_LIT"; // int literals
        case TOK_FLOAT_LIT: return "FLOAT_LIT"; // float literals
        //arithmetic operators
        case TOK_PLUS: return "PLUS"; // +
        case TOK_MINUS: return "MINUS"; // -
        case TOK_STAR: return "STAR"; //*
        case TOK_SLASH: return "SLASH"; // /
        case TOK_PERCENT: return "PERCENT"; //%
        // relational operators
        case TOK_EQ: return "EQUAL"; // ==
        case TOK_NEQ: return "N_EQEUAL"; // Not Equal
        case TOK_LE: return "LESS_EQUAL_THAN"; // Less or equal than
        case TOK_GE: return "GRATER_EQUAL_THAN"; //Greater or equal than
        case TOK_LT: return "LESS_THAN"; //Less than
        case TOK_GT: return "GREATER_THAN"; //Greater than
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
        case TOK_INVALID: return "INVALID";
        default: return "UNKNOWN";
    }
}
