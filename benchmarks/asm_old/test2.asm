.Data
.Text
        # 0: t0 = 5
        addi $t0, $zero, 5

        # 1: t1 = 10
        addi $t1, $zero, 10

        # 2: t2 = t0 + t1
        add  $t2, $t0, $t1

        # 3: store t2 em 0(sp)
        sw   $t2, 0($sp)

        # 4: load pra t3
        lw   $t3, 0($sp)

        # 5: se t3 == t2, vai pra ok
        beq  $t3, $t2, ok

        # 6: delay slot / instrução qualquer (coloquei nop)
        nop

loop:
        # 7: loop infinito em loop
        j    loop

        # 8: delay slot de j (nop)
        nop

ok:
        # 9: move t0 = t3 (pseudo)
        move $t0, $t3

        # 10: retorno
        jr   $ra

        # 11: delay slot (nop)
        nop

#0  00000000  addi $t0,$zero,5      0x20080005
#1  00000004  addi $t1,$zero,10     0x2009000A
#2  00000008  add  $t2,$t0,$t1      0x01095020
#3  0000000C  sw   $t2,0($sp)       0xAFAA0000
#4  00000010  lw   $t3,0($sp)       0x8FAB0000
#5  00000014  beq  $t3,$t2,ok       0x116A0003
#6  00000018  nop                   0x00000020   ; add $zero,$zero,$zero
#7  0000001C  loop: j loop          0x08000007
#8  00000020  nop                   0x00000020
#9  00000024  ok: move $t0,$t3      0x01604020   ; add $t0,$t3,$zero
#10 00000028  jr   $ra              0x03E00008
#11 0000002C  nop                   0x00000020
