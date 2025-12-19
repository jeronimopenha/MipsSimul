#include <minic_parser.h>

using namespace std;

//program ::= { global_decl } EOF
vector<unique_ptr<AstNode> > MiniCParser::parseProgram() {
    vector<unique_ptr<AstNode> > program;
    while (!eof()) {
        unique_ptr<AstNode> line = parseGlobalDecl();
        program.push_back(std::move(line));
    }
    return program;
}

// global_decl ::= func_def | decl_stmt
unique_ptr<AstNode> MiniCParser::parseGlobalDecl() {
    if (!(check(TOK_VOID) || check(TOK_INT) || check(TOK_FLOAT))) {
        return nullptr;
    }

    // VOID just func
    if (check(TOK_VOID)) {
        return parseFuncDef();
    }

    // Lookahead: type ('*')* IDENT '('  => func_def
    int k = 1; // 0 é o tipo (peek), então começamos olhando 1 à frente
    while (peekN(k).kind == TOK_STAR) k++;

    if (peekN(k).kind != TOK_IDENT) {
        error(peekN(k), "expected identifier after type");
    }

    if (peekN(k + 1).kind == TOK_LPAREN) {
        return parseFuncDef();
    }

    return parseDeclStmt();
}

// func_def ::= base_type TOK_IDENT TOK_LPAREN param_list? TOK_RPAREN block_stmt
// base_type ::= TOK_VOID | TOK_INT | TOK_FLOAT
unique_ptr<AstNode> MiniCParser::parseFuncDef() {
    unique_ptr<AstNode> funcNode;
    if (!(match(TOK_VOID) || match(TOK_INT) || match(TOK_FLOAT))) {
        error(peek(), "expected 'void', 'int' or 'float' as function return type");
    }
    const Token tType = previous();

    int ptr = 0;
    while (match(TOK_STAR))
        ptr++;

    expect(TOK_IDENT, "expected identifier");
    const Token tIdent = previous();

    unique_ptr<TypeSpec> returnType = make_unique<TypeSpec>(tType.kind, ptr);

    expect(TOK_LPAREN, "expected '('");
    vector<unique_ptr<ParamNode> > paramList;
    if (!check(TOK_RPAREN)) {
        paramList = parseParamList();
    }
    expect(TOK_RPAREN, "expected ')'");
    unique_ptr<StmtNode> block = parseBlockStmt();

    unique_ptr<DeclNode> funcDefNode = make_unique<FuncDefNode>(std::move(returnType), tIdent.lexeme,
                                                                std::move(paramList), std::move(block));
    return funcDefNode;
}

// param_list ::= param { TOK_COMMA param }
vector<unique_ptr<ParamNode> > MiniCParser::parseParamList() {
    vector<unique_ptr<ParamNode> > paramList;

    paramList.push_back(parseParamItem());

    while (match(TOK_COMMA)) {
        paramList.push_back(parseParamItem());
    }
    return paramList;
}

// param ::= param_type ptr_opt TOK_IDENT param_dims_opt
// param_type ::= TOK_INT | TOK_FLOAT
// ptr_opt     ::= { TOK_STAR }
// array_dims ::= { TOK_LBRACKET (TOK_INT_LIT | TOK_HEX_LIT) TOK_RBRACKET }
unique_ptr<ParamNode> MiniCParser::parseParamItem() {
    // param_type
    if (!(match(TOK_INT) || match(TOK_FLOAT))) {
        error(peek(), "expected int or float");
    }
    Token tType = previous();

    // ptr_opt
    int ptr = 0;
    while (match(TOK_STAR))
        ptr++;

    // IDENT
    expect(TOK_IDENT, "expected identifier");
    Token tIdent = previous();

    // arr_opt
    std::vector<std::unique_ptr<ExprNode> > dims = parseArrayDimsStmt();

    return make_unique<ParamNode>(tType.kind, tIdent.lexeme, ptr, std::move(dims));
}

/*
 * stmt ::= while_stmt
 *        | if_stmt
 *        | block_stmt
 *        | return_stmt
 *        | break_stmt
 *        | continue_stmt
 *        | decl_stmt
 *        | expr_stmt
 */
