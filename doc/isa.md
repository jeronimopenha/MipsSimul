# R-type:
* add, sub, mul, and, or, xor, nor, slt, sltu
* sll, srl, sra
*  jr

# I-type:
* addi, andi, ori, xori, lui
* lw, sw, lb, lbu, sb
* beq, bne

# J-type:
* j, jal

# Pseudo/controle:
* move, nop, halt

# FP memoria:
* lwc1, swc1

# FP aritmetica:
* add.s, sub.s, mul.s, div.s

# FP comparacao:
* c.eq.s, c.lt.s, c.le.s

# FP branch:
* bc1t, bc1f

$zero = 0
$at   = 1
$v0   = 2
$v1   = 3
$a0   = 4
$a1   = 5
$a2   = 6
$a3   = 7
$t0   = 8
$t1   = 9
$t2   = 10
$t3   = 11
$t4   = 12
$t5   = 13
$t6   = 14
$t7   = 15
$s0   = 16
$s1   = 17
$s2   = 18
$s3   = 19
$s4   = 20
$s5   = 21
$s6   = 22
$s7   = 23
$t8   = 24
$t9   = 25
$k0   = 26
$k1   = 27
$gp   = 28
$sp   = 29
$fp   = 30
$ra   = 31

$f0  = 0
$f1  = 1
$f2  = 2
$f3  = 3
...
$f30 = 30
$f31 = 31

FPCond

PC
GPR[32]
FPR[32]
FPCond
Memory
halted
cycle