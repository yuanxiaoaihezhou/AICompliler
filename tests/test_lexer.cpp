#include <iostream>
#include <cassert>
#include "lexer.h"

void test_keywords() {
    std::string source = "const int void if else while break continue return";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.tokenize();
    
    assert(tokens[0].type == TokenType::CONST);
    assert(tokens[1].type == TokenType::INT);
    assert(tokens[2].type == TokenType::VOID);
    assert(tokens[3].type == TokenType::IF);
    assert(tokens[4].type == TokenType::ELSE);
    assert(tokens[5].type == TokenType::WHILE);
    assert(tokens[6].type == TokenType::BREAK);
    assert(tokens[7].type == TokenType::CONTINUE);
    assert(tokens[8].type == TokenType::RETURN);
    
    std::cout << "test_keywords passed\n";
}

void test_operators() {
    std::string source = "+ - * / % < <= > >= == != && || !";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.tokenize();
    
    assert(tokens[0].type == TokenType::PLUS);
    assert(tokens[1].type == TokenType::MINUS);
    assert(tokens[2].type == TokenType::MULT);
    assert(tokens[3].type == TokenType::DIV);
    assert(tokens[4].type == TokenType::MOD);
    assert(tokens[5].type == TokenType::LT);
    assert(tokens[6].type == TokenType::LE);
    assert(tokens[7].type == TokenType::GT);
    assert(tokens[8].type == TokenType::GE);
    assert(tokens[9].type == TokenType::EQ);
    assert(tokens[10].type == TokenType::NE);
    assert(tokens[11].type == TokenType::AND);
    assert(tokens[12].type == TokenType::OR);
    assert(tokens[13].type == TokenType::NOT);
    
    std::cout << "test_operators passed\n";
}

void test_identifiers_and_numbers() {
    std::string source = "x y123 _abc 42 0 999";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.tokenize();
    
    assert(tokens[0].type == TokenType::IDENT);
    assert(tokens[0].lexeme == "x");
    assert(tokens[1].type == TokenType::IDENT);
    assert(tokens[1].lexeme == "y123");
    assert(tokens[2].type == TokenType::IDENT);
    assert(tokens[2].lexeme == "_abc");
    assert(tokens[3].type == TokenType::INT_LITERAL);
    assert(tokens[3].value == 42);
    assert(tokens[4].type == TokenType::INT_LITERAL);
    assert(tokens[4].value == 0);
    assert(tokens[5].type == TokenType::INT_LITERAL);
    assert(tokens[5].value == 999);
    
    std::cout << "test_identifiers_and_numbers passed\n";
}

void test_comments() {
    std::string source = "int x; // single line comment\nint y; /* multi\nline\ncomment */ int z;";
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.tokenize();
    
    // Should have: int, x, ;, int, y, ;, int, z, ;
    assert(tokens[0].type == TokenType::INT);
    assert(tokens[1].type == TokenType::IDENT);
    assert(tokens[1].lexeme == "x");
    assert(tokens[2].type == TokenType::SEMICOLON);
    assert(tokens[3].type == TokenType::INT);
    assert(tokens[4].type == TokenType::IDENT);
    assert(tokens[4].lexeme == "y");
    assert(tokens[5].type == TokenType::SEMICOLON);
    assert(tokens[6].type == TokenType::INT);
    assert(tokens[7].type == TokenType::IDENT);
    assert(tokens[7].lexeme == "z");
    
    std::cout << "test_comments passed\n";
}

int main() {
    std::cout << "Running Lexer Tests...\n";
    test_keywords();
    test_operators();
    test_identifiers_and_numbers();
    test_comments();
    std::cout << "All lexer tests passed!\n";
    return 0;
}
