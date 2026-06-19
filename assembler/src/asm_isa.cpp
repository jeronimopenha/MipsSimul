#include <asm_isa.h>

using namespace std;

const AsmInstrDesc *asmFindInstrByName(const std::string &name) {
    for (const auto &d: ISA_TABLE) {
        if (d.name == name) {
            return &d;
        }
    }
    return nullptr;
}
