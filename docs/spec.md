# Mini-C – Especificação v0.2 (com float)

Mini-C é uma linguagem C reduzida projetada para:
- Compilar para sua ISA MIPS-like (simulador e FPGA)
- Não depender de libc padrão
- Suportar ponteiros, vetores, recursão e acesso a hardware mapeado em memória

1. **Restrições Gerais**

   - Um único arquivo fonte
   - Não existe `#include`, `#define` ou pré-processador
   - Tudo deve estar no próprio código
   - Não há casts explícitos ainda `((int)x`, `(float)x` etc. não existem por enquanto)

2. **Tipos Suportados**
    
    **Tipos base**:
   
    ```
    int      // 32 bits com sinal
    float    // IEEE 754 single, 32 bits
    void
    ```

    **Ponteiros**:

    ```
    int   *p;
    float *q;
    void  *ptr;   // se você quiser permitir, pode deixar ou cortar
    ```

    **Vetores (somente de tipos base ou ponteiros, se você quiser)**:
    
    ```
    int   v[10];
    float f[8];
    int   *tab[4];  // opcional
    ```

    **Não existem (por enquanto)**:
    
    ```
    double
    char
    struct
    enum
    typedef
    union
    long long
    ```

    **Regra de simplificação semântica (para depois do lexer)**:
    
    Para v0.2 operações binárias são feitas entre operandos do mesmo tipo:
    
    ```
    int op int → ok
    float op float → ok
    int op float → inicialmente proibido.
    ```

3. **Variáveis**

    **Globais**:
    
    ```
    int x;
    int v[10];
    int *p;
    
    float g;
    float F[16];
    float *pf;
    ```
    
    **Locais**:

    ```
    int a;
    int b[8];
    int *q;
    
    float t;
    float buf[4];
    ```
4. **Ponteiros**

    Mesma ideia da v0.1, agora também com float:
    
    ```
    int x;
    int v[10];
    int *p;
    
    p = &x;
    p = v;
    *p = 42;
    x = *p;
    
    //int *HW = (int*)0x80000000;  // sintaxe de cast ainda não existe
    int *HW = 0x80000000;        

    *HW = 1;
    
    float y;
    float *pf;
    pf = &y;
    *pf = 3.14f;
    ```

5. **Expressões e Operadores**

    **Operadores binários suportados**:

    ```
    +  -  *  /  %        // % só para int
    == != < <= > >=
    && ||
    =                  // atribuição
    [ ]                // acesso a array
    ()                 // chamada de função
    ```

    **Operadores unários suportados**:

    ```
   -   // unário (negativo)
    !   // not lógico
    &   // endereço
   *   // desreferência
    ```

6. **Controle de Fluxo**
   
    IF (sempre com bloco)

    ```
    if (cond) {
    ...
    } else {
    ...
    }
    ```
    WHILE (sempre com bloco)

     ```
    while (cond) {
    ...
    }
     ```

    **Não existem**:

     ```
    for
    do
    switch
    break
    continue
    goto
     ```

7. **Funções**

    Declarações e definições:

    ```
    int soma(int a, int b) {
    ...
    }

    void foo(int *p) {
    ...
    }
    
    float media(float a, float b) {
    return (a + b) / 2.0f;
    }
    
    int main() {
    ...
    }
     ```
    
    Parâmetros podem ser int, float, ponteiros e vetores (se você quiser permitir).
    
    main deve ser:
     ```
    int main() { ... }
    ```

8. **Funções Built-in (fornecidas pelo backend)**

    Assinaturas fixas:

    ```
    int  read_int();
    void print_int(int x);
    float read_float();      
    void  print_float(float x);
    void led_write(int x);
    int  led_read();
    void uart_write(int x);
    int  uart_read();
    ```



9. **Comentários**

    Comentário de linha:
    
    ```
    // até o fim da linha
    ```
    
    Comentário de bloco:

    ```
    /* até encontrar o */
    ```
    
    
