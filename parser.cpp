#include "parser.h"
#include <stdexcept>
#include <utility> // For std::move
#include <iostream>

// AST Node Constructors
NumberExpr::NumberExpr(double v): value(v) {}
std::unique_ptr<Expr> NumberExpr::clone() const {
    return std::make_unique<NumberExpr>(value);
}

VariableExpr::VariableExpr(const std::string& n): name(n) {}
std::unique_ptr<Expr> VariableExpr::clone() const {
    return std::make_unique<VariableExpr>(name);
}

BinaryExpr::BinaryExpr(char o, std::unique_ptr<Expr> l, std::unique_ptr<Expr> r): op(o), left(std::move(l)), right(std::move(r)) {}
std::unique_ptr<Expr> BinaryExpr::clone() const {
    return std::make_unique<BinaryExpr>(op, left->clone(), right->clone());
}


// Parser Implementation
Parser::Parser(std::vector<Token> toks): tokens(std::move(toks)) {
    for(auto x: tokens){
        std::cout << "-> " <<x.text << std::endl;
    }
}

Token Parser::peek() {
    return (pos < tokens.size()) ? tokens[pos] : Token{TokenType::END, ""}; 
}
Token Parser::get() { return (pos < tokens.size()) ? tokens[pos++] : Token {TokenType::END, ""}; }

std::unique_ptr<Expr> Parser::parseExpression() {
    auto expr = parseTerm();
    while(peek().type == TokenType::PLUS || peek().type == TokenType::MINUS ) {
        char op = get().text[0];
        auto rhs = parseTerm();
        expr = std::make_unique<BinaryExpr>(op, std::move(expr), std::move(rhs));
    }
    return expr;
}

std::unique_ptr<Expr> Parser::parseTerm() {
    auto expr = parseFactor();
    while(peek().type == TokenType::MUL || peek().type == TokenType::DIV){
        char op = get().text[0];
        auto rhs = parseFactor();
        expr = std::make_unique<BinaryExpr>(op, std::move(expr), std::move(rhs));
    }
    return expr;
}

std::unique_ptr<Expr> Parser::parseFactor(){
    return parsePower();
}

std::unique_ptr<Expr> Parser::parsePower() {
    auto base = parsePrimary();
    if(peek().type == TokenType::POW){
        get(); // consume ^
        auto exponent = parseFactor();
        return std::make_unique<BinaryExpr>('^', std::move(base), std::move(exponent));
    }
    return base;
}

std::unique_ptr<Expr> Parser::parsePrimary() {
    Token tok = get();
    if(tok.type == TokenType::NUMBER){
        return std::make_unique<NumberExpr>(std::stod(tok.text));
    }
    if (tok.type == TokenType::IDENT){
        return std::make_unique<VariableExpr>(tok.text);
    }
    if (tok.type == TokenType::LPAREN) {
        auto expr = parseExpression();
        if(peek().type != TokenType::RPAREN) {
            throw std::runtime_error("Expected )");
        }
        get();
        return expr;
    }
    throw std::runtime_error("Unexpected token in primary: " + tok.text);
}

// Expression Printer
std::string printExpr(const Expr* e) {
    if(auto num = dynamic_cast<const NumberExpr*>(e)) {
        return std::to_string((int)num->value);
    }
    if(auto var = dynamic_cast<const VariableExpr*>(e)) {
        return var->name;
    }
    if(auto bin = dynamic_cast<const BinaryExpr*>(e)) {
        std::string left = printExpr(bin->left.get());
        std::string right = printExpr(bin->right.get());

        switch(bin->op) {
            case '+': case '-':
                return "(" + left + " " + bin->op + " " + right + ")";
            case '*': case '/':
                return "(" + left + " " + bin->op + " " + right + ")";
            case '^':
                return left + "^" + right;
        }
    }
    return "?";
}

