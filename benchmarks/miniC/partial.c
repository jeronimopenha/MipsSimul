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

10
10.0
.5
5f
0x10101a
-10
-10.0
-.5
-5f
0x10101a
*a
-b
&c
!avocado
!1

// ===== primary: ints, hex, floats =====
42;
0x2A;
3.14;
10.;
.5;
2.5f;
1.0e3;
6.02E23;

// ===== primary: ident + parentheses =====
x;
HW;
(x);
((x));
(((42)));
((0x10));
(3.14);

// ===== unary: + - ! * & =====
-10;
+10;
--10;          // equivale a -(-10) (na sua gramática atual)
-(-10);
!x;
!!x;
*&x;
*(&x);

// ===== mul/div/mod (associatividade à esquerda) =====
a*b;
a/b;
a%3;
a*b/c%2;
a/b/c;         // deve virar ((a/b)/c)
a%b%7;         // deve virar ((a%b)%7)

// ===== add/sub (associatividade à esquerda) =====
a+b;
a-b;
a-b-c;         // deve virar ((a-b)-c)
a+b-c+d;       // deve virar (((a+b)-c)+d)

// ===== precedência mul > add =====
a + b*c;
a*b + c;
a + b*c - d/e;
(a + b) * (c - d);
-(a*b) + (c/2);

// ===== floats misturados =====
(1.0e3) * (.5);
-(6.02E23) / (10.);
(2.5 + 1.0) * 3.0;

// ===== combinações com hex/int =====
(0x10) * (-3);
0x20 + 4*2;
(0x2A - 10) + (3 * 7);

// ===== relational (< <= > >=) =====
a < b;
a <= b;
a > b;
a >= b;

// precedência: add/mul antes de rel
a + 1 < b * 2;
(a + 1) < (b * 2);
a * 2 <= b + 3;
(a*b) > (c/d);

// com unário
-a < b;
!(a < b);          // unary '!' aplicado a um rel via parênteses
(-a) >= (+b);

// com literais int/hex/float (SEM sufixo f/F)
0 < 1;
0x10 >= 15;
3.14 < 10.;
.5 <= 1.0e3;
-(6.02E23) > 0.0;

// cadeia (C aceita; bom pra ver AST e associatividade)
a < b < c;


// ===== equality (== !=) =====
a == b;
a != b;

// precedência: rel antes de eq
a < b == c < d;
(a < b) == (c < d);

// mistura com add/mul
a + 1 == b * 2;
a * 2 != b + 3;
(a*b) == (c/d);

// com unário e parênteses
!(a == b);
!((a < b) != (c >= d));

// literais (SEM sufixo f/F)
0 == 0;
0 != 1;
0x10 == 16;
3.14 != 10.;
.5 == 0.5;
1.0e3 == 1000.0;

// cadeia (só pra ver associatividade na AST)
a == b == c;
a != b != c;
