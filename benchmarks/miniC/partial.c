//int g_int = 42;
//int g_vec[3];
//int *HW = 0x80000000;

//float g_f0 = 0.0;
/* ... outras globais ... */

//int read_int();
//void print_int(int x);
/* ... */

//int soma_vetor(int *v, int n) { ... }

//float processa_float(float a, float b) { ... }

//int testa_logica(int a, int b) { ... }

//int main() { ... }

42;
0xFF;
3.14;
x;
(42);
(a);
(a + 1);   // aqui já usa add_expr também

-42;
+42;
!flag;
*p;
&a;
-*p;
*!a;
&*p;

2 * 3 + 4;
2 + 3 * 4;
a * b / c % d;
a + b - c + d;
a + b * c - d / e;

a < b;
a <= b;
a > b;
a >= b;

a == b;
a != b;

a + 1 < b * 2;
a < b == c < d;    // checar associatividade/eval

a && b;
a || b;
a && b && c;
a || b || c;
a && b || c && d;
a < b && b < c;
a == b || c != d && e < f;

x = 1;
*p = 2;
v[i] = 3;
v[i + 1] = v[i] * 2;
(*p) = 10;
v[(a + b) * 2] = x;
p = &v[0];           // lado direito não é lvalue, mas expr
**pp = 3;            // se permitir, testa isso também

return;

//1 = x;
//3.14 = y;
//(a + b) = 10;
//(x * y) = z;
//(a < b) = 1;
//&(a) = p;       // &a não é lvalue (no seu modelo simples)
//v[1 + 2] + 3 = x;
