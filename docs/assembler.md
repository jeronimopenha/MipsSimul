# Assembler MIPS – Especificação

O assembler é responsável por converter **assembly textual** em **código de máquina executável** pela CPU MIPS-like do projeto.

Ele funciona de forma independente do compilador Mini-C.

---

## 1. Função do Assembler

Pipeline interna:

```
Assembly (.asm)
↓
Lexer ASM
↓
Parser ASM
↓
Montagem (Assembler)
↓
Binário (.bin / .hex)
```

---

## 2. Entrada Aceita

- Arquivos `.asm`
- Sintaxe MIPS-like
- Labels
- Diretivas mínimas: `.text`, `.data`, `.word`

---

## 3. Instruções Reais da ISA

O assembler gera instruções binárias apenas para:

```
ADD SUB AND OR XOR SLT
ADDI ORI LUI
LW SW
BEQ BNE J
JAL JR
```

---

## 4. Pseudoinstruções

O assembler aceita e expandir:

```
LA rd, label
LI rd, imm32
MOVE rd, rs
NOP
```

Expansões:

```
MOVE rd, rs → ADD rd, rs, $zero
NOP → ADD $zero, $zero, $zero
LA, LI → LUI + ORI
```

---

## 5. Labels e Endereços

- Duas passagens:
    - Passagem 1: resolve labels
    - Passagem 2: gera binário

---

## 6. Saída

A saída pode ser:

- `.bin` para simulador
- `.hex` para memória da FPGA
- `.mem` para inicialização de ROM

---