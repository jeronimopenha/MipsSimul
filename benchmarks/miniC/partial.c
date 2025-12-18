{
    //;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //;;; MiniC parser stress test (stmt + expr). Assumes control stmts need {} ;;;
    //;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    ;                       /* empty expr_stmt */
    123;                    /* int literal */
    0x2A;                   /* hex literal */
    3.14159;                /* float literal */
    (1 + 2) * (3 - 4) / 5 % 2;

    a;
    b;
    c;

    /* unary ops (+, -, !, *, &) + precedence */
    +a;
    -a;
    !a;
    *p;
    &a;
    *(&a);

    /* postfix indexing on primary_expr: ident[expr] and chaining */
    arr[0];
    arr[i + 1];
    mat[i][j + 2];

    /* lvalue variants: ident, *unary_expr, (lvalue), and with indexing */
    a = 1;
    a = b = c = 7;
    arr[i] = 10;
    mat[i][j] = mat[i][j] + 1;
    (*p) = 99;
    ((*p)) = 100;
    (*p) = (*p) + 1;
    (*p) = (a + b) * (c - 2);

    /* assignment to indexed lvalue, plus nested expr inside index */
    arr[i + (j * 2) - 1] = 123;
    mat[(i + 1)][(j + 2) * 3] = 0x10;

    /* relational + equality */
    (a < b);
    (a <= b);
    (a > b);
    (a >= b);
    (a == b);
    (a != b);

    /* logical and/or chaining */
    (a < b) && (b < c) && (c < d);
    (a < b) || (b < c) || (c < d);
    ((a < b) && (b < c)) || ((c < d) && (d < e));

    /* NOT with grouped expr */
    !(a == b);
    !((a < b) || (b < c));

    /* parentheses everywhere */
    (((a)));
    (a + (b * (c + (d - 1))));

    /* IF (with mandatory compound blocks) */
    if (a) {
        x = 1;
        y = 2;
        ;
        (x + y);
    }

    if ((a < b) && (b < c)) {
        z = (a + b) * (c - 2);
        mat[i][j] = z;
    } else {
        z = 0;
        mat[i][j] = z;
        ;
    }

    /* ELSE IF chain (else contains a compound which contains another if) */
    if (a == 0) {
        r = 0;
    } else {
        if (a == 1) {
            r = 10;
        } else {
            if (a == 2) {
                r = 20;
            } else {
                r = 30;
            }
        }
    }

    /* WHILE (body is compound) */
    while (i < 10) {
        i = i + 1;
        sum = sum + i;
        arr[i] = sum;
    }

    /* nested while + if */
    while (i < n) {
        j = 0;
        while (j < m) {
            if ((i + j) % 2 == 0) {
                mat[i][j] = mat[i][j] + 1;
            } else {
                mat[i][j] = mat[i][j] - 1;
            }
            j = j + 1;
        }
        i = i + 1;
    }

    /* break/continue (syntactic only; semantic check later) */
    while (k < 100) {
        k = k + 1;
        if (k == 10) {
            continue;
        } else {
            ;
        }
        if (k == 20) {
            break;
        } else {
            ;
        }
        acc = acc + k;
    }

    /* return without expr */
    return;

    /* unreachable (still useful for parsing) */
    return 0;
}

int a;
int a, b, c;
int a = 1;
float x = 3.14;
int v[10];
int m[3][4];
int v[10] = 0;
float y = a + 2 * b;