std::unique_ptr<Expr> simplifyBinary(char op, std::unique_ptr<Expr> left, std::unique_ptr<Expr> right){
    auto leftNum = dynamic_cast<NumberExpr*>(left.get());
    auto rightNum = dynamic_cast<NumberExpr*>(right.get());

    switch(op){
        case '+':
            if(leftNum && leftNum->value == 0) return (right);
            if(rightNum && rightNum->value == 0) return (left);
            // if(leftNum && leftNum->value == 1) return std::move(right);
            // if(rightNum && rightNum->value == 1) return std::move(left);
            if(leftNum && rightNum){
                return std::make_unique<NumberExpr>(leftNum->value+rightNum->value);
            }
            break;
        case '-':
            if(leftNum && leftNum->value == 0) return (right);
            if(rightNum && rightNum->value == 0) return (left);
            // if(leftNum && leftNum->value == 1) return std::move(right);
            // if(rightNum && rightNum->value == 1) return std::move(left);
            if(leftNum && rightNum){
                return std::make_unique<NumberExpr>(leftNum->value-rightNum->value);
            }
            break;
        case '*':
            if(leftNum && leftNum->value == 0) return std::make_unique<NumberExpr>(0);
            if(rightNum && rightNum->value == 0) return std::make_unique<NumberExpr>(0);
            if(leftNum && leftNum->value == 1) return (right);
            if(rightNum && rightNum->value == 1) return (left);
            if(leftNum && rightNum){
                return std::make_unique<NumberExpr>(leftNum->value*rightNum->value);
            }
            break;
        case '/':
            if(leftNum && leftNum->value == 0) return std::make_unique<NumberExpr>(0);
            if(rightNum && rightNum->value == 0) return std::make_unique<NumberExpr>(0);
            if(leftNum && leftNum->value == 1) return (right);
            if(rightNum && rightNum->value == 1) return (left);
            if(leftNum && rightNum){
                return std::make_unique<NumberExpr>(leftNum->value/rightNum->value);
            }
            break;
        case '^':
            if(rightNum && rightNum->value == 0) return std::make_unique<NumberExpr>(1);
            if(rightNum && rightNum->value == 1) return (left);
            break;
    }
    return std::make_unique<BinaryExpr>(op, std::move(left), std::move(right));
}
// Add this function after printExpr

std::unique_ptr<Expr> simplify(const Expr* e){
    if(auto num = dynamic_cast<const NumberExpr*>(e)){
        return std::make_unique<NumberExpr>(num->value);
    }
    if(auto var = dynamic_cast<const VariableExpr*>(e)){
        return std::make_unique<VariableExpr>(var->name);
    }
    if(auto bin = dynamic_cast<const BinaryExpr*>(e)){
        // switch(bin->op):
        auto left = simplify(bin->left.get());
        auto right = simplify(bin->right.get());

        return simplifyBinary(bin->op, std::move(left), std::move(right));
    }

    return e->clone();
}

std::string printSimplified(const Expr* e) {
    if (auto num = dynamic_cast<const NumberExpr*>(e)) {
        if (num->value == (int)num->value) {
            return std::to_string((int)num->value);
        }
        return std::to_string(num->value);
    }
    
    if (auto var = dynamic_cast<const VariableExpr*>(e)) {
        return var->name;
    }
    
    if (auto bin = dynamic_cast<const BinaryExpr*>(e)) {
        std::string left = printSimplified(bin->left.get());
        std::string right = printSimplified(bin->right.get());
        
        switch(bin->op) {
            case '+':
                return left + " + " + right;
            case '-':
                return left + " - " + right;
            case '*':
                // Special formatting for multiplication
                if (auto leftNum = dynamic_cast<const NumberExpr*>(bin->left.get())) {
                    if (leftNum->value == 1) return right;
                    return std::to_string((int)leftNum->value) + "*" + right;
                }
                return left + "*" + right;
            case '/':
                return left + "/" + right;
            case '^':
                return left + "^" + right;
        }
    }
    
    return "?";
}

std::string simplifyAndPrint(const Expr* e) {
    auto simplified = simplify(e);
    return printSimplified(simplified.get());
}
