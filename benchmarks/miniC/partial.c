{
    int a, b = 2, c;
    int v[10][0x4];
    int i = 0, sum = 0;

    ;  // expr_stmt vazio

    // aritmética + precedência + parênteses
    a = 1 + 2 * 3 - (4 / 2) + (7 % 3);

    // indexação (postfix) + lvalue IndexNode
    v[0][1] = a;

    // lvalue com '*' (UnaryOpNode TOK_STAR) + assign
    *p = 10;

    while (i < 10 && (a != 0 || b == 2)) {
        // if/else
        if (i < 3 || (i >= 7 && i < 9)) {
            sum = sum + v[0][1];
        } else {
            sum = sum + i;
        }

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
