# Teste 3 - operações variadas
add $t0, $t1, $t2        # $8 = $9 + $10
sub $s0, $s1, $s2        # $16 = $17 - $18
and $a0, $a1, $a2        # $4 = $5 AND $6
or $v1, $v0, $zero       # $3 = $2 OR $0
slt $t3, $t4, $t5        # $11 = ($12 < $13) ? 1 : 0
addi $s5, $s5, -15       # $21 = $21 - 15
lw $t6, 8($sp)           # $14 = Mem[$29 + 8]
sw $t7, -12($sp)         # Mem[$15 - 12] = $15
beq $t0, $t1, 5          # branch se $8 == $9
bne $s2, $s3, -2         # branch se $18 != $19
j 4096                    # salto para endereço = 4096
