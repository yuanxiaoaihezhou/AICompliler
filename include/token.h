#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum class TokenType {
    // Keywords
    CONST, INT, VOID, IF, ELSE, WHILE, BREAK, CONTINUE, RETURN,
    CHAR, TYPEDEF, STRUCT,
    
    // Identifiers and literals
    IDENT, INT_LITERAL, CHAR_LITERAL, STRING_LITERAL,
    
    // Operators
    PLUS, MINUS, MULT, DIV, MOD,
    LT, LE, GT, GE, EQ, NE,
    AND, OR, NOT,
    ASSIGN,
    AMPERSAND,           // & (address-of)
    ARROW,               // ->
    INCREMENT, DECREMENT, // ++ --
    
    // Delimiters
    LPAREN, RPAREN,      // ( )
    LBRACE, RBRACE,      // { }
    LBRACKET, RBRACKET,  // [ ]
    SEMICOLON, COMMA,
    DOT,                 // .
    
    // Special
    END_OF_FILE,
    UNKNOWN
};

class Token {
public:
    TokenType type;
    std::string lexeme;
    int line;
    int column;
    int value;  // For integer literals
    std::string string_value;  // For string literals
    
    Token(TokenType type, const std::string& lexeme, int line, int column);
    Token(TokenType type, const std::string& lexeme, int line, int column, int value);
    Token(TokenType type, const std::string& lexeme, int line, int column, const std::string& str_val);
    
    std::string toString() const;
    static std::string tokenTypeToString(TokenType type);
};

#endif // TOKEN_H
