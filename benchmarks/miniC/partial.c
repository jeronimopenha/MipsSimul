{
    // -------- DECL / LISTA / INIT / DIMS (int + hex) ----------
    int a, b = 2, c;
    int v[10][0x4];
    int i = 0, sum = 0;

    ;  // expr_stmt vazio

    // -------- PRECEDÊNCIA + PAREN + % / * / / / + / - ----------
    a = 1 + 2 * 3 - (4 / 2) + (7 % 3);

    // -------- INDEXAÇÃO ENCADEADA (a[0][1]) + LVALUE ----------
    v[0][1] = a;
    v[i][1] = v[0][1] + 1;

    // -------- UNARY: - + ! * & ----------
    c = -a + +b;
    a = !(a == 0);
    *p = 10;
    q = &a;          // só pra gerar UnaryOpNode TOK_AMP

    // -------- CHAMADAS (postfix) ----------
    f();                         // call com 0 args (arg_list?)
    g(a);                        // 1 arg
    h(1, 2+3, v[0][1]);           // vários args + expr como arg
    a = f(1)[0];                  // call seguido de index  (f(x)[0])
    b = f()(a);                   // chaining de call      (f()(x))

    // -------- WHILE + IF/ELSE + && || + REL/EQ ----------
    while (i < 10 && (a != 0 || b == 2)) {

        if (i < 3 || (i >= 7 && i < 9)) {
            sum = sum + v[0][1];
        } else {
            sum = sum + i;
        }

        // -------- LVALUE INDEX EM ASSIGN ----------
        v[i][0] = sum;

        // -------- CONTINUE ----------
        if (i == 5) {
            i = i + 1;
            continue;
        }

        // -------- BREAK ----------
        if (sum > 100) {
            break;
        }

        // -------- atribuição aninhada (direita-associativa) ----------
        a = b = c = i;

        i = i + 1;
    }

    return sum;
}
