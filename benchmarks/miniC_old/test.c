// Mini-C lexer test file
// Testa: int, float, ponteiros, arrays, if/else, while, operadores, comentários, literais

/* Variáveis globais */

int g_int = 42;
int g_vec[3];
int *HW = 0x80000000;   // hardware mapeado

float g_f0 = 0.0;
float g_f1 = 1.0;
float g_f2 = 3.14;
float g_f3 = 10.;
float g_f4 = .5;
float g_f5 = 2.5f;
float g_f6 = 1.0e3;
float g_f7 = 6.02E23f;
float g_f8 = 6.02E+23f;
float g_f9 = 6.02E+23f;

/* Builtins (apenas nomes para o lexer) */

int  read_int();
void print_int(int x);
void led_write(int x);
int  led_read();
void uart_write(int x);
int  uart_read();

/* Função auxiliar com int e ponteiro */

int soma_vetor(int *v, int n) {
    int i;
    int s;
    i = 0;
    s = 0;

    while (i < n) {
        s = s + v[i];
        i = i + 1;
    }

    return s;
}

/* Função com float, operadores e comparações */

float processa_float(float a, float b) {
    float x;
    float y;
    float z;

    x = a + b;
    y = a - b;
    z = a * b / 1.0;

    // Comparações com todos os operadores relacionais
    if (x == y) {
        z = 0.0;
    } else {
        if (x != y && x >= z) {
            z = 3.14;
        } else {
            if (x < y || z <= 10.) {
                z = .5;
            }
        }
    }

    return z;
}

/* Função com mistura de int/float e lógica */

int testa_logica(int a, int b) {
    int r;
    int tmp;

    tmp = a % 3;
    r = 0;

    if (tmp == 0 && !(a < b)) {
        r = 1;
    } else {
        if (tmp != 0 || a > b) {
            r = 2;
        }
    }

    return r;
}

/* main – usa de tudo um pouco */

int main() {
    int x;
    int y;
    int v[4];
    int *p;

    float fa;
    float fb;
    float fc;

    x = read_int();
    y = 1024;

    v[0] = x;
    v[1] = y;
    v[2] = 1;
    v[3] = 0;

    p = &v[0];
    x = soma_vetor(p, 4);

    fa = 2.5f;
    fb = 1.0e3;
    fc = processa_float(fa, fb);

    led_write(x);
    uart_write(x);
    print_int(testa_logica(x, y));

    *HW = 1;

    return 0;
}
