#include "parser.h"

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens), current(0) {}

Token Parser::currentToken() const {
    if (current < tokens.size()) {
        return tokens[current];
    }
    return tokens.back();  // Return EOF
}

Token Parser::peek(int offset) const {
    size_t pos = current + offset;
    if (pos < tokens.size()) {
        return tokens[pos];
    }
    return tokens.back();  // Return EOF
}

void Parser::advance() {
    if (current < tokens.size() - 1) {
        current++;
    }
}

bool Parser::match(TokenType type) {
    if (currentToken().type == type) {
        advance();
        return true;
    }
    return false;
}

bool Parser::match(const std::vector<TokenType>& types) {
    for (TokenType type : types) {
        if (currentToken().type == type) {
            advance();
            return true;
        }
    }
    return false;
}

void Parser::expect(TokenType type, const std::string& message) {
    if (currentToken().type != type) {
        throw ParseError(message + " at line " + std::to_string(currentToken().line));
    }
    advance();
}

std::unique_ptr<Program> Parser::parse() {
    return parseProgram();
}

std::unique_ptr<Program> Parser::parseProgram() {
    auto program = std::make_unique<Program>();
    
    while (currentToken().type != TokenType::END_OF_FILE) {
        if (currentToken().type == TokenType::CONST) {
            program->declarations.push_back(parseConstDecl());
        } else if (currentToken().type == TokenType::INT || 
                   currentToken().type == TokenType::VOID ||
                   currentToken().type == TokenType::CHAR) {
            // Look ahead to distinguish between function and variable
            // Need to skip potential pointer stars
            int lookahead = 1;
            while (peek(lookahead).type == TokenType::MULT) {
                lookahead++;
            }
            if (peek(lookahead).type == TokenType::IDENT && peek(lookahead + 1).type == TokenType::LPAREN) {
                program->declarations.push_back(parseFunctionDef());
            } else {
                program->declarations.push_back(parseVarDecl());
            }
        } else {
            throw ParseError("Unexpected token at top level: " + currentToken().lexeme);
        }
    }
    
    return program;
}

std::unique_ptr<FunctionDef> Parser::parseFunctionDef() {
    std::string return_type;
    if (match(TokenType::INT)) {
        return_type = "int";
    } else if (match(TokenType::VOID)) {
        return_type = "void";
    } else if (match(TokenType::CHAR)) {
        return_type = "char";
    } else {
        throw ParseError("Expected return type");
    }
    
    // Handle pointer return types
    while (match(TokenType::MULT)) {
        return_type += "*";
    }
    
    std::string name = currentToken().lexeme;
    expect(TokenType::IDENT, "Expected function name");
    expect(TokenType::LPAREN, "Expected '('");
    
    auto func = std::make_unique<FunctionDef>(name, return_type);
    
    // Parse parameters
    if (currentToken().type != TokenType::RPAREN) {
        do {
            std::string param_type;
            if (match(TokenType::INT)) {
                param_type = "int";
            } else if (match(TokenType::CHAR)) {
                param_type = "char";
            } else if (match(TokenType::VOID)) {
                param_type = "void";
            } else {
                throw ParseError("Expected parameter type");
            }
            
            // Handle pointer parameters
            while (match(TokenType::MULT)) {
                param_type += "*";
            }
            
            std::string param_name = currentToken().lexeme;
            expect(TokenType::IDENT, "Expected parameter name");
            
            func->params.push_back({param_type, param_name});
        } while (match(TokenType::COMMA));
    }
    
    expect(TokenType::RPAREN, "Expected ')'");
    func->body = parseBlock();
    
    return func;
}

std::unique_ptr<VarDecl> Parser::parseVarDecl() {
    std::string var_type;
    if (match(TokenType::INT)) {
        var_type = "int";
    } else if (match(TokenType::CHAR)) {
        var_type = "char";
    } else {
        throw ParseError("Expected type (int or char)");
    }
    
    // Handle pointer types
    int pointer_level = 0;
    while (match(TokenType::MULT)) {
        var_type += "*";
        pointer_level++;
    }
    
    std::string name = currentToken().lexeme;
    expect(TokenType::IDENT, "Expected variable name");
    
    bool is_array = false;
    int array_size = 0;
    
    if (match(TokenType::LBRACKET)) {
        is_array = true;
        if (currentToken().type == TokenType::INT_LITERAL) {
            array_size = currentToken().value;
            advance();
        }
        expect(TokenType::RBRACKET, "Expected ']'");
    }
    
    std::unique_ptr<Expression> init_value = nullptr;
    if (match(TokenType::ASSIGN)) {
        init_value = parseExpression();
    }
    
    expect(TokenType::SEMICOLON, "Expected ';'");
    
    return std::make_unique<VarDecl>(name, var_type, false, is_array, array_size, pointer_level, std::move(init_value));
}

