#include <minic_parser.h>

using namespace std;

//TODO fix lexer that is transformim 10 in 16. It is thinkink 10 is hex, but not

ExprNode *MiniCParser::parseExpr() {
    ExprNode *expr = parseEql();
    match(TOK_SEMI);
    return expr;
}

/*
 * eql_expr ::= rel_expr { (TOK_EQ | TOK_NEQ ) rel_expr }
 */
ExprNode *MiniCParser::parseEql() {
    ExprNode *lhs = parseRel();

    while (match(TOK_EQ) || match(TOK_NEQ)) {
        const int op = previous().kind;

        ExprNode *rhs = parseRel();
        lhs = new BinaryOpNode(lhs, op, rhs);
    }

    return lhs;
}

/*
 * rel_expr ::= add_expr { (TOK_LT | TOK_LE | TOK_GT | TOK_GE) add_expr }
 */
ExprNode *MiniCParser::parseRel() {
    ExprNode *lhs = parseAdd();

    while (match(TOK_LT) || match(TOK_LE) || match(TOK_GT) || match(TOK_GE)) {
        const int op = previous().kind;

        ExprNode *rhs = parseAdd();
        lhs = new BinaryOpNode(lhs, op, rhs);
    }

    return lhs;
}

/*
 * add_expr ::= mul_expr { (TOK_PLUS | TOK_MINUS) mul_expr }
 */
ExprNode *MiniCParser::parseAdd() {
    ExprNode *lhs = parseMul();

    while (match(TOK_PLUS) || match(TOK_MINUS)) {
        const int op = previous().kind;

        ExprNode *rhs = parseMul();
        lhs = new BinaryOpNode(lhs, op, rhs);
    }

    return lhs;
}

/*
 * mul_expr ::= unary_expr { (TOK_STAR | TOK_SLASH | TOK_PERCENT) unary_expr }
 */
ExprNode *MiniCParser::parseMul() {
    ExprNode *lhs = parseUnary();

    while (match(TOK_STAR) || match(TOK_SLASH) || match(TOK_PERCENT)) {
        const int op = previous().kind;

        ExprNode *rhs = parseUnary();
        lhs = new BinaryOpNode(lhs, op, rhs);
    }

    return lhs;
}


/*
 * unary_expr ::= unary_op unary_expr
 *                | primary_expr
 *
 * unary_op ::= TOK_MINUS
 *              | TOK_PLUS
 *              | TOK_NOT
 *              | TOK_STAR
 *              | TOK_AMP
 */
ExprNode *MiniCParser::parseUnary() {
    if (match(TOK_MINUS) || match(TOK_PLUS) || match(TOK_NOT) || match(TOK_STAR) || match(TOK_AMP)) {
        const Token &opTok = previous();
        ExprNode *rhs = parseUnary();
        return new UnaryOpNode(opTok.kind, rhs);
    }
    return parsePrimary();
}

/*
 * primary_expr ::= TOK_INT_LIT
 *                | TOK_HEX_LIT
 *                | TOK_FLOAT_LIT
 *                | TOK_IDENT
 *                | TOK_LPAREN expr TOK_RPAREN
 */
ExprNode *MiniCParser::parsePrimary() {
    if (match(TOK_INT_LIT) || match(TOK_HEX_LIT)) {
        const Token &t = previous();
        return new IntLiteralNode(t.intValue);
    }

    if (match(TOK_FLOAT_LIT)) {
        const Token &t = previous();
        return new FloatLiteralNode(t.floatValue);
    }

    if (match(TOK_IDENT)) {
        const Token &t = previous();
        return new IdentNode(t.lexeme);
    }

    if (match(TOK_LPAREN)) {
        ExprNode *e = parseExpr();
        expect(TOK_RPAREN, "expected ')'");
        return e;
    }

    error(get(), "expected primary expression");

    return {};
}


void MiniCParser::skipNewLines() {
    while (match(TOK_NEWLN));
}
