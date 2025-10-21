#include <iostream>
#include <cassert>
#include "lexer.h"
#include "parser.h"

void test_simple_function() {
    std::string source = "int main() { return 0; }";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.tokenize();
    
    Parser parser(tokens);
    auto program = parser.parse();
    
    assert(program != nullptr);
    assert(program->declarations.size() == 1);
    assert(program->declarations[0]->type == ASTNodeType::FUNCTION_DEF);
    
    std::cout << "test_simple_function passed\n";
}

void test_function_with_params() {
    std::string source = "int add(int a, int b) { return a + b; }";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.tokenize();
    
    Parser parser(tokens);
    auto program = parser.parse();
    
    assert(program != nullptr);
    assert(program->declarations.size() == 1);
    
    FunctionDef* func = dynamic_cast<FunctionDef*>(program->declarations[0].get());
    assert(func != nullptr);
    assert(func->name == "add");
    assert(func->params.size() == 2);
    
    std::cout << "test_function_with_params passed\n";
}

void test_if_statement() {
    std::string source = "int main() { if (x > 0) return 1; else return 0; }";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.tokenize();
    
    Parser parser(tokens);
    auto program = parser.parse();
    
    assert(program != nullptr);
    
    std::cout << "test_if_statement passed\n";
}

void test_while_statement() {
    std::string source = "int main() { int x; x = 0; while (x < 10) { x = x + 1; } return x; }";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.tokenize();
    
    Parser parser(tokens);
    auto program = parser.parse();
    
    assert(program != nullptr);
    
    std::cout << "test_while_statement passed\n";
}

void test_expressions() {
    std::string source = "int main() { int x; x = 1 + 2 * 3; return x; }";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.tokenize();
    
    Parser parser(tokens);
    auto program = parser.parse();
    
    assert(program != nullptr);
    
    std::cout << "test_expressions passed\n";
}

int main() {
    std::cout << "Running Parser Tests...\n";
    test_simple_function();
    test_function_with_params();
    test_if_statement();
    test_while_statement();
    test_expressions();
    std::cout << "All parser tests passed!\n";
    return 0;
}
