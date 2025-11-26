# Mini-C – Especificação v0.1

Mini-C é uma linguagem C reduzida projetada para:

- Compilar para MIPS (simulador e FPGA)
- Não depender de libc
- Suportar ponteiros, vetores, recursão e hardware mapeado

---

## 1. Restrições Gerais

- Um único arquivo fonte
- Não existe `#include`, `#define` ou pré-processador
- Tudo deve estar no próprio código

---

## 2. Tipos Suportados

```
int
void
int*
int[]
```

Não existem:

```
float
double
char
struct
enum
typedef
```
---

## 3. Variáveis

**Globais**:

```
int x;
int v[10];
int *p;
```

**Locais**:
```
int a;
int b[8];
int *q;
```

---

## 4. Ponteiros

```
p = &x;
p = v;
*p = 42;
x = *p;
int *HW = 0x80000000;
*HW = 1;
```

---

## 5. Expressões

```
/ %
== != < <= > >=
&& ||
=
[]
()
```
---

## 6. Controle de Fluxo

### IF (sempre com bloco)

```
if (cond) {
...
} else {
...
}
```

### WHILE (sempre com bloco)

```
while (cond) {
...
}
```

Não existem:

```
for
do
switch
break
continue
```

---

## 7. Funções

```
int soma(int a, int b)
void foo(int *p)

int main() { ... }
```
---

## 8. Funções Builtin (backend)

```
int read_int();
void print_int(int x);
void led_write(int x);
int led_read();
void uart_write(int x);
int uart_read();
```
---

## 9. Comentários

```
// Linha
/* Bloco */
```