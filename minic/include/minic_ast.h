#ifndef MINIC_MINIC_AST_H
#define MINIC_MINIC_AST_H

#include <definitions.h>
#include <token.h>

#include <utility>

#include "minic_t_kind.h"

inline void printIndent(const int n) {
    for (int i = 0; i < n; ++i)
        std::cout << ' ';
}

// ----------------------------------------------
/*primary_expr ::= TOK_INT_LIT
               | TOK_HEX_LIT
               | TOK_FLOAT_LIT
               | TOK_IDENT
               | TOK_LPAREN expr TOK_RPAREN
*/

struct ExprNode {
    virtual ~ExprNode() = default;

    virtual void dump(int ident) =0;
};

struct HexLiteralNode : ExprNode {
    int32_t value;

    explicit HexLiteralNode(const int32_t v) : value(v) {
    }

    void dump(const int ident) override {
        printIndent(ident);
        std::cout << "INT_HEX(" << value << ")\n";
    }
};

struct IntLiteralNode : ExprNode {
    int32_t value;

    explicit IntLiteralNode(const int32_t v) : value(v) {
    }

    void dump(int ident) override {
        printIndent(ident);
        std::cout << "INT_DEC(" << value << ")\n";
    }
};

struct FloatLiteralNode : ExprNode {
    double value;

    explicit FloatLiteralNode(const double v) : value(v) {
    }

    void dump(const int ident) override {
        printIndent(ident);
        std::cout << "FLOAT(" << value << ")\n";
    }
};

struct IdentNode : ExprNode {
    std::string name;

    explicit IdentNode(std::string s) : name(std::move(s)) {
    }

    void dump(int ident) override {
        printIndent(ident);
        std::cout << "ID(" << name << ")\n";
    }
};

struct UnaryOpNode : ExprNode {
    int op; // token kind: TOK_MINUS, TOK_NOT, TOK_STAR, TOK_AMP
    //ParenExprNode *expr; // the operand
    std::unique_ptr<ExprNode> expr;

    UnaryOpNode(const int op, std::unique_ptr<ExprNode> expr)
        : op(op), expr(std::move(expr)) {
    }


    void dump(const int ident) override {
        printIndent(ident);
        std::cout << "(" << minicTokenKindToSimbol(op) << ")\n";
        expr->dump(ident + 2);
    }
};

struct BinaryOpNode : ExprNode {
    int op; // TOK_STAR, TOK_SLASH, TOK_PERCENT, ...
    std::unique_ptr<ExprNode> left;
    std::unique_ptr<ExprNode> right;

    BinaryOpNode(std::unique_ptr<ExprNode> l, const int op, std::unique_ptr<ExprNode> r)
        : op(op), left(std::move(l)), right(std::move(r)) {
    }

    void dump(const int ident) override {
        printIndent(ident);
        std::cout << "(" << minicTokenKindToSimbol(op) << ")\n";
        left->dump(ident + 2);
        right->dump(ident + 2);
    }
};

struct AssignNode : ExprNode {
    int op;
    std::unique_ptr<ExprNode> left;
    std::unique_ptr<ExprNode> right;

    AssignNode(std::unique_ptr<ExprNode> l, const int op, std::unique_ptr<ExprNode> r)
        : op(op), left(std::move(l)), right(std::move(r)) {
    }

    void dump(const int ident) override {
        printIndent(ident);
        std::cout << "(" << minicTokenKindToSimbol(op) << ")\n";
        left->dump(ident + 2);
        right->dump(ident + 2);
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
