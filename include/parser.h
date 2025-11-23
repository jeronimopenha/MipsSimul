#ifndef MIPSSIMUL_PARSER_H
#define MIPSSIMUL_PARSER_H

#include <definitions.h>
#include <lexer.h>
#include <ast.h>


class Parser {
public:
    explicit Parser(const std::vector<Token> &tokens);

    std::vector<Line> parseProgram();

private:
    std::vector<Token> tokens;
    size_t pos = 0;

    const Token &peek();

    const Token &get();

    bool match(TokenKind k);

    const Token &expect(TokenKind k, const std::string &msg);

    Line parseLine();

    Instruction parseInstruction();

    std::vector<Operand> parseOperandList();

    Operand parseOperand();
};

/* - Grammar

program    ::= { line }

line       ::= [ label ":" ] [ instruction ] NEWLINE

label      ::= IDENT

instruction ::= IDENT operand_list

operand_list ::= operand { "," operand }

operand ::= REGISTER
          | NUMBER
          | memaddr
          | IDENT        (* used as label for branch/jump *)

memaddr ::= NUMBER "(" REGISTER ")"
          | "(" REGISTER ")"
*/

#endif //MIPSSIMUL_PARSER_H
