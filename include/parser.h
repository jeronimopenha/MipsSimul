#ifndef MIPSSIMUL_PARSER_H
#define MIPSSIMUL_PARSER_H

#include <definitions.h>
#include <lexer.h>


struct Operand {
    enum class Kind { Reg, Imm, Mem, LabelRef } kind;

    int reg = 0; // if register
    int imm = 0; // if immediate or offset
    int baseReg = 0; // if Mem (imm(baseReg))
    std::string label; // if LabelRef
};

struct Instruction {
    std::string op; // "addi", "lw", "j", ...
    std::vector<Operand> args;
    int line = 0;
};

struct Line {
    std::string label; // "" if empty
    bool hasInstr = false;
    Instruction instr;
};

struct Parser {
    std::vector<Token> tokens;
    size_t pos = 0;

    // funções parseX virão aqui
    explicit Parser(const std::vector<Token> &tokens);

    const Token &peek();

    const Token &get();

    bool match(TokenKind k);

    const Token &expect(TokenKind k, const std::string &msg);

    std::vector<Line> parseProgram();

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
