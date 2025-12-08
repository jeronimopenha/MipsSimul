#include <minic_parser.h>

using namespace std;

unique_ptr<Expr> MiniCParser::parseExpr() {
    skipNewLines();
    return parseAssignExpr();
}

unique_ptr<Expr> MiniCParser::parseAssignExpr() {
    skipNewLines();
    auto left = parseLogicalOrExpr();

    if (match({TOK_ASSIGN})) {
        Token op = previous();
        auto value = parseAssignExpr(); // recursão à direita

        // se quiser depois tratar l-values de verdade, aqui é o lugar
        return make_unique<BinaryExpr>(move(left), op, move(value));
    }

    return left;
}

unique_ptr<Expr> MiniCParser::parseLogicalOrExpr() {
    skipNewLines();
    auto expr = parseLogicalAndExpr();

    while (match({TOK_OR_OR})) {
        Token op = previous();
        auto right = parseLogicalAndExpr();
        expr = make_unique<BinaryExpr>(move(expr), op, move(right));
    }

    return expr;
}

unique_ptr<Expr> MiniCParser::parseLogicalAndExpr() {
    skipNewLines();
    auto expr = parseEqualityExpr();

    while (match({TOK_AND_AND})) {
        Token op = previous();
        auto right = parseEqualityExpr();
        expr = make_unique<BinaryExpr>(move(expr), op, move(right));
    }

    return expr;
}

unique_ptr<Expr> MiniCParser::parseEqualityExpr() {
    skipNewLines();
    auto expr = parseRelationalExpr();

    while (match({TOK_EQ, TOK_NEQ})) {
        Token op = previous();
        auto right = parseRelationalExpr();
        expr = make_unique<BinaryExpr>(move(expr), op, move(right));
    }

    return expr;
}

unique_ptr<Expr> MiniCParser::parseRelationalExpr() {
    skipNewLines();
    auto expr = parseAdditiveExpr();

    while (match({TOK_LT, TOK_LE, TOK_GT, TOK_GE})) {
        Token op = previous();
        auto right = parseAdditiveExpr();
        expr = make_unique<BinaryExpr>(move(expr), op, move(right));
    }

    return expr;
}

unique_ptr<Expr> MiniCParser::parseAdditiveExpr() {
    skipNewLines();
    auto expr = parseMultiplicativeExpr();

    while (match({TOK_PLUS, TOK_MINUS})) {
        Token op = previous();
        auto right = parseMultiplicativeExpr();
        expr = make_unique<BinaryExpr>(move(expr), op, move(right));
    }

    return expr;
}

unique_ptr<Expr> MiniCParser::parseMultiplicativeExpr() {
    skipNewLines();
    auto expr = parseUnaryExpr();

    while (match({TOK_STAR, TOK_SLASH, TOK_PERCENT})) {
        Token op = previous();
        auto right = parseUnaryExpr();
        expr = make_unique<BinaryExpr>(move(expr), op, move(right));
    }

    return expr;
}

std::unique_ptr<Expr> MiniCParser::parseUnaryExpr() {
    skipNewLines();
    if (match({TOK_PLUS, TOK_MINUS, TOK_NOT, TOK_STAR, TOK_AMP})) {
        Token op = previous();
        auto right = parseUnaryExpr();
        return std::make_unique<UnaryExpr>(op, std::move(right));
    }

    return parsePostfixExpr();
}

std::unique_ptr<Expr> MiniCParser::parsePostfixExpr() {
    skipNewLines();
    auto expr = parsePrimaryExpr();

    while (true) {
        if (match({TOK_LPAREN})) {
            // args
            std::vector<std::unique_ptr<Expr>> args;
            if (!check(TOK_RPAREN)) {
                do {
                    args.push_back(parseAssignExpr());
                } while (match({TOK_COMMA}));
            }
            expect(TOK_RPAREN, "expected ')' after arguments");

            // por enquanto, se quiser, pode ignorar CallExpr
            // ou declarar uma struct CallExpr como fiz antes
            // Aqui, pra não alongar, você pode só dar erro se quiser implementar depois.
            // Mas o certo é criar um CallExpr.
            // Vou deixar o TODO aqui:
            // expr = std::make_unique<CallExpr>(std::move(expr), std::move(args));
        } else if (match({TOK_LBRACKET})) {
            auto index = parseAssignExpr();
            expect(TOK_RBRACKET, "expected ']' after index");
            // TODO: criar IndexExpr e usar aqui
        } else {
            break;
        }
    }

    return expr;
}


std::unique_ptr<Expr> MiniCParser::parsePrimaryExpr() {
    skipNewLines();
    if (match({TOK_INT_LIT, TOK_HEX_LIT, TOK_FLOAT_LIT})) {
        return std::make_unique<LiteralExpr>(previous());
    }

    if (match({TOK_IDENT})) {
        return std::make_unique<VariableExpr>(previous());
    }

    if (match({TOK_LPAREN})) {
        auto expr = parseAssignExpr();
        expect(TOK_RPAREN, "expected ')' after expression");
        return std::make_unique<GroupingExpr>(std::move(expr));
    }

    error(peek(), "expected expression");
}



void MiniCParser::error(const Token &token, const string &msg) {
    ostringstream oss;
    oss << "MiniC parse error at line " << token.line
            << ", col " << token.col << ": " << msg
            << " (found '" << token.lexeme << "')";
    throw runtime_error(oss.str());
}

void MiniCParser::skipNewLines() {
    while (check(TOK_NEWLN)) {
        get();
    }
}