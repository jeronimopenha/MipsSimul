# mipsSimul – Ecossistema Completo Mini-C + MIPS + FPGA

Este repositório contém um ecossistema completo para:

- Estudo de **Arquitetura de Computadores**
- Desenvolvimento de um **processador MIPS-like em FPGA**
- Implementação de um **simulador MIPS**
- Desenvolvimento de um **assembler próprio**
- Criação de um **compilador Mini-C**
- Execução de programas bare-metal e mini-kernels

O projeto é organizado em camadas, desde a linguagem de alto nível até o hardware.

---

## Visão Geral da Pipeline

```
Mini-C (fonte)
↓
Lexer Mini-C
↓
Parser Mini-C
↓
AST + Análise Semântica
↓
CodeGen MIPS → Assembly (.asm)
↓
Lexer ASM
↓
Parser ASM
↓
Assembler → Binário (.bin / .hex)
↓
Simulador MIPS / CPU em FPGA
```

---

## Organização do Repositório

```
mipsSimul/
├── cpu/ # CPU MIPS-like (Verilog/VHDL)
├── simulator/ # Simulador em C++
├── assembler/ # Montador MIPS próprio
├── minic/ # Compilador Mini-C
├── runtime/ # Rotinas em assembly (builtins)
├── examples/ # Programas Mini-C
├── docs/ # Documentação
│ ├── ISA.md
│ ├── MiniC_Spec.md
│ ├── Assembler.md
│ ├── CodeGen.md
│ ├── Runtime.md
│ ├── Datapath.md
│ └── Pipeline.md
└── tools/ # Scripts de build e testes
```

---

## Objetivos do Projeto

- Criar uma **ISA mínima MIPS-like**
- Implementar uma **CPU monociclo em FPGA**
- Desenvolver um **assembler com pseudoinstruções**
- Desenvolver um **compilador Mini-C próprio**
- Rodar:
  - jogos simples (Pong, Snake)
  - algoritmos clássicos
  - mini-kernel bare-metal
---

## Mini-C

A especificação completa da linguagem está em:

```
docs/MiniC_Spec.md
```

---

## ISA

A especificação da arquitetura está em:

```
docs/ISA.md
```

---

## Status do Projeto

- [x] Definição da ISA mínima
- [x] Definição da linguagem Mini-C
- [ ] Lexer do Mini-C
- [ ] Parser do Mini-C
- [ ] CodeGen Mini-C → MIPS
- [x] Assembler MIPS
- [ ] Simulador MIPS completo
- [ ] CPU MIPS monociclo em FPGA
- [ ] Pong rodando em hardware
---

## Licença

Projeto para fins acadêmicos, didáticos e de pesquisa.

