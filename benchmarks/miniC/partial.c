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