unique_ptr<StmtNode> MiniCParser::parseStmt() {
    if (match(TOK_SEMI)) {
        return make_unique<ExprStmtNode>(nullptr);
    }

    //while_stmt ::= TOK_WHILE TOK_LPAREN expr TOK_RPAREN block_stmt
    if (check(TOK_WHILE)) {
        return parseWhileStmt();
    }

    //if_stmt ::= TOK_IF TOK_LPAREN expr TOK_RPAREN block_stmt [ TOK_ELSE block_stmt]
    if (check(TOK_IF)) {
        return parseIfStmt();
    }

    //block_stmt ::= TOK_LBRACE {stmt} TOK_RBRACE
    if (check(TOK_LBRACE)) {
        return parseBlockStmt();
    }

    // return_stmt ::= TOK_RETURN expr? TOK_SEMI
    if (check(TOK_RETURN)) {
        return parseReturnStmt();
    }

    //break_stmt ::= TOK_BREAK TOK_SEMI
    if (check(TOK_BREAK)) {
        return parseBreakStmt();;
    }

    //continue_stmt ::= TOK_CONTINUE TOK_SEMI
    if (check(TOK_CONTINUE)) {
        return parseContinueStmt();
    }

    //decl_stmt ::= type decl_list TOK_SEMI
    if (check(TOK_INT) || check(TOK_FLOAT)) {
        return parseDeclStmt();
    }


    unique_ptr<ExprNode> expr = parseExpr();
    expect(TOK_SEMI, "expected semi");
    unique_ptr<StmtNode> stmt = make_unique<ExprStmtNode>(std::move(expr));
    return stmt;
}

//decl_stmt ::= type decl_list TOK_SEMI
unique_ptr<StmtNode> MiniCParser::parseDeclStmt() {
    if (match(TOK_INT) || match(TOK_FLOAT)) {
        Token t = previous();
        vector<unique_ptr<DeclStmtItem> > declList = parseDeclListStmt();
        expect(TOK_SEMI, "expected semi");

        unique_ptr<DeclStmtNode> decl = make_unique<DeclStmtNode>(t.kind, std::move(declList));
        return decl;
    }
    return nullptr;
}

//decl_list ::= decl_item { TOK_COMMA decl_item }
vector<unique_ptr<DeclStmtItem> > MiniCParser::parseDeclListStmt() {
    vector<unique_ptr<DeclStmtItem> > declList;
    declList.push_back(parseDeclItemStmt());
    while (match(TOK_COMMA)) {
        declList.push_back(parseDeclItemStmt());
    }
    return declList;
}

//decl_item ::= ptr_opt TOK_IDENT decl_suffix
unique_ptr<DeclStmtItem> MiniCParser::parseDeclItemStmt() {
    // ptr_opt
    int ptr = 0;
    while (match(TOK_STAR))
        ptr++;

    expect(TOK_IDENT, "expected identifier");
    const Token &t = previous();
    unique_ptr<DeclStmtItem> item = parseDeclSufixStmt();
    item->name = t.lexeme;
    item->ptrLevel = ptr;
    return item;
}

//decl_suffix ::= array_dims? init_opt?
unique_ptr<DeclStmtItem> MiniCParser::parseDeclSufixStmt() {
    vector<unique_ptr<ExprNode> > dims = parseArrayDimsStmt();
    unique_ptr<ExprNode> init = parseInitOptStmt();
    unique_ptr<DeclStmtItem> item = make_unique<DeclStmtItem>("", 0, std::move(dims), std::move(init));
    return item;
}

//array_dims ::= { TOK_LBRACKET (TOK_INT_LIT | TOK_HEX_LIT) TOK_RBRACKET }
vector<unique_ptr<ExprNode> > MiniCParser::parseArrayDimsStmt() {
    vector<unique_ptr<ExprNode> > dims;
    while (match(TOK_LBRACKET)) {
        if (match(TOK_INT_LIT)) {
            const Token &t = previous();
            dims.push_back(make_unique<IntLiteralNode>(t.intValue));
        } else if (match(TOK_HEX_LIT)) {
            const Token &t = previous();
            dims.push_back(make_unique<HexLiteralNode>(t.intValue));
        } else {
            //const Token &t = peek();
            //error(t, "expected array dimension literal (int or hex)");
            dims.push_back(nullptr);
        }
        expect(TOK_RBRACKET, "expected ']'");
    }
    return dims;
}

//init_opt ::= TOK_ASSIGN assign_expr | ε
unique_ptr<ExprNode> MiniCParser::parseInitOptStmt() {
    if (match(TOK_ASSIGN)) {
        return parseExpr();
    }
    return nullptr;
}

//while_stmt ::= TOK_WHILE TOK_LPAREN expr TOK_RPAREN block_stmt
unique_ptr<StmtNode> MiniCParser::parseWhileStmt() {
    expect(TOK_WHILE, "expected 'while'");
    expect(TOK_LPAREN, "expected '('");
    unique_ptr<ExprNode> cond = parseExpr();
    expect(TOK_RPAREN, "expected ')'");

    unique_ptr<StmtNode> body = parseBlockStmt();

    unique_ptr<StmtNode> stmt = make_unique<WhileStmtNode>(std::move(cond), std::move(body));
    return stmt;
}

