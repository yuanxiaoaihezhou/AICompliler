#include "lexer.h"
#include <cctype>
#include <stdexcept>

Lexer::Lexer(const std::string& source) 
    : source(source), position(0), line(1), column(1) {
    current_char = source.empty() ? '\0' : source[0];
    
    // Initialize keywords
    keywords["const"] = TokenType::CONST;
    keywords["int"] = TokenType::INT;
    keywords["void"] = TokenType::VOID;
    keywords["char"] = TokenType::CHAR;
    keywords["typedef"] = TokenType::TYPEDEF;
    keywords["if"] = TokenType::IF;
    keywords["else"] = TokenType::ELSE;
    keywords["while"] = TokenType::WHILE;
    keywords["break"] = TokenType::BREAK;
    keywords["continue"] = TokenType::CONTINUE;
    keywords["return"] = TokenType::RETURN;
}

void Lexer::advance() {
    if (position < source.length() - 1) {
        position++;
        column++;
        current_char = source[position];
        if (current_char == '\n') {
            line++;
            column = 1;
        }
    } else {
        position++;
        current_char = '\0';
    }
}

char Lexer::peek(int offset) const {
    size_t peek_pos = position + offset;
    if (peek_pos < source.length()) {
        return source[peek_pos];
    }
    return '\0';
}

void Lexer::skipWhitespace() {
    while (current_char != '\0' && std::isspace(current_char)) {
        advance();
    }
}

void Lexer::skipComment() {
    if (current_char == '/' && peek() == '/') {
        // Single line comment
        while (current_char != '\0' && current_char != '\n') {
            advance();
        }
    } else if (current_char == '/' && peek() == '*') {
        // Multi-line comment
        advance(); // skip '/'
        advance(); // skip '*'
        while (current_char != '\0') {
            if (current_char == '*' && peek() == '/') {
                advance(); // skip '*'
                advance(); // skip '/'
                break;
            }
            advance();
        }
    }
}

Token Lexer::number() {
    int start_line = line;
    int start_column = column;
    std::string num_str;
    
    while (current_char != '\0' && std::isdigit(current_char)) {
        num_str += current_char;
        advance();
    }
    
    int value = std::stoi(num_str);
    return Token(TokenType::INT_LITERAL, num_str, start_line, start_column, value);
}

Token Lexer::identifier() {
    int start_line = line;
    int start_column = column;
    std::string id_str;
    
    while (current_char != '\0' && (std::isalnum(current_char) || current_char == '_')) {
        id_str += current_char;
        advance();
    }
    
    // Check if it's a keyword
    auto it = keywords.find(id_str);
    if (it != keywords.end()) {
        return Token(it->second, id_str, start_line, start_column);
    }
    
    return Token(TokenType::IDENT, id_str, start_line, start_column);
}

Token Lexer::charLiteral() {
    int start_line = line;
    int start_column = column;
    advance(); // skip opening '
    
    if (current_char == '\0') {
        throw std::runtime_error("Unterminated character literal");
    }
    
    int value = 0;
    std::string lexeme = "'";
    
    if (current_char == '\\') {
        advance();
        lexeme += '\\';
        // Handle escape sequences
        switch (current_char) {
            case 'n': value = '\n'; lexeme += 'n'; break;
            case 't': value = '\t'; lexeme += 't'; break;
            case 'r': value = '\r'; lexeme += 'r'; break;
            case '0': value = '\0'; lexeme += '0'; break;
            case '\\': value = '\\'; lexeme += '\\'; break;
            case '\'': value = '\''; lexeme += '\''; break;
            default: value = current_char; lexeme += current_char; break;
        }
        advance();
    } else {
        value = current_char;
        lexeme += current_char;
        advance();
    }
    
    if (current_char != '\'') {
        throw std::runtime_error("Expected closing ' in character literal");
    }
    lexeme += '\'';
    advance(); // skip closing '
    
    return Token(TokenType::CHAR_LITERAL, lexeme, start_line, start_column, value);
}

