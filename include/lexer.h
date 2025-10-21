#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include <string>
#include <vector>
#include <unordered_map>

class Lexer {
private:
    std::string source;
    size_t position;
    int line;
    int column;
    char current_char;
    
    std::unordered_map<std::string, TokenType> keywords;
    
    void advance();
    char peek(int offset = 1) const;
    void skipWhitespace();
    void skipComment();
    Token number();
    Token identifier();
    Token charLiteral();
    Token stringLiteral();
    
public:
    Lexer(const std::string& source);
    Token getNextToken();
    std::vector<Token> tokenize();
};

#endif // LEXER_H