std::unique_ptr<VarDecl> Parser::parseConstDecl() {
    expect(TokenType::CONST, "Expected 'const'");
    
    std::string var_type;
    if (match(TokenType::INT)) {
        var_type = "int";
    } else if (match(TokenType::CHAR)) {
        var_type = "char";
    } else {
        throw ParseError("Expected type (int or char)");
    }
    
    // Handle pointer types
    int pointer_level = 0;
    while (match(TokenType::MULT)) {
        var_type += "*";
        pointer_level++;
    }
    
    std::string name = currentToken().lexeme;
    expect(TokenType::IDENT, "Expected constant name");
    
    expect(TokenType::ASSIGN, "Expected '=' for const initialization");
    auto init_value = parseExpression();
    
    expect(TokenType::SEMICOLON, "Expected ';'");
    
    return std::make_unique<VarDecl>(name, var_type, true, false, 0, pointer_level, std::move(init_value));
}

std::unique_ptr<Block> Parser::parseBlock() {
    expect(TokenType::LBRACE, "Expected '{'");
    
    auto block = std::make_unique<Block>();
    
    while (currentToken().type != TokenType::RBRACE && 
           currentToken().type != TokenType::END_OF_FILE) {
        block->statements.push_back(parseStatement());
    }
    
    expect(TokenType::RBRACE, "Expected '}'");
    
    return block;
}

std::unique_ptr<Statement> Parser::parseStatement() {
    if (currentToken().type == TokenType::INT || currentToken().type == TokenType::CHAR) {
        return parseVarDecl();
    } else if (currentToken().type == TokenType::CONST) {
        return parseConstDecl();
    } else if (currentToken().type == TokenType::IF) {
        return parseIfStmt();
    } else if (currentToken().type == TokenType::WHILE) {
        return parseWhileStmt();
    } else if (currentToken().type == TokenType::RETURN) {
        return parseReturnStmt();
    } else if (currentToken().type == TokenType::BREAK) {
        advance();
        expect(TokenType::SEMICOLON, "Expected ';'");
        return std::make_unique<BreakStmt>();
    } else if (currentToken().type == TokenType::CONTINUE) {
        advance();
        expect(TokenType::SEMICOLON, "Expected ';'");
        return std::make_unique<ContinueStmt>();
    } else if (currentToken().type == TokenType::LBRACE) {
        return parseBlock();
    } else {
        // Expression statement
        auto expr = parseExpression();
        expect(TokenType::SEMICOLON, "Expected ';'");
        return std::make_unique<ExprStmt>(std::move(expr));
    }
}

std::unique_ptr<IfStmt> Parser::parseIfStmt() {
    expect(TokenType::IF, "Expected 'if'");
    expect(TokenType::LPAREN, "Expected '('");
    
    auto condition = parseExpression();
    
    expect(TokenType::RPAREN, "Expected ')'");
    
    auto then_stmt = parseStatement();
    std::unique_ptr<Statement> else_stmt = nullptr;
    
    if (match(TokenType::ELSE)) {
        else_stmt = parseStatement();
    }
    
    return std::make_unique<IfStmt>(std::move(condition), std::move(then_stmt), std::move(else_stmt));
}

std::unique_ptr<WhileStmt> Parser::parseWhileStmt() {
    expect(TokenType::WHILE, "Expected 'while'");
    expect(TokenType::LPAREN, "Expected '('");
    
    auto condition = parseExpression();
    
    expect(TokenType::RPAREN, "Expected ')'");
    
    auto body = parseStatement();
    
    return std::make_unique<WhileStmt>(std::move(condition), std::move(body));
}

std::unique_ptr<ReturnStmt> Parser::parseReturnStmt() {
    expect(TokenType::RETURN, "Expected 'return'");
    
    std::unique_ptr<Expression> value = nullptr;
    if (currentToken().type != TokenType::SEMICOLON) {
        value = parseExpression();
    }
    
    expect(TokenType::SEMICOLON, "Expected ';'");
    
    return std::make_unique<ReturnStmt>(std::move(value));
}

std::unique_ptr<Expression> Parser::parseExpression() {
    auto expr = parseLogicalOr();
    
    // Handle assignment
    if (currentToken().type == TokenType::ASSIGN) {
        advance();
        auto right = parseExpression();
        // Convert left to identifier if it is one
        if (auto* ident = dynamic_cast<IdentExpr*>(expr.get())) {
            expr = std::make_unique<BinaryExpr>("=", std::move(expr), std::move(right));
        }
    }
    
    return expr;
}

std::unique_ptr<Expression> Parser::parseLogicalOr() {
    auto left = parseLogicalAnd();
    
    while (match(TokenType::OR)) {
        left = std::make_unique<BinaryExpr>("||", std::move(left), parseLogicalAnd());
    }
    
    return left;
}

std::unique_ptr<Expression> Parser::parseLogicalAnd() {
    auto left = parseEquality();
    
    while (match(TokenType::AND)) {
        left = std::make_unique<BinaryExpr>("&&", std::move(left), parseEquality());
    }
    
    return left;
}

