#include <parser.h>

using namespace std;

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

const Token &Parser::expect(const int kind, const string &msg) {
    if (peek().kind != kind) {
        cerr << "Error on line " << peek().line
                << ": expected " << msg << " (got kind=" << peek().lexeme << ")\n";
        throw runtime_error("parsing error");
    }
    return get();
}

bool Parser::eof() const {
    return pos >= tokens.size();
}

const Token &Parser::previous() const {
    return tokens[pos - 1];
}

bool Parser::check(const int kind) const {
    if (eof()) {
        return false;
    }
    return peek().kind == kind;
}

bool Parser::match(const initializer_list<int> kinds) {
    for (const int k: kinds) {
        if (check(k)) {
            get();
            return true;
        }
    }
    return false;
}

void Parser::error(const Token &token, const string &msg) {
    ostringstream oss;
    oss << "Parser error at line " << token.line
            << ", col " << token.col << ": " << msg
            << " (found '" << token.lexeme << "')";
    throw runtime_error(oss.str());
}
