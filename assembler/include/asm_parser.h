#ifndef ASSEMBLER_PARSER_H
#define ASSEMBLER_PARSER_H

#include <definitions.h>
#include <asm_lexer.h>
#include <asm_ast.h>


class AsmParser {
public:
    explicit AsmParser(const std::vector<Token> &tokens) : tokens(tokens) {
    }

    std::vector<AsmProgram> parseProgram();



private:
    const std::vector<Token> &tokens;
    size_t pos = 0;

    bool check(const int kind) const ;

    bool eof() const;

    bool match(int kind);

    [[nodiscard]] const Token &peek() const;

    const Token &get();

    //AsmLine parseLine();

    //MiniCInstruction parseInstruction();

    //std::vector<AsmOperand> parseOperandList();

    //AsmOperand parseOperand();
};

/* - Grammar

program        ::= { line }

line           ::= empty_line
| section_directive
| labeled_statement
| statement

empty_line     ::= NEWLINE

section_directive ::= ".text" NEWLINE
| ".data" NEWLINE

labeled_statement ::= label ":" [ statement ]

label          ::= IDENT

statement      ::= instruction NEWLINE
| data_directive NEWLINE

instruction    ::= IDENT [ operand_list ]

operand_list   ::= operand { "," operand }

operand         ::= register
| fp_register
| integer
| float
| string
| memory_operand
| label_ref

memory_operand ::= integer "(" register ")"
| "(" register ")"

data_directive ::= ".word" data_list
| ".float" data_list
| ".space" integer

data_list      ::= data_value { "," data_value }

data_value     ::= integer
| float
| string
| label_ref
*/

#endif
