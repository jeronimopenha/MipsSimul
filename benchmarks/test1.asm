# Programa de teste "grande" para o lexer/parser MIPS
# NÃO tem .data/.text pra não atrapalhar o parser nesse começo

main:
    # inicializa alguns registradores
    addi $t0, $zero, 10      # t0 = 10  (limite do loop externo)
    addi $t1, $zero, 0       # t1 = 0   (contador externo)
    addi $t2, $zero, 0       # t2 = 0   (acumulador)
    addi $t3, $zero, 0       # t3 = 0   (contador interno)
    addi $s0, $zero, 0       # s0 = 0   (uso geral)
    addi $s1, $zero, 0       # s1 = 0   (uso geral)

outer_loop:
    # se t1 == t0, fim do loop externo
    beq  $t1, $t0, end_outer

    # zera contador interno
    addi $t3, $zero, 0       # t3 = 0

inner_loop:
    # se t3 == t1, sai do loop interno
    beq  $t3, $t1, end_inner

    # t2 = t2 + t3
    add  $t2, $t2, $t3

    # t3++
    addi $t3, $t3, 1

    # volta pro inner_loop
    j    inner_loop

end_inner:
    # t1++
    addi $t1, $t1, 1

    # volta pro outer_loop
    j    outer_loop

end_outer:
    # só pra ter umas lógicas com and/or/slt
    addi $a0, $zero, 5
    addi $a1, $zero, 12

    and  $s0, $a0, $a1       # s0 = a0 & a1
    or   $s1, $a0, $a1       # s1 = a0 | a1
    slt  $t4, $a0, $a1       # t4 = (a0 < a1) ? 1 : 0

    # simula base de memória em gp
    addi $gp, $zero, 0x1000

    # guarda alguns valores em "memória"
    sw   $t2, 0($gp)         # MEM[0x1000] = t2
    sw   $s0, 4($gp)         # MEM[0x1004] = s0
    sw   $s1, 8($gp)         # MEM[0x1008] = s1

    # carrega de volta
    lw   $t5, 0($gp)         # t5 = MEM[0x1000]
    lw   $t6, 4($gp)         # t6 = MEM[0x1004]
    lw   $t7, 8($gp)         # t7 = MEM[0x1008]

    # chama uma "função" max2(a0,a1)
    jal  max2

    # resultado em v0; copia pra s2
    addi $s2, $v0, 0         # s2 = v0

    # fim: pula pra label end
    j    end

max2:
    # calcula max(a0,a1) e retorna em v0
    # if (a0 < a1) v0 = a1; else v0 = a0;

    slt  $t4, $a0, $a1       # t4 = (a0 < a1)
    beq  $t4, $zero, a_ge_b

    # então a0 < a1 -> v0 = a1
    addi $v0, $a1, 0
    j    ret_max2

a_ge_b:
    # caso contrário -> v0 = a0
    addi $v0, $a0, 0

ret_max2:
    # "retorno" manual: aqui você depois vai implementar jr $ra
    # por enquanto deixa só um salto para end para não complicar o montador
    j    end

end:
    # nop real em MIPS: sll $zero,$zero,0
    sll  $zero, $zero, 0

#main:
#    addi $t0, $zero, 5
#    lw   $t1, 4($sp)   # load
#    j    main
