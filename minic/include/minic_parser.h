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
    std::unique_ptr<ExprNode> parseExpr();

    std::unique_ptr<ExprNode> parseAssign();

    std::unique_ptr<ExprNode> parseLValue();

    std::unique_ptr<ExprNode> parseOR();

    std::unique_ptr<ExprNode> parseAnd();

    std::unique_ptr<ExprNode> parseEql();

    std::unique_ptr<ExprNode> parseRel();

    std::unique_ptr<ExprNode> parseAdd();

    std::unique_ptr<ExprNode> parseMul();

    std::unique_ptr<ExprNode> parseUnary();

    std::unique_ptr<ExprNode> parsePrimary();

private:
    void skipNewLines();
};

/*
 * Precedency order
 * postfix (call, indexing)
 * unary (- ! * &)
 * mul (* / %)
 * add (+ -)
 * rel (< <= > >=)
 * eq (== !=)
 * and (&&)
 * or (||)
 * assign (=) (weak and rhs)
 */

/*
 * New grammar
 *
 * program ::= { external_decl } TOK_EOF
 *
 * expr := or_expr TOK_SEMI
 *
 * assign_expr := or_expr | lvalue TOP_EQ assign_expr
 *
 * lvalue ::= TOK_IDENT
 *            | '*' unary_expr
 *            | lvalue '[' expr ']'
 *
 * or_expr ::= and_expr { (TOK_OR_OR ) and_expr }
 *
 * and_expr ::= eql_expr { (TOK_AND_AND ) eql_expr }
 *
 * eql_expr ::= rel_expr { (TOK_EQ | TOK_NEQ ) rel_expr }
 *
 * rel_expr ::= add_expr { (TOK_LT | TOK_LE | TOK_GT | TOK_GE) add_expr }
 *
 * add_expr ::= mul_expr { (TOK_PLUS | TOK_MINUS) mul_expr }
 *
 * mul_expr ::= unary_expr { (TOK_STAR | TOK_SLASH | TOK_PERCENT) unary_expr }
 *
 * unary_expr ::= unary_op unary_expr
 *                | primary_expr
 *
 * unary_op ::= TOK_MINUS
 *              | TOK_PLUS
 *              | TOK_NOT
 *              | TOK_STAR
 *              | TOK_AMP
 *
 * primary_expr ::= TOK_INT_LIT
 *                | TOK_HEX_LIT
 *                | TOK_FLOAT_LIT
 *                | TOK_IDENT
 *                | TOK_LPAREN expr TOK_RPAREN
 */


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