std::unique_ptr<Expression> Parser::parseEquality() {
    auto left = parseRelational();
    
    while (true) {
        if (match(TokenType::EQ)) {
            left = std::make_unique<BinaryExpr>("==", std::move(left), parseRelational());
        } else if (match(TokenType::NE)) {
            left = std::make_unique<BinaryExpr>("!=", std::move(left), parseRelational());
        } else {
            break;
        }
    }
    
    return left;
}

std::unique_ptr<Expression> Parser::parseRelational() {
    auto left = parseAdditive();
    
    while (true) {
        if (match(TokenType::LT)) {
            left = std::make_unique<BinaryExpr>("<", std::move(left), parseAdditive());
        } else if (match(TokenType::LE)) {
            left = std::make_unique<BinaryExpr>("<=", std::move(left), parseAdditive());
        } else if (match(TokenType::GT)) {
            left = std::make_unique<BinaryExpr>(">", std::move(left), parseAdditive());
        } else if (match(TokenType::GE)) {
            left = std::make_unique<BinaryExpr>(">=", std::move(left), parseAdditive());
        } else {
            break;
        }
    }
    
    return left;
}

std::unique_ptr<Expression> Parser::parseAdditive() {
    auto left = parseMultiplicative();
    
    while (true) {
        if (match(TokenType::PLUS)) {
            left = std::make_unique<BinaryExpr>("+", std::move(left), parseMultiplicative());
        } else if (match(TokenType::MINUS)) {
            left = std::make_unique<BinaryExpr>("-", std::move(left), parseMultiplicative());
        } else {
            break;
        }
    }
    
    return left;
}

std::unique_ptr<Expression> Parser::parseMultiplicative() {
    auto left = parseUnary();
    
    while (true) {
        if (match(TokenType::MULT)) {
            left = std::make_unique<BinaryExpr>("*", std::move(left), parseUnary());
        } else if (match(TokenType::DIV)) {
            left = std::make_unique<BinaryExpr>("/", std::move(left), parseUnary());
        } else if (match(TokenType::MOD)) {
            left = std::make_unique<BinaryExpr>("%", std::move(left), parseUnary());
        } else {
            break;
        }
    }
    
    return left;
}

std::unique_ptr<Expression> Parser::parseUnary() {
    if (match(TokenType::PLUS)) {
        return std::make_unique<UnaryExpr>("+", parseUnary());
    } else if (match(TokenType::MINUS)) {
        return std::make_unique<UnaryExpr>("-", parseUnary());
    } else if (match(TokenType::NOT)) {
        return std::make_unique<UnaryExpr>("!", parseUnary());
    } else if (match(TokenType::AMPERSAND)) {
        return std::make_unique<UnaryExpr>("&", parseUnary());
    } else if (match(TokenType::MULT)) {
        // Dereference operator
        return std::make_unique<UnaryExpr>("*", parseUnary());
    } else if (match(TokenType::INCREMENT)) {
        return std::make_unique<UnaryExpr>("++", parseUnary());
    } else if (match(TokenType::DECREMENT)) {
        return std::make_unique<UnaryExpr>("--", parseUnary());
    }
    
    return parsePrimary();
}

std::unique_ptr<Expression> Parser::parsePrimary() {
    // Integer literal
    if (currentToken().type == TokenType::INT_LITERAL) {
        int value = currentToken().value;
        advance();
        return std::make_unique<IntLiteralExpr>(value);
    }
    
    // Character literal
    if (currentToken().type == TokenType::CHAR_LITERAL) {
        int value = currentToken().value;
        advance();
        return std::make_unique<CharLiteralExpr>(value);
    }
    
    // String literal
    if (currentToken().type == TokenType::STRING_LITERAL) {
        std::string value = currentToken().string_value;
        advance();
        return std::make_unique<StringLiteralExpr>(value);
    }
    
    // Identifier (variable or function call)
    if (currentToken().type == TokenType::IDENT) {
        std::string name = currentToken().lexeme;
        advance();
        
        // Function call
        if (match(TokenType::LPAREN)) {
            auto call = std::make_unique<CallExpr>(name);
            
            if (currentToken().type != TokenType::RPAREN) {
                do {
                    call->args.push_back(parseExpression());
                } while (match(TokenType::COMMA));
            }
            
            expect(TokenType::RPAREN, "Expected ')'");
            return call;
        }
        // Array access
        else if (match(TokenType::LBRACKET)) {
            auto index = parseExpression();
            expect(TokenType::RBRACKET, "Expected ']'");
            return std::make_unique<ArrayAccess>(name, std::move(index));
        }
        // Simple variable
        else {
            return std::make_unique<IdentExpr>(name);
        }
    }
    
    // Parenthesized expression
    if (match(TokenType::LPAREN)) {
        auto expr = parseExpression();
        expect(TokenType::RPAREN, "Expected ')'");
        return expr;
    }
    
    throw ParseError("Unexpected token in expression: " + currentToken().lexeme);
}
