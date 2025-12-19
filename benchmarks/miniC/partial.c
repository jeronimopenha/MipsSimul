// ===== globals (testa global_decl chooser) =====
int G;
int GA[4];
int GM[2][0x3];
int *gp;        // precisa do ptr_opt em decl_item (se ainda não tiver, comente)
float GF;

// ===== funcs =====
int id(int x) {
    return x;
}

int sum2(int a, int b) {
    return a + b;
}

// param com []
int pick(int v[], int i) {
    return v[i];
}

// retorno ponteiro + param ponteiro
int* retptr(int *p) {
    return p;
}

// main com argc/argv[] pra testar param_dims_opt
int main(int argc, int argv[90]) {
    // decl local: sem init, com init, com ptr, com arrays dec/hex
    int a, b = 2, c;
    int v[10][0x4];
    int i = 0, sum = 0;
    int *p;          // precisa ptr_opt em decl_item
    int *q;          // ponteiro
    int **pp;        // ponteiro de ponteiro

    ;  // expr_stmt vazio

    // precedência/aritimética/unário
    a = 1 + 2 * 3 - (4 / 2) + (7 % 3);
    c = -a + (b + 1);

    // indexação (postfix) + lvalue IndexNode
    v[0][1] = a;
    v[i][1] = v[0][1] + 1;

    // &, *, assign
    q = &a;
    p = retptr(q);
    *p = 10;

    // usa globals + index global
    G = id(a);
    GA[1] = sum2(G, 5);
    GM[1][2] = GA[1];

    // chamadas: 1 arg, 2 args, N args com expr e index como argumento
    a = id(a);
    b = sum2(a, 5);
    sum = pick(v[0], 1);

    // INDEX em cima de CALL
    a = retptr(p)[0];

    // CALL em cima de CALL (encadeado)
    p = retptr(retptr(p));

    // assignment encadeado (right associative)
    a = b = c = i;

    a = !(a == 0);

    // boolean/rel/eq/&&/||
    while (i < 10 && (a != 0 || b == 2)) {

        if (i < 3 || (i >= 7 && i < 9)) {
            sum = sum + v[0][1];
        } else {
            sum = sum + i;
        }

        // também testa index com variável
        v[i][0] = sum;

        // continue
        if (i == 5) {
            i = i + 1;
            continue;
        }

        // break
        if (sum > 100) {
            break;
        }

        i = i + 1;
    }

    return sum;
}
