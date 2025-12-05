# ISA

## Arithmetic / logic

- add rd, rs, rt  
- sub rd, rs, rt 
- and rd, rs, rt
- or rd, rs, rt
- xor rd, rs, rt
- nor rd, rs, rt

## Comparison

- slt rd, rs, rt
- sltu rd, rs, rt

## Shifts

- sll rd, rt, shamt
- srl rd, rt, shamt
- sra rd, rt, shamt

## Immediate

- addi rt, rs, imm
- andi rt, rs, imm
- ori rt, rs, imm
- xori rt, rs, imm
- lui rt, imm 

## Memory

- lw rt, offset(rs)
- sw rt, offset(rs)
- lb rt, offset(rs)
- lbu rt, offset(rs)
- sb rt, offset(rs)

## Branches and junps

- beq rs, rt, label
- bne rs, rt, label
- j label
- jal label
- jr rs

## Pseudo
- move rd, rs
- nop