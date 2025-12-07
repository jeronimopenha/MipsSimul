#include <asm_registers.h>

using namespace std;

int regNumber(const string &name) {
    for (int i = 0; i < 32; i++) {
        if (REG_NAME[i] == name) {
            return i;
        }
    }
    throw runtime_error("Unknown register: " + name);
}

string regNameFromNumber(const int n) {
    return REG_NAME[n];
}
