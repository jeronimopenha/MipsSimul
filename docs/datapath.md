# Datapath – CPU MIPS Monociclo

Este documento descreve o datapath **mínimo** da CPU.

---

## 1. Componentes

- PC (Program Counter)
- Memória de instruções
- Banco de registradores
- ALU
- Memória de dados
- Controle

---

## 2. Fluxo Geral

PC → Memória de instruções → Instrução
↓
Controle → Registradores → ALU → Memória → Write-back

yaml
Copiar código

---

## 3. Caminhos principais

### LW:

ALU: rs + offset
Memória → rt

shell
Copiar código

### SW:

ALU: rs + offset
rt → Memória

shell
Copiar código

### ADD:

rs + rt → rd

shell
Copiar código

### BEQ:

if (rs == rt) PC = PC + offset

yaml
Copiar código

---

## 4. Loop WHILE

label:
cond
beq → exit
corpo
j label
exit:

yaml
Copiar código

---

## 5. Chamada de Função

jal func
→ $ra = PC+4
→ PC = func

Copiar código
jr $ra

yaml
Copiar código

---

## 6. Stack

$sp aponta para memória de dados
frame:
locals
$ra