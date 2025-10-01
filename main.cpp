#include "parser.h"
#include "lexer.h"
#include "differentiator.h"
#include <iostream>
#include <string>
#include <vector>

int main() {
    std::cout << "Enter an expression: ";
    std::string line;
    if (!std::getline(std::cin, line)) {
        return 1;
    }

    std::cout << "Differentiate with respect to: ";
    std::string var;
    if(!std::getline(std::cin, var)){
        return 1;
    }

    try {
        Lexer lexer(line);

        std::vector<Token> tokens;
        for(Token t = lexer.nextToken(); t.type != TokenType::END; t = lexer.nextToken()){
            tokens.push_back(t);
        }


        Parser parser(tokens);
        std::unique_ptr<Expr> ast = parser.parseExpression();

        std::cout << "Parsed expression AST: " << printExpr(ast.get()) << "\n";

        auto derivative = differentiate(ast.get(), var);
        std::cout << "Derivative: " << simplifyAndPrint(derivative.get()) << "\n";

    } catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
