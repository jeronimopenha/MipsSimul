# ISA Mínima – CPU MIPS-like para Mini-C

Esta é a especificação da **ISA mínima** necessária para executar todos os programas escritos em **Mini-C v0.1**.

A arquitetura é **load/store, 32 bits, registrador-registrador**, inspirada no MIPS.

---

## 1. Visão Geral

- Palavra: 32 bits
- Endereçamento: byte-addressed
- Memória: unificada ou separada (instr/dados)
- Modelo: RISC
- Execução inicial: **monociclo**

Esta ISA é **Turing Completa**, pois possui:
- Memória arbitrária (`LW`, `SW`)
- Saltos condicionais (`BEQ`, `BNE`)
- Laços (`while`)
- Chamadas recursivas (`JAL`, `JR`)

---

## 2. Banco de Registradores

| Tipo                 | Quantidade |
|----------------------|------------|
| Registradores gerais | 32         |

Convenção sugerida:

| Nome          | Uso               |
|---------------|-------------------|
| `$zero`       | constante 0       |
| `$at`         | reservado         |
| `$v0`, `$v1`, | retorno de função |
| `$a0–$a3`     | argumentos        |
| `$t0–$t9`     | temporários       |
| `$s0–$s7`     | salvos            |
| `$k0`, `$k1`  | reservados        |
| `$gp`,        | global pointer    |
| `$sp`         | stack pointer     |
| `$fp`         | stack frame       |
| `$ra`         | return address    |


---

## 3. Conjunto Mínimo de Instruções

### 3.1 Aritmética Lógica (R-type)

```
ADD rd, rs, rt
SUB rd, rs, rt
AND rd, rs, rt
OR rd, rs, rt
XOR rd, rs, rt
SLT rd, rs, rt
```

---

### 3.2 Imediatos (I-type)

```
ADDI rt, rs, imm
ORI rt, rs, imm
LUI rt, imm
```

---

### 3.3 Memória

```
LW rt, offset(rs)
SW rt, offset(rs)
```

---

### 3.4 Controle de Fluxo

```
BEQ rs, rt, label
BNE rs, rt, label
J label
```

---

### 3.5 Função e Retorno

```
JAL label
JR rs
```

---

## 4. Pseudoinstruções do Assembler

(*Hard coded* pelo assembler):

```
LA rd, label -> LUI + ORI
LI rd, imm32 -> LUI + ORI
MOVE rd, rs -> ADD rd, rs, $zero
NOP -> ADD $zero,$zero,$zero
```

---

## Efeitos

É possível executar:

- Mini-C v0.1 completo
- Recursão
- Ponteiros e vetores
- Hardware mapeado em memória
- Sistemas bare-metal
- Mini-kernels