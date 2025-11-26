# Runtime – Suporte ao Mini-C e ao Sistema Bare-Metal

O Runtime fornece funções em assembly que dão suporte ao Mini-C sem usar libc.

---

## 1. Objetivo

- Substituir libc
- Fornecer I/O
- Fornecer inicialização
- Fornecer temporização

---

## 2. Funções Builtin

Implementadas em assembly:

```
read_int
print_int
print_char
print_newline
led_write
led_read
uart_write
uart_read
wait_vblank
```
---

## 3. Exemplo: print_int.s

```
print_int:
    # recebe valor em $a0
    # escreve via UART mapeada
    jr $ra
```

## 4. Startup

Arquivo startup.s:

```
inicializa stack ($sp)

zera .bss

chama main

entra em loop infinito após retorno
```

## 5. Compatibilidade
O runtime funciona:
- no simulador
- na FPGA real
- Os endereços de hardware são definidos no backend.

## 6. Extensão Futura
No futuro pode incluir:
- drivers
- sistema de arquivos
- multitarefa
- interrupções

---
