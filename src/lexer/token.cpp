#include "token.h"

Token::Token(TokenType type, const std::string& lexeme, int line, int column)
    : type(type), lexeme(lexeme), line(line), column(column), value(0) {}

Token::Token(TokenType type, const std::string& lexeme, int line, int column, int value)
    : type(type), lexeme(lexeme), line(line), column(column), value(value) {}

std::string Token::toString() const {
    std::string result = "Token(" + tokenTypeToString(type) + ", '" + lexeme + "'";
    if (type == TokenType::INT_LITERAL) {
        result += ", value=" + std::to_string(value);
    }
    result += ", line=" + std::to_string(line) + ", col=" + std::to_string(column) + ")";
    return result;
}

std::string Token::tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::CONST: return "CONST";
        case TokenType::INT: return "INT";
        case TokenType::VOID: return "VOID";
        case TokenType::IF: return "IF";
        case TokenType::ELSE: return "ELSE";
        case TokenType::WHILE: return "WHILE";
        case TokenType::BREAK: return "BREAK";
        case TokenType::CONTINUE: return "CONTINUE";
        case TokenType::RETURN: return "RETURN";
        case TokenType::IDENT: return "IDENT";
        case TokenType::INT_LITERAL: return "INT_LITERAL";
        case TokenType::PLUS: return "PLUS";
        case TokenType::MINUS: return "MINUS";
        case TokenType::MULT: return "MULT";
        case TokenType::DIV: return "DIV";
        case TokenType::MOD: return "MOD";
        case TokenType::LT: return "LT";
        case TokenType::LE: return "LE";
        case TokenType::GT: return "GT";
        case TokenType::GE: return "GE";
        case TokenType::EQ: return "EQ";
        case TokenType::NE: return "NE";
        case TokenType::AND: return "AND";
        case TokenType::OR: return "OR";
        case TokenType::NOT: return "NOT";
        case TokenType::ASSIGN: return "ASSIGN";
        case TokenType::LPAREN: return "LPAREN";
        case TokenType::RPAREN: return "RPAREN";
        case TokenType::LBRACE: return "LBRACE";
        case TokenType::RBRACE: return "RBRACE";
        case TokenType::LBRACKET: return "LBRACKET";
        case TokenType::RBRACKET: return "RBRACKET";
        case TokenType::SEMICOLON: return "SEMICOLON";
        case TokenType::COMMA: return "COMMA";
        case TokenType::END_OF_FILE: return "EOF";
        case TokenType::UNKNOWN: return "UNKNOWN";
        default: return "UNKNOWN";
    }
}
