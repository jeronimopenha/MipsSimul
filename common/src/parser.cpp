#include <parser.h>

const Token &Parser::peek() const {
    if (pos >= tokens.size())
        return tokens.back();
    return tokens[pos];
}

const Token &Parser::get() {
    const Token &t = peek();
    if (pos < tokens.size()) {
        ++pos;
    }
    return t;
}

bool Parser::match(const int kind) {
    if (peek().kind == kind) {
        get();
        return true;
    }
    return false;
}

const Token &Parser::expect(const int kind, const std::string &msg) {
    if (peek().kind != kind) {
        std::cerr << "Error on line " << peek().line
                << ": expected " << msg << " (got kind=" << peek().kind << ")\n";
        throw std::runtime_error("parsing error");
    }
    return get();
}

bool Parser::eof() const {
    return pos >= tokens.size();
}
