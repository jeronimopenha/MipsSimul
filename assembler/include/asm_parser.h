#ifndef ASSEMBLER_PARSER_H
#define ASSEMBLER_PARSER_H

#include <definitions.h>
#include <asm_lexer.h>
#include <parser.h>
#include <asm_ast.h>


class AsmParser : public Parser {
public:
    explicit AsmParser(const std::vector<Token> &tokens) : Parser(tokens) {
    }

    std::vector<AsmLine> parseProgram();

private:
    AsmLine parseLine();

    MiniCInstruction parseInstruction();

    std::vector<AsmOperand> parseOperandList();

    AsmOperand parseOperand();
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

#endif
