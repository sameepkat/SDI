#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include <vector>
#include <memory>
#include <string>

struct Expr {
    virtual ~Expr() = default;
    virtual std::unique_ptr<Expr> clone() const = 0;
};

struct NumberExpr: Expr {
    double value;
    NumberExpr(double v);
    std::unique_ptr<Expr> clone() const override;
};

struct VariableExpr: Expr {
    std::string name;
    VariableExpr(const std::string& n);
    std::unique_ptr<Expr> clone() const override;
};

struct BinaryExpr: Expr {
    char op;
    std::unique_ptr<Expr> left, right;
    BinaryExpr(char o, std::unique_ptr<Expr> l, std::unique_ptr<Expr> r);
    std::unique_ptr<Expr> clone() const override;
};

class Parser {
    std::vector<Token> tokens;
    size_t pos = 0;

public:
    Parser(std::vector<Token> toks);

    std::unique_ptr<Expr> parseExpression();

private:
    Token peek();
    Token get();
    std::unique_ptr<Expr> parseTerm();
    std::unique_ptr<Expr> parseFactor();
    std::unique_ptr<Expr> parsePower();
    std::unique_ptr<Expr> parsePrimary();
};

std::string printExpr(const Expr* e);
std::string printSimplified(const Expr* e);
std::unique_ptr<Expr> simplifyBinary(char op, std::unique_ptr<Expr> left, std::unique_ptr<Expr> right);
std::string simplifyAndPrint(const Expr* e);

#endif // PARSER_H
