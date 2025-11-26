# CodeGen – Mini-C → MIPS Assembly

Este módulo transforma a **AST do Mini-C** em **assembly MIPS-like textual**.

---

## 1. Responsabilidade

O CodeGen NÃO é um assembler.

Ele apenas gera texto `.asm`.

---

## 2. Entrada

- AST analisada semanticamente
- Tabela de símbolos
- Tipos (int, int*, int[])

---

## 3. Saída

- Código assembly textual
- Uso de pseudoinstruções permitido

---

## 4. Mapeamento Básico

### Atribuição:

c:
```
x = a + b;
```
asm:
```
lw   $t0, a($fp)
lw   $t1, b($fp)
add  $t2, $t0, $t1
sw   $t2, x($fp)
```
---
## 5. IF
c
```
if (x < 10) { ... }
```
asm
```
lw   $t0, x($fp)
li   $t1, 10
slt  $t2, $t0, $t1
beq  $t2, $zero, L_end
```
---
## 6. WHILE
c
```
while (x != 0) { ... }
```
asm
```
L_loop:
lw   $t0, x($fp)
beq  $t0, $zero, L_exit

...
j    L_loop
L_exit:
```
---

## 7. Funções

Prólogo:

```
addi $sp, $sp, -N
sw   $ra, offset($sp)
```
Epílogo:
```
lw   $ra, offset($sp)
addi $sp, $sp, N
jr   $ra
```

## 8. Ponteiros e Arrays
```
*p   → lw/sw via ponteiro
p[i] → base + i*4
```
---
## 9. Builtins no CodeGen
Exemplos:
```
print_int(x)
led_write(x)
uart_read()
```
O CodeGen gera chamadas especiais ou acesso direto a hardware.

## 10. Independência da Arquitetura
Apenas este módulo depende da ISA.

Parser, AST e Semântica são totalmente independentes.
```

