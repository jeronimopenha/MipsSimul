#include <minic_parser.h>

using namespace std;

//TODO fix lexer that is transformim 10 in 16. It is thinkink 10 is hex, but not

unique_ptr<ExprNode> MiniCParser::parseExpr() {
    unique_ptr<ExprNode> expr = parseAssign();
    match(TOK_SEMI);
    return expr;
}

/*
 * assign_expr := lvalue TOK_EQ assign_expr | or_expr
 */
std::unique_ptr<ExprNode> MiniCParser::parseAssign() {
    unique_ptr<ExprNode> lhs = parseOR();

    if (!match(TOK_ASSIGN)) {
        return lhs; // without '=' is not an assignment
    }

    const Token opTok = previous(); // '='

    if (!isLValue(lhs.get())) {
        error(opTok, "left side of '=' is not assignable");
    }

    unique_ptr<ExprNode> rhs = parseAssign();

    return make_unique<AssignNode>(std::move(lhs), opTok.kind, std::move(rhs));
}

bool MiniCParser::isLValue(ExprNode *n) {
    if (!n) return false;

    // IDENT
    const auto id = dynamic_cast<IdentNode *>(n);
    if (id) {
        return true;
    }

    // 2) *p   → unreference
    const auto un = dynamic_cast<UnaryOpNode *>(n);
    if (un) {
        return un->op == TOK_STAR;
    }

    // 3) v[i] ou v[i][j], index
    const auto idx = dynamic_cast<IndexNode *>(n);
    if (idx) {
        return true;
    }

    // 4) nothing
    return false;
}

/*
* or_expr ::= and_expr { (TOK_OR_OR ) and_expr }
*/
unique_ptr<ExprNode> MiniCParser::parseOR() {
    unique_ptr<ExprNode> lhs = parseAnd();

    while (match(TOK_OR_OR)) {
        const int op = previous().kind;

        unique_ptr<ExprNode> rhs = parseAnd();
        lhs = make_unique<BinaryOpNode>(std::move(lhs), op, std::move(rhs));
    }

    return lhs;
}

/*
* and_expr ::= eql_expr { (TOK_AND_AND ) eql_expr }
*/

unique_ptr<ExprNode> MiniCParser::parseAnd() {
    unique_ptr<ExprNode> lhs = parseEql();

    while (match(TOK_AND_AND)) {
        const int op = previous().kind;

        unique_ptr<ExprNode> rhs = parseEql();
        lhs = make_unique<BinaryOpNode>(std::move(lhs), op, std::move(rhs));
    }

    return lhs;
}

/*
 * eql_expr ::= rel_expr { (TOK_EQ | TOK_NEQ ) rel_expr }
 */
unique_ptr<ExprNode> MiniCParser::parseEql() {
    unique_ptr<ExprNode> lhs = parseRel();

    while (match(TOK_EQ) || match(TOK_NEQ)) {
        const int op = previous().kind;

        unique_ptr<ExprNode> rhs = parseRel();
        lhs = make_unique<BinaryOpNode>(std::move(lhs), op, std::move(rhs));
    }

    return lhs;
}

/*
 * rel_expr ::= add_expr { (TOK_LT | TOK_LE | TOK_GT | TOK_GE) add_expr }
 */
unique_ptr<ExprNode> MiniCParser::parseRel() {
    unique_ptr<ExprNode> lhs = parseAdd();

    while (match(TOK_LT) || match(TOK_LE) || match(TOK_GT) || match(TOK_GE)) {
        const int op = previous().kind;

        unique_ptr<ExprNode> rhs = parseAdd();
        lhs = make_unique<BinaryOpNode>(std::move(lhs), op, std::move(rhs));
    }

    return lhs;
}

/*
 * add_expr ::= mul_expr { (TOK_PLUS | TOK_MINUS) mul_expr }
 */
unique_ptr<ExprNode> MiniCParser::parseAdd() {
    unique_ptr<ExprNode> lhs = parseMul();

    while (match(TOK_PLUS) || match(TOK_MINUS)) {
        const int op = previous().kind;

        unique_ptr<ExprNode> rhs = parseMul();
        lhs = make_unique<BinaryOpNode>(std::move(lhs), op, std::move(rhs));
    }

    return lhs;
}

/*
 * mul_expr ::= unary_expr { (TOK_STAR | TOK_SLASH | TOK_PERCENT) unary_expr }
 */
unique_ptr<ExprNode> MiniCParser::parseMul() {
    unique_ptr<ExprNode> lhs = parseUnary();

    while (match(TOK_STAR) || match(TOK_SLASH) || match(TOK_PERCENT)) {
        const int op = previous().kind;

        unique_ptr<ExprNode> rhs = parseUnary();
        lhs = make_unique<BinaryOpNode>(std::move(lhs), op, std::move(rhs));
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
unique_ptr<ExprNode> MiniCParser::parseUnary() {
    if (match(TOK_MINUS) || match(TOK_PLUS) || match(TOK_NOT) || match(TOK_STAR) || match(TOK_AMP)) {
        const Token &opTok = previous();
        unique_ptr<ExprNode> rhs = parseUnary();
        return make_unique<UnaryOpNode>(opTok.kind, std::move(rhs));
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
unique_ptr<ExprNode> MiniCParser::parsePrimary() {
    if (match(TOK_INT_LIT)) {
        const Token &t = previous();
        return make_unique<IntLiteralNode>(t.intValue);
    }

    if (match(TOK_HEX_LIT)) {
        const Token &t = previous();
        return make_unique<HexLiteralNode>(t.intValue);
    }

    if (match(TOK_FLOAT_LIT)) {
        const Token &t = previous();
        return make_unique<FloatLiteralNode>(t.floatValue);
    }

    if (match(TOK_IDENT)) {
        const Token &t = previous();
        return make_unique<IdentNode>(t.lexeme);
    }

    if (match(TOK_LPAREN)) {
        unique_ptr<ExprNode> e = parseExpr();
        expect(TOK_RPAREN, "expected ')'");
        return e;
    }

    error(get(), "expected primary expression");

    return {};
}


void MiniCParser::skipNewLines() {
    while (match(TOK_NEWLN));
}

/*
 * lvalue ::=  lvalue_base { TOK_LBRACKET expr TOK_RBRACKET }
*/
std::unique_ptr<ExprNode> MiniCParser::parseLValue() {
    unique_ptr<ExprNode> node = parseLValueBase();

    while (match(TOK_LBRACKET)) {
        unique_ptr<ExprNode> idx = parseExpr();
        expect(TOK_RBRACKET, "expected ']'");
        node = std::make_unique<IndexNode>(std::move(node), std::move(idx));
    }

    return node;
}


/*
 * lvalue_base ::= TOK_IDENT
 *                 | '*' unary_expr
 *                 | TOK_LPAREN lvalue TOK_RPAREN
*/
std::unique_ptr<ExprNode> MiniCParser::parseLValueBase() {
    if (match(TOK_IDENT)) {
        Token idTok = previous();
        return std::make_unique<IdentNode>(idTok.lexeme);
    }
    if (match(TOK_STAR)) {
        auto rhs = parseUnary();
        return std::make_unique<UnaryOpNode>(TOK_STAR, std::move(rhs));
    }
    if (match(TOK_LPAREN)) {
        auto inner = parseLValue();
        expect(TOK_RPAREN, "expected ')'");
        return inner;
    }

    error(get(), "expected lvalue");
    return {};
}