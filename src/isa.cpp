#include <isa.h>

using namespace std;

const InstrDesc* findInstrByName(const std::string& name) {
    for (const auto& d : ISA_TABLE) {
        if (d.name == name) return &d;
    }
    return nullptr;
}
