#ifndef LEXER_H
#define LEXER_H

#include <string>

enum TokenType {
    NUMBER,
    IDENT,
    PLUS,
    MINUS,
    MUL,
    DIV,
    POW,
    LPAREN,
    RPAREN,
    END
};

struct Token {
    TokenType type;
    std::string text;
};

class Lexer {
    std::string input;
    size_t pos = 0;

public:
    Lexer(const std::string& src);
    Token nextToken();

private:
    char peek();
    char get();
    void skipWhiteSpace();
};

#endif // LEXER_H
