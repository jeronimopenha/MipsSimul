#include <minic_parser.h>

using namespace std;

ExprNode *MiniCParser::parseExpr() {
    //skipNewLines();
    //return ; //parseAssignExpr();
}

ExprNode *MiniCParser::parsePrimary() {
    skipNewLines();

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
}


void MiniCParser::skipNewLines() {
    while (check(TOK_NEWLN)) {
        get();
    }
}
