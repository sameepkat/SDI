 #include "lexer.h"
#include <stdexcept>
#include <cctype>
#include <iostream>

Lexer::Lexer(const std::string& src) : input(src) {}

char Lexer::peek() {
	return (pos < input.size() ? input[pos] : '\0');
}

char Lexer::get() {
    return (pos < input.size() ? input[pos++] : '\0');
}

void Lexer::skipWhiteSpace(){
	while(isspace(peek())) get();
}

Token Lexer::nextToken() {
	skipWhiteSpace();
	char c = peek();

	if(isdigit(c)) {
		std::string num;
		while(isdigit(peek())) num += get();
		return {TokenType::NUMBER, num};
	}

	if(isalpha(c)) { // IDENT
		std::string ident;
		while(isalnum(peek())) ident += get();
		return {TokenType::IDENT, ident};
	}

	switch(c) {
		case '+': get(); return {TokenType::PLUS, "+"};
		case '-': get(); return {TokenType::MINUS, "-"};
		case '*': get(); return {TokenType::MUL, "*"};
		case '/': get(); return {TokenType::DIV, "/"};
		case '^': get(); return {TokenType::POW, "^"};
		case '(': get(); return {TokenType::LPAREN, "("};
		case ')': get(); return {TokenType::RPAREN, ")"};
		case '\0': return {TokenType::END, "EOF"};
	}

	throw std::runtime_error(std::string("Unexpected character: ") + c);
}


// int main(){
// 	std::string input = "2+x^2+5";
// 	Lexer lex(input);
// 	do{
// 		auto current = lex.nextToken();
// 		std::cout << current.text << std::endl;
// 		if(current.type == TokenType::END)	{
// 			break;
// 		}
// 	}while(true);
// 	return 0;
// }