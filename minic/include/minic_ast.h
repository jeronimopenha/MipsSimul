#ifndef MINIC_MINIC_AST_H
#define MINIC_MINIC_AST_H

#include <definitions.h>
#include <token.h>

struct Expr {
    virtual ~Expr() = default;
};

struct BinaryExpr : Expr {
    std::unique_ptr<Expr> left;
    Token op;
    std::unique_ptr<Expr> right;

    BinaryExpr(std::unique_ptr<Expr> l, Token o, std::unique_ptr<Expr> r)
        : left(std::move(l)), op(o), right(std::move(r)) {
    }
};

struct UnaryExpr : Expr {
    Token op;
    std::unique_ptr<Expr> expr;

    UnaryExpr(Token o, std::unique_ptr<Expr> e)
        : op(o), expr(std::move(e)) {
    }
};

struct LiteralExpr : Expr {
    Token value;

    explicit LiteralExpr(const Token &t) : value(t) {
    }
};

struct VariableExpr : Expr {
    Token name;

    explicit VariableExpr(const Token &t) : name(t) {
    }
};

struct GroupingExpr : Expr {
    std::unique_ptr<Expr> expr;

    explicit GroupingExpr(std::unique_ptr<Expr> e)
        : expr(std::move(e)) {
    }
};

#endif
