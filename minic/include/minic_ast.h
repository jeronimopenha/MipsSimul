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

struct IndexNode : ExprNode {
    std::unique_ptr<ExprNode> base;
    std::unique_ptr<ExprNode> index;

    IndexNode(std::unique_ptr<ExprNode> b, std::unique_ptr<ExprNode> i)
        : base(std::move(b)), index(std::move(i)) {
    }

    void dump(const int ident) override {
        printIndent(ident);
        std::cout << "INDEX\n";
        if (base) {
            base->dump(ident + 2);
        } else {
            printIndent(ident + 2);
            std::cout << "NULL\n";
        }
        if (index) {
            index->dump(ident + 2);
        } else {
            printIndent(ident + 2);
            std::cout << "NULL\n";
        }
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
        if (expr) {
            expr->dump(ident + 2);
        } else {
            printIndent(ident + 2);
            std::cout << "NULL\n";
        }
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
        if (left) {
            left->dump(ident + 2);
        } else {
            printIndent(ident + 2);
            std::cout << "NULL\n";
        }
        if (right) {
            right->dump(ident + 2);
        } else {
            printIndent(ident + 2);
            std::cout << "NULL\n";
        }
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
        if (left) {
            left->dump(ident + 2);
        } else {
            printIndent(ident + 2);
            std::cout << "NULL\n";
        }
        if (right) {
            right->dump(ident + 2);
        } else {
            printIndent(ident + 2);
            std::cout << "NULL\n";
        }
    }
};

struct CallNode : ExprNode {
    std::unique_ptr<ExprNode> callee;
    std::vector<std::unique_ptr<ExprNode> > args;

    CallNode(std::unique_ptr<ExprNode> callee,
             std::vector<std::unique_ptr<ExprNode> > args)
        : callee(std::move(callee)), args(std::move(args)) {
    }

    void dump(const int ident) override {
        printIndent(ident);
        std::cout << "CALL\n";
        callee->dump(ident + 2);
        for (const auto &a: args) {
            a->dump(ident + 2);
        }
    }
};

struct StmtNode {
    virtual ~StmtNode() = default;

    virtual void dump(int ident) = 0;
};

struct ExprStmtNode : StmtNode {
    std::unique_ptr<ExprNode> expr;

    ExprStmtNode(std::unique_ptr<ExprNode> expr) : expr(std::move(expr)) {
    }

    void dump(const int ident) override {
        printIndent(ident);
        std::cout << "EXPR" << "\n";
        if (expr) {
            expr->dump(ident + 2);
        } else {
            printIndent(ident + 2);
            std::cout << "NULL\n";
        }
    }
};

struct ReturnStmtNode : StmtNode {
    std::unique_ptr<ExprNode> expr;

    ReturnStmtNode(std::unique_ptr<ExprNode> expr) : expr(std::move(expr)) {
    }

    void dump(const int ident) override {
        printIndent(ident);
        std::cout << "RETURN" << "\n";
        if (expr) {
            expr->dump(ident + 2);
        } else {
            printIndent(ident + 2);
            std::cout << "NULL\n";
        }
    }
};

struct WhileStmtNode : StmtNode {
    std::unique_ptr<ExprNode> cond;
    std::unique_ptr<StmtNode> body;

    WhileStmtNode(
        std::unique_ptr<ExprNode> cond,
        std::unique_ptr<StmtNode> body
    ) : cond(std::move(cond)),
        body(std::move(body)) {
    }

    void dump(const int ident) override {
        printIndent(ident);
        std::cout << "WHILE" << "\n";
        if (cond) {
            cond->dump(ident + 2);
        } else {
            printIndent(ident + 2);
            std::cout << "NULL\n";
        }
        printIndent(ident);
        std::cout << "DO" << "\n";
        if (body) {
            body->dump(ident + 2);
        } else {
            printIndent(ident + 2);
            std::cout << "NULL\n";
        }
    }
};

struct IfStmtNode : StmtNode {
    std::unique_ptr<ExprNode> cond;
    std::unique_ptr<StmtNode> thenBranch;
    std::unique_ptr<StmtNode> elseBranch;

    IfStmtNode(std::unique_ptr<ExprNode> cond,
               std::unique_ptr<StmtNode> thenBranch,
               std::unique_ptr<StmtNode> elseBranch
    ) : cond(std::move(cond)),
        thenBranch(std::move(thenBranch)),
        elseBranch(std::move(elseBranch)) {
    }

    void dump(const int ident) override {
        printIndent(ident);
        std::cout << "IF" << "\n";
        if (cond) {
            cond->dump(ident + 2);
        } else {
            printIndent(ident + 2);
            std::cout << "NULL\n";
        }
        std::cout << "THEN" << "\n";
        if (thenBranch) {
            thenBranch->dump(ident + 2);
        } else {
            printIndent(ident + 2);
            std::cout << "NULL\n";
        }
        std::cout << "ELSE" << "\n";
        if (elseBranch) {
            elseBranch->dump(ident + 2);
        } else {
            printIndent(ident + 2);
            std::cout << "NULL\n";
        }
    }
};

struct BlockStmtNode : StmtNode {
    std::vector<std::unique_ptr<StmtNode> > items;

    BlockStmtNode(std::vector<std::unique_ptr<StmtNode> > items) : items(std::move(items)) {
    }

    void dump(const int ident) override {
        printIndent(ident);
        std::cout << "BLOCK" << "\n";
        for (const auto &item: items) {
            if (item)
                item->dump(ident + 2);
        }
    }
};

struct BreakStmtNode : StmtNode {
    BreakStmtNode() = default;

    void dump(const int ident) override {
        printIndent(ident);
        std::cout << "BREAK" << "\n";
    }
};

struct ContinueStmtNode : StmtNode {
    ContinueStmtNode() = default;

    void dump(const int ident) override {
        printIndent(ident);
        std::cout << "CONTINUE" << "\n";
    }
};

struct DeclItem {
    std::string name;
    std::vector<std::unique_ptr<ExprNode> > dims;
    std::unique_ptr<ExprNode> init;

    DeclItem(
        std::string name,
        std::vector<std::unique_ptr<ExprNode> > dims,
        std::unique_ptr<ExprNode> init
    ) : name(std::move(name)),
        dims(std::move(dims)),
        init(std::move(init)) {
    }

    void dump(const int ident) const {
        printIndent(ident);
        std::cout << "ITEM " << name << "\n";
        bool flag = false;
        for (auto &dim: dims) {
            if (dim) {
                if (!flag) {
                    printIndent(ident + 2);
                    std::cout << "DIMS\n";
                    flag = true;
                }
                dim->dump(ident + 4);
            }
        }
        if (init) {
            printIndent(ident + 2);
            std::cout << "INIT\n";
            init->dump(ident + 4);
        }
    }
};

struct DeclStmtNode : StmtNode {
    int type;
    std::vector<std::unique_ptr<DeclItem> > items;

    DeclStmtNode(
        const int type,
        std::vector<std::unique_ptr<DeclItem> > items
    ) : type(type), items(std::move(items)) {
    }

    void dump(const int ident) override {
        printIndent(ident);
        std::cout << "DECL " << minicTokenKindToSimbol(type) << "\n";
        for (auto &item: items) {
            if (item) {
                item->dump(ident + 2);
            } else {
                std::cout << "null\n";
            }
        }
    }
};


// ----------------------------------------------


#endif

/*
* step 1 — Expressions

primary
unary
multiplicative
addictive
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
