#ifndef MINIC_PARSER_H
#define MINIC_PARSER_H

#include <definitions.h>
#include <asm_lexer.h>
#include <parser.h>
#include <minic_ast.h>
#include <minic_t_kind.h>


class MiniCParser : public Parser {
public:
    explicit MiniCParser(const std::vector<Token> &toks)
        : Parser(toks) {
    }

    // FIXME CHANGE ON FUTURE
    std::unique_ptr<Expr> parseExpr();

private:
    std::unique_ptr<Expr> parseAssignExpr();

    std::unique_ptr<Expr> parseLogicalOrExpr();

    std::unique_ptr<Expr> parseLogicalAndExpr();

    std::unique_ptr<Expr> parseEqualityExpr();

    std::unique_ptr<Expr> parseRelationalExpr();

    std::unique_ptr<Expr> parseAdditiveExpr();

    std::unique_ptr<Expr> parseMultiplicativeExpr();

    std::unique_ptr<Expr> parseUnaryExpr();

    std::unique_ptr<Expr> parsePostfixExpr();

    std::unique_ptr<Expr> parsePrimaryExpr();

    [[noreturn]] void error(const Token &token, const std::string &msg);

    void skipNewLines();
};


/* - Grammar

program        ::= { external_decl } EOF ;

external_decl  ::= function_def
                 | function_decl
                 | global_var_decl ;

global_var_decl ::= type_spec declarator_list SEMICOLON ;

type_spec      ::= INT
                 | FLOAT
                 | VOID ;

declarator_list ::= declarator { COMMA declarator } ;

declarator     ::= pointer_opt IDENT array_suffix_opt init_opt ;

pointer_opt    ::= { STAR } ;           //  *, **, etc

array_suffix_opt ::=                      // simple array like v[4]
                   | LBRACKET INT_LIT RBRACKET ;

init_opt       ::=
                 | ASSIGN expr ;        // initialization: int x = 42;

function_decl ::= type_spec IDENT LPAREN param_list_opt RPAREN SEMICOLON ;

function_def  ::= type_spec IDENT LPAREN param_list_opt RPAREN compound_stmt ;

param_list_opt ::=
                 | param_list ;

param_list    ::= param_decl { COMMA param_decl } ;

param_decl    ::= type_spec pointer_opt IDENT ;

stmt          ::= compound_stmt
                 | if_stmt
                 | while_stmt
                 | return_stmt
                 | expr_stmt
                 | SEMICOLON ;     // linha vazia opcional, se quiser

compound_stmt ::= LBRACE { local_decl | stmt } RBRACE ;

local_decl    ::= type_spec declarator_list SEMICOLON ;
                 // ex: int i;  int x, y;

if_stmt       ::= IF LPAREN expr RPAREN stmt
                   [ ELSE stmt ] ;

while_stmt    ::= WHILE LPAREN expr RPAREN stmt ;

return_stmt   ::= RETURN expr_opt SEMICOLON ;

expr_opt      ::=
                 | expr ;

expr_stmt     ::= expr SEMICOLON ;

expr          ::= assign_expr ;

assign_expr   ::= logical_or_expr
                 | unary_expr ASSIGN assign_expr ;

logical_or_expr  ::= logical_and_expr
                   | logical_or_expr OR_OR logical_and_expr ;

logical_and_expr ::= equality_expr
                   | logical_and_expr AND_AND equality_expr ;

equality_expr  ::= relational_expr
                 | equality_expr EQUAL relational_expr
                 | equality_expr N_EQEUAL relational_expr ;

relational_expr ::= additive_expr
                 | relational_expr LESS_THAN additive_expr
                 | relational_expr LESS_EQUAL_THAN additive_expr
                 | relational_expr GREATER_THAN additive_expr
                 | relational_expr GRATER_EQUAL_THAN additive_expr ;

additive_expr ::= multiplicative_expr
                | additive_expr PLUS multiplicative_expr
                | additive_expr MINUS multiplicative_expr ;

multiplicative_expr ::= unary_expr
                      | multiplicative_expr STAR unary_expr
                      | multiplicative_expr SLASH unary_expr
                      | multiplicative_expr PERCENT unary_expr ;

unary_expr    ::= postfix_expr
                | PLUS unary_expr
                | MINUS unary_expr
                | NOT unary_expr
                | STAR unary_expr     // unreferenced: *p
                | AMP unary_expr ;    // address: &v[0]

postfix_expr  ::= primary_expr postfix_tail ;

postfix_tail  ::=
                | LBRACKET expr RBRACKET postfix_tail      // v[i]
                | LPAREN arg_list_opt RPAREN postfix_tail ;// function caller

arg_list_opt  ::=
                | arg_list ;

arg_list      ::= expr { COMMA expr } ;

primary_expr  ::= IDENT
                | literal
                | LPAREN expr RPAREN ;

literal       ::= INT_LIT
                | HEX_LIT
                | FLOAT_LIT ;


*/

#endif