NÃO é permitido aninhar comentários de bloco.

10. **Especificação Léxica (para o LEXER)**
    
    10.1. Palavras reservadas (keywords)

    O lexer deve reconhecer essas como tokens de palavra-chave, não como identificadores:
        
        ```
        int
        float
        void
        if
        else
        while
        return
        ```

    10.2. **Identificadores**

    Regra padrão C simplificada:
        
    - Primeiro caractere: `[A-Za-z_]`
    - Restante: `[A-Za-z0-9_]*`
        
    Não há limite de tamanho na especificação, mas você pode impor um no compilador.
        
    Exemplos válidos:
    ```
    x
    v1
    _soma
    LED_PORT
    main
    read_int
    __mc_fadd
    ```
    10.3. **Literais inteiros**
        
    Formato sugerido (simples, mas já útil):
        
    Decimal:
    - `[0-9]+`
        
    Opcionalmente, hexadecimal:
    - `0x[0-9A-Fa-f]+`
        
    Sem sufixos (u, l, etc.) por enquanto.
        
    Exemplos válidos:
    ```       
    0
    1
    42
    1024
    0x80000000
    0xff
    0XDEAD
    ```

    10.4. Literais de ponto flutuante

    Para v0.2, são suportados suportar:
        
    Notação decimal com ponto:
     ```   
    123.0
    0.5
    .5
    10.
    ```
    
    Opcionalmente, expoente: 
    - `1.0e10`
    - `3.14E-2`
    Sufixo opcional f ou F (como em C), mas sem obrigação:
    - `3.14f`
    - `0.5F`
    Gramática aproximada pro lexer (modo leitura humana):
    - Caso 1: `dígitos '.' [dígitos] [expoente] [sufixo]`
    - Caso 2: `'.' dígitos [expoente] [sufixo]`
    - Opcional `[eE][+-]?dígitos` pro expoente
    - Opcional `[fF]` no final
    
    Exemplos válidos:
    ```
    0.0
    1.0
    3.14
    10.
    .5
    2.5f
    1.0e3
    6.02E23f
    ```
    10.5. **Símbolos / Operadores / Pontuação**
        
    O lexer deve gerar tokens distintos para cada operador e pontuação:
        
    Operadores aritméticos e lógicos:
    ```      
    +    -    *    /    %
    ==   !=   <    <=   >    >=
    &&   ||
    !    =
    ```
        
    Operadores / símbolos de ponteiro/array/chamada:
    ```
    &    *    [    ]    (    )
    ```

    Pontuação:

    ```   
    ;    ,    {    }
    ```
        
    Importante pro lexer:
    
    Operadores de dois caracteres devem ser lidos primeiro:
        
        ==, !=, <=, >=, &&, ||
        
    Depois os de um caractere:
        
        =, &, |, <, >, !, etc.
        
    10.6. Espaços em branco
        
    São ignorados, exceto para separar tokens:
        
    - espaço: `' '`
    - tab: `'\t'`
    - newline: `'\n', '\r'` 

11. **Resumo rápido pro LEXER (checklist)**

    O lexer precisa reconhecer:
    
    Keywords:

    ```
    int, float, void, if, else, while, return
    ```
    
    Identificadores:
    ```
    [A-Za-z_][A-Za-z0-9_]*
    ```
    
    Literais inteiros:

    ```    
    [0-9]+ e opcional 0x[0-9A-Fa-f]+
    ``` 
    
    Literais float (v0.2 mínima):
    
    dígitos `'.'`  dígitos
    
    Operadores de 2 chars:
    ```
    ==, !=, <=, >=, &&, ||
    ```
    
    Operadores de 1 char:
    ```
    + - * / % < > ! = &
    ```
    
    Pontuação:
    ```
    ; , ( ) { } [ ]
    ```
    
    Comentários:
    
    ```
    // ... até fim da linha
    /* ... */ bloco, sem aninhar
    ```
    
    Whitespace:
    ignorado (espaços, tabs, quebras de linha)
