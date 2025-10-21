#ifndef PARSER_H
#define PARSER_H

#include "token.h"
#include "ast.h"
#include <vector>
#include <memory>
#include <stdexcept>

class ParseError : public std::runtime_error {
public:
    ParseError(const std::string& message) : std::runtime_error(message) {}
};

class Parser {
private:
    std::vector<Token> tokens;
    size_t current;
    
    Token currentToken() const;
    Token peek(int offset = 1) const;
    void advance();
    bool match(TokenType type);
    bool match(const std::vector<TokenType>& types);
    void expect(TokenType type, const std::string& message);
    
    std::unique_ptr<Program> parseProgram();
    std::unique_ptr<FunctionDef> parseFunctionDef();
    std::unique_ptr<VarDecl> parseVarDecl();
    std::unique_ptr<VarDecl> parseConstDecl();
    std::unique_ptr<Statement> parseStatement();
    std::unique_ptr<Block> parseBlock();
    std::unique_ptr<IfStmt> parseIfStmt();
    std::unique_ptr<WhileStmt> parseWhileStmt();
    std::unique_ptr<ReturnStmt> parseReturnStmt();
    std::unique_ptr<Expression> parseExpression();
    std::unique_ptr<Expression> parseLogicalOr();
    std::unique_ptr<Expression> parseLogicalAnd();
    std::unique_ptr<Expression> parseEquality();
    std::unique_ptr<Expression> parseRelational();
    std::unique_ptr<Expression> parseAdditive();
    std::unique_ptr<Expression> parseMultiplicative();
    std::unique_ptr<Expression> parseUnary();
    std::unique_ptr<Expression> parsePrimary();
    
public:
    Parser(const std::vector<Token>& tokens);
    std::unique_ptr<Program> parse();
};

#endif // PARSER_H