//if_stmt ::= TOK_IF TOK_LPAREN expr TOK_RPAREN block_stmt [ TOK_ELSE block_stmt]
unique_ptr<StmtNode> MiniCParser::parseIfStmt() {
    expect(TOK_IF, "expected 'if'");
    expect(TOK_LPAREN, "expected '('");
    unique_ptr<ExprNode> cond = parseExpr();
    expect(TOK_RPAREN, "expected ')'");

    unique_ptr<StmtNode> thenBranch = nullptr;
    thenBranch = parseBlockStmt();

    unique_ptr<StmtNode> elseBranch = nullptr;
    if (match(TOK_ELSE)) {
        elseBranch = parseBlockStmt();
    }
    unique_ptr<StmtNode> stmt = make_unique<IfStmtNode>(std::move(cond), std::move(thenBranch),
                                                        std::move(elseBranch));
    return stmt;
}

//block_stmt ::= TOK_LBRACE {stmt} TOK_RBRACE
unique_ptr<StmtNode> MiniCParser::parseBlockStmt() {
    expect(TOK_LBRACE, "expected '{'");
    std::vector<std::unique_ptr<StmtNode> > items;
    while (!check(TOK_RBRACE) && !eof()) {
        items.push_back(parseStmt());
    }
    expect(TOK_RBRACE, "expected '}'");
    unique_ptr<StmtNode> stmt = make_unique<BlockStmtNode>(std::move(items));
    return stmt;
}

// return_stmt ::= TOK_RETURN expr? TOK_SEMI
unique_ptr<StmtNode> MiniCParser::parseReturnStmt() {
    expect(TOK_RETURN, "expected 'return'");
    unique_ptr<ExprNode> expr = nullptr;
    if (!check(TOK_SEMI)) {
        expr = parseExpr();
    }
    expect(TOK_SEMI, "expected semi");
    unique_ptr<StmtNode> stmt = make_unique<ReturnStmtNode>(std::move(expr));
    return stmt;
}

//break_stmt ::= TOK_BREAK TOK_SEMI
unique_ptr<StmtNode> MiniCParser::parseBreakStmt() {
    expect(TOK_BREAK, "expected 'break'");
    expect(TOK_SEMI, "expected semi");
    unique_ptr<StmtNode> stmt = make_unique<BreakStmtNode>();
    return stmt;
}

//continue_stmt ::= TOK_CONTINUE TOK_SEMI
unique_ptr<StmtNode> MiniCParser::parseContinueStmt() {
    expect(TOK_CONTINUE, "expected 'continue'");
    expect(TOK_SEMI, "expected semi");
    unique_ptr<StmtNode> stmt = make_unique<ContinueStmtNode>();
    return stmt;
}

/*
 * expr_stmt ::= expr? TOK_SEMI
 */
unique_ptr<StmtNode> MiniCParser::parseExprStmt() {
    unique_ptr<ExprNode> expr = parseExpr();
    match(TOK_SEMI);
    Token opTok = previous();
    unique_ptr<StmtNode> stmt = make_unique<ExprStmtNode>(std::move(expr));
    return stmt;
}

/*
 * expr := [assign_expr]
 */
unique_ptr<ExprNode> MiniCParser::parseExpr() {
    unique_ptr<ExprNode> expr = parseAssign();
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

    const Token opTok = previous();

    if (!isLValue(lhs.get())) {
        error(previousTwo(), "left side of '=' is not assignable");
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

    // 2) *p   → dereference
    const auto de = dynamic_cast<UnaryOpNode *>(n);
    if (de) {
        return de->op == TOK_STAR;
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
 *                | postfix_expr
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
    return parsePostfix();
}

/*
 * postfix_expr ::= primary_expr { TOK_LBRACKET expr TOK_RBRACKET | TOK_LPAREN arg_list? TOK_RPAREN }
 */
unique_ptr<ExprNode> MiniCParser::parsePostfix() {
    unique_ptr<ExprNode> node = parsePrimary();

    while (true) {
        if (match(TOK_LBRACKET)) {
            auto idx = parseExpr();
            expect(TOK_RBRACKET, "expected ']'");
            node = make_unique<IndexNode>(std::move(node), std::move(idx));
            continue;
        }

        if (match(TOK_LPAREN)) {
            vector<unique_ptr<ExprNode> > args;

            //optional
            if (!check(TOK_RPAREN)) {
                args = parseArgList();
            }

            expect(TOK_RPAREN, "expected ')'");
            node = make_unique<CallNode>(std::move(node), std::move(args));
            continue;
        }

        break;
    }

    return node;
}

// arg_list ::= expr { ',' expr }
vector<unique_ptr<ExprNode> > MiniCParser::parseArgList() {
    vector<unique_ptr<ExprNode> > args;
    args.push_back(parseExpr());
    while (match(TOK_COMMA)) {
        args.push_back(parseExpr());
    }
    return args;
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
