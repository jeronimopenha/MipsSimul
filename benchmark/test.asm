# full_isa_test.asm

.data
x:       .word 10
y:       .word 20
z:       .word 0
neg:     .word -15
hexv:    .word 0xFF

fa:      .float 2.5
fb:      .float 4.0
fc:      .float 0.0
fneg:    .float -3.75

vet:     .word 1, 2, 3, 4
buf:     .space 16

.text
main:
    lw   $t0, 0($gp)
    lw   $t1, 4($gp)
    lw   $t2, 12($gp)
    lw   $t3, 16($gp)

    add  $s0, $t0, $t1
    sub  $s1, $t1, $t0
    mul  $s2, $t0, $t1
    and  $s3, $t0, $t1
    or   $s4, $t0, $t1
    xor  $s5, $t0, $t1
    nor  $s6, $t0, $t1
    slt  $s7, $t0, $t1
    sltu $t8, $t0, $t1

    sll  $t4, $t0, 2
    srl  $t5, $t1, 1
    sra  $t6, $t2, 1

    addi $a0, $zero, -10
    addi $a1, $zero, 0x20
    andi $a2, $a1, 0x0F
    ori  $a3, $zero, 0x80
    xori $v0, $a3, 0xFF
    lui  $v1, 0x1001

    sw   $s0, 8($gp)
    sb   $t0, 52($gp)
    lb   $t9, 52($gp)
    lbu  $k0, 52($gp)

    move $s0, $t0
    nop

    beq  $s0, $t0, equal_case
    addi $s1, $zero, -1

equal_case:
    bne  $s0, $t1, different_case
    addi $s2, $zero, -2

different_case:
    jal function_test
    j fp_test

function_test:
    addi $s5, $zero, 123
    jr $ra

fp_test:
    lwc1  $f2,  20($gp)
    lwc1  $f4,  24($gp)
    lwc1  $f14, 32($gp)

    add.s $f6,  $f2, $f4
    sub.s $f8,  $f4, $f2
    mul.s $f10, $f2, $f4
    div.s $f12, $f4, $f2

    swc1  $f6, 28($gp)

    c.lt.s $f2, $f4
    bc1t   fp_less
    addi   $s6, $zero, 0
    j      after_fp_less

fp_less:
    addi   $s6, $zero, 1

after_fp_less:
    c.eq.s $f2, $f4
    bc1f   fp_not_equal
    addi   $s7, $zero, 0
    j      after_fp_equal

fp_not_equal:
    addi   $s7, $zero, 1

after_fp_equal:
    c.le.s $f2, $f4
    bc1t   fp_less_equal
    addi   $t8, $zero, 0
    j      end

fp_less_equal:
    addi   $t8, $zero, 1

end:
    halt