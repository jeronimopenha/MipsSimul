#ifndef MINIC_MINIC_AST_H
#define MINIC_MINIC_AST_H

#include <definitions.h>
#include <token.h>


// ----------------------------------------------
/*primary_expr ::= TOK_INT_LIT
               | TOK_HEX_LIT
               | TOK_FLOAT_LIT
               | TOK_IDENT
               | TOK_LPAREN expr TOK_RPAREN
*/

struct ExprNode {
    virtual ~ExprNode() = default;

    virtual void dump() =0;
};

struct HexLiteralNode : ExprNode {
    int32_t value;

    explicit HexLiteralNode(const int32_t v) : value(v) {
    }

    void dump() override {
        std::cout << "HexLiteralNode(" << value << ")\n";
    }
};

struct IntLiteralNode : ExprNode {
    int32_t value;

    explicit IntLiteralNode(const int32_t v) : value(v) {
    }

    void dump() override {
        std::cout << "IntLiteralNode(" << value << ")\n";
    }
};

struct FloatLiteralNode : ExprNode {
    double value;

    explicit FloatLiteralNode(const double v) : value(v) {
    }

    void dump() override {
        std::cout << "FloatLiteralNode(" << value << ")\n";
    }
};

struct IdentNode : ExprNode {
    std::string name;

    IdentNode(const std::string &s) : name(s) {
    }

    void dump() override {
        std::cout << "IdentNode(" << name << ")\n";
    }
};

struct ParenExprNode : ExprNode {
    ExprNode *expr;

    ParenExprNode(ExprNode *e) : expr(e) {
    }

    void dump() override {
        expr->dump();
    }
};

// ----------------------------------------------


#endif

/*
* step 1 — Expressions

primary
unary
multiplicative
additive
relational
equality
logical
assignment
expr_stmt

step 2 — Declarations and statements

var_decl
block { ... }
return
if
while

step 3 — Functions

parameters
parameters list
function body
complete function

step 4 — Programa

external_decl
program

 */
