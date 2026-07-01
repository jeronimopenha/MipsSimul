#ifndef ASSEMBLER_AST_H
#define ASSEMBLER_AST_H

#include <definitions.h>

inline void printIndent(const int n) {
    for (int i = 0; i < n; ++i)
        std::cout << ' ';
}

enum class AsmSection {
    NONE,
    TEXT,
    DATA
};

inline const char* asmSectionToString(const AsmSection s) {
    switch (s) {
        case AsmSection::NONE: return "NONE";
        case AsmSection::TEXT: return "TEXT";
        case AsmSection::DATA: return "DATA";
    }
    return "UNKNOWN";
}

struct AsmNode {
    virtual ~AsmNode() = default;
    virtual void dump(int indent) const = 0;
};

struct AsmOperand:AsmNode {
    enum class Kind {
        GPR, // $t0, $s1, $zero
        FPR, // $f0, $f2, $f31
        INT_IMM, // 10, -5, 0xFF
        FLOAT_IMM, // 2.5, -3.75
        MEM, // 8($gp)
        LABEL_REF // main, loop, end
    } kind;

    std::string text;

    int gpr = 0;
    int fpr = 0;

    int32_t intValue = 0;
    float floatValue = 0.0f;

    int32_t offset = 0;
    std::string baseReg;

    void dump(const int indent) const override {
        printIndent(indent);

        switch (kind) {
            case Kind::GPR:
                std::cout << "GPR " << text << "\n";
                break;

            case Kind::FPR:
                std::cout << "FPR " << text << "\n";
                break;

            case Kind::INT_IMM:
                std::cout << "INT_IMM " << intValue
                          << " text=" << text << "\n";
                break;

            case Kind::FLOAT_IMM:
                std::cout << "FLOAT_IMM " << floatValue
                          << " text=" << text << "\n";
                break;

            case Kind::MEM:
                std::cout << "MEM offset=" << offset
                          << " base=" << baseReg << "\n";
                break;

            case Kind::LABEL_REF:
                std::cout << "LABEL_REF " << text << "\n";
                break;
        }
    }
};

struct AsmStatement : AsmNode {
    void dump(int indent) const override;
};

struct AsmInstruction : AsmStatement {
    std::string mnemonic;
    std::vector<AsmOperand> args;
    int line = 0;

    void dump(const int indent) const override {
        printIndent(indent);
        std::cout << "INSTRUCTION " << mnemonic
                  << " line=" << line << "\n";

        for (const auto& arg : args) {
            arg.dump(indent + 2);
        }
    }
};

struct AsmDirective : AsmStatement {
    std::string name;
    std::vector<AsmOperand> args;
    int line = 0;

    void dump(const int indent) const override {
        printIndent(indent);
        std::cout << "DIRECTIVE " << name
                  << " line=" << line << "\n";

        for (const auto& arg : args) {
            arg.dump(indent + 2);
        }
    }
};

struct AsmEmptyStatement : AsmStatement {
    void dump(int indent) const override {
        printIndent(indent);
        std::cout << "EMPTY\n";
    }
};

struct AsmLine:AsmNode {
    std::string label; // "" if it don't have label
    AsmSection section = AsmSection::NONE;

    std::unique_ptr<AsmStatement> stmt;

    int line = 0;

    void dump(const int indent) const override {
        printIndent(indent);
        std::cout << "LINE " << line
                  << " SECTION " << asmSectionToString(section);

        if (!label.empty()) {
            std::cout << " LABEL " << label;
        }

        std::cout << "\n";

        if (stmt) {
            stmt->dump(indent + 2);
        } else {
            printIndent(indent + 2);
            std::cout << "NULL_STMT\n";
        }
    }
};

struct AsmProgram:AsmNode {
    std::vector<std::unique_ptr<AsmLine>> lines;

    void dump(const int indent) const override {
        printIndent(indent);
        std::cout << "ASM_PROGRAM\n";

        for (const auto& line : lines) {
            if (line) {
                line->dump(indent + 2);
            }
        }
    }
};


#endif