Token Lexer::stringLiteral() {
    int start_line = line;
    int start_column = column;
    advance(); // skip opening "
    
    std::string value;
    std::string lexeme = "\"";
    
    while (current_char != '\0' && current_char != '"') {
        if (current_char == '\\') {
            advance();
            lexeme += '\\';
            // Handle escape sequences
            switch (current_char) {
                case 'n': value += '\n'; lexeme += 'n'; break;
                case 't': value += '\t'; lexeme += 't'; break;
                case 'r': value += '\r'; lexeme += 'r'; break;
                case '0': value += '\0'; lexeme += '0'; break;
                case '\\': value += '\\'; lexeme += '\\'; break;
                case '"': value += '"'; lexeme += '"'; break;
                default: value += current_char; lexeme += current_char; break;
            }
            advance();
        } else {
            value += current_char;
            lexeme += current_char;
            advance();
        }
    }
    
    if (current_char != '"') {
        throw std::runtime_error("Unterminated string literal");
    }
    lexeme += '"';
    advance(); // skip closing "
    
    return Token(TokenType::STRING_LITERAL, lexeme, start_line, start_column, value);
}

Token Lexer::getNextToken() {
    while (current_char != '\0') {
        if (std::isspace(current_char)) {
            skipWhitespace();
            continue;
        }
        
        if (current_char == '/' && (peek() == '/' || peek() == '*')) {
            skipComment();
            continue;
        }
        
        int start_line = line;
        int start_column = column;
        
        if (std::isdigit(current_char)) {
            return number();
        }
        
        if (std::isalpha(current_char) || current_char == '_') {
            return identifier();
        }
        
        // Character literals
        if (current_char == '\'') {
            return charLiteral();
        }
        
        // String literals
        if (current_char == '"') {
            return stringLiteral();
        }
        
        // Single character tokens
        char ch = current_char;
        advance();
        
        switch (ch) {
            case '+':
                if (current_char == '+') {
                    advance();
                    return Token(TokenType::INCREMENT, "++", start_line, start_column);
                }
                return Token(TokenType::PLUS, "+", start_line, start_column);
            case '-':
                if (current_char == '-') {
                    advance();
                    return Token(TokenType::DECREMENT, "--", start_line, start_column);
                } else if (current_char == '>') {
                    advance();
                    return Token(TokenType::ARROW, "->", start_line, start_column);
                }
                return Token(TokenType::MINUS, "-", start_line, start_column);
            case '*': return Token(TokenType::MULT, "*", start_line, start_column);
            case '/': return Token(TokenType::DIV, "/", start_line, start_column);
            case '%': return Token(TokenType::MOD, "%", start_line, start_column);
            case '(': return Token(TokenType::LPAREN, "(", start_line, start_column);
            case ')': return Token(TokenType::RPAREN, ")", start_line, start_column);
            case '{': return Token(TokenType::LBRACE, "{", start_line, start_column);
            case '}': return Token(TokenType::RBRACE, "}", start_line, start_column);
            case '[': return Token(TokenType::LBRACKET, "[", start_line, start_column);
            case ']': return Token(TokenType::RBRACKET, "]", start_line, start_column);
            case ';': return Token(TokenType::SEMICOLON, ";", start_line, start_column);
            case ',': return Token(TokenType::COMMA, ",", start_line, start_column);
            case '.': return Token(TokenType::DOT, ".", start_line, start_column);
            case '!':
                if (current_char == '=') {
                    advance();
                    return Token(TokenType::NE, "!=", start_line, start_column);
                }
                return Token(TokenType::NOT, "!", start_line, start_column);
            case '=':
                if (current_char == '=') {
                    advance();
                    return Token(TokenType::EQ, "==", start_line, start_column);
                }
                return Token(TokenType::ASSIGN, "=", start_line, start_column);
            case '<':
                if (current_char == '=') {
                    advance();
                    return Token(TokenType::LE, "<=", start_line, start_column);
                }
                return Token(TokenType::LT, "<", start_line, start_column);
            case '>':
                if (current_char == '=') {
                    advance();
                    return Token(TokenType::GE, ">=", start_line, start_column);
                }
                return Token(TokenType::GT, ">", start_line, start_column);
            case '&':
                if (current_char == '&') {
                    advance();
                    return Token(TokenType::AND, "&&", start_line, start_column);
                }
                return Token(TokenType::AMPERSAND, "&", start_line, start_column);
            case '|':
                if (current_char == '|') {
                    advance();
                    return Token(TokenType::OR, "||", start_line, start_column);
                }
                break;
        }
        
        return Token(TokenType::UNKNOWN, std::string(1, ch), start_line, start_column);
    }
    
    return Token(TokenType::END_OF_FILE, "", line, column);
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    Token token = getNextToken();
    while (token.type != TokenType::END_OF_FILE) {
        tokens.push_back(token);
        token = getNextToken();
    }
    tokens.push_back(token);  // Add EOF token
    return tokens;
}
