program        ::= { line } EOF

line           ::= empty_line
| section_directive line_end
| labeled_statement line_end
| statement line_end

line_end       ::= NEWLINE
| EOF

empty_line     ::= NEWLINE

section_directive ::= ".text"
| ".data"

labeled_statement ::= label ":" [ statement ]

label          ::= IDENT

statement      ::= instruction
| data_directive

instruction    ::= IDENT [ operand_list ]

operand_list   ::= operand { "," operand }

operand        ::= memory_operand
| register
| fp_register
| integer
| float
| label_ref

memory_operand ::= integer "(" register ")"
| "(" register ")"

data_directive ::= ".word" data_list
| ".float" float_list
| ".space" integer

data_list      ::= data_value { "," data_value }

data_value     ::= integer
| label_ref

float_list     ::= float_value { "," float_value }

float_value    ::= float
| integer

label_ref      ::= IDENT

register       ::= ASM_REG
fp_register    ::= ASM_FP_REG
integer        ::= ASM_INT_LIT
float          ::= ASM_FLOAT_LIT