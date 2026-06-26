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