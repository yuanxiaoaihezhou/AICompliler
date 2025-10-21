#ifndef AST_H
#define AST_H

#include <string>
#include <vector>
#include <memory>

// Forward declarations
class ASTVisitor;

enum class ASTNodeType {
    PROGRAM,
    FUNCTION_DEF,
    VAR_DECL,
    CONST_DECL,
    STRUCT_DECL,
    TYPEDEF_DECL,
    BLOCK,
    IF_STMT,
    WHILE_STMT,
    RETURN_STMT,
    BREAK_STMT,
    CONTINUE_STMT,
    EXPR_STMT,
    BINARY_EXPR,
    UNARY_EXPR,
    CALL_EXPR,
    IDENT_EXPR,
    INT_LITERAL_EXPR,
    CHAR_LITERAL_EXPR,
    STRING_LITERAL_EXPR,
    ARRAY_ACCESS,
    MEMBER_ACCESS
};

class ASTNode {
public:
    ASTNodeType type;
    virtual ~ASTNode() = default;
    virtual void accept(ASTVisitor* visitor) = 0;
};

class Expression : public ASTNode {};

class Statement : public ASTNode {};

// Expressions
class IntLiteralExpr : public Expression {
public:
    int value;
    IntLiteralExpr(int value);
    void accept(ASTVisitor* visitor) override;
};

class CharLiteralExpr : public Expression {
public:
    int value;
    CharLiteralExpr(int value);
    void accept(ASTVisitor* visitor) override;
};

class StringLiteralExpr : public Expression {
public:
    std::string value;
    StringLiteralExpr(const std::string& value);
    void accept(ASTVisitor* visitor) override;
};

class IdentExpr : public Expression {
public:
    std::string name;
    IdentExpr(const std::string& name);
    void accept(ASTVisitor* visitor) override;
};

class BinaryExpr : public Expression {
public:
    std::string op;
    std::unique_ptr<Expression> left;
    std::unique_ptr<Expression> right;
    BinaryExpr(const std::string& op, std::unique_ptr<Expression> left, 
               std::unique_ptr<Expression> right);
    void accept(ASTVisitor* visitor) override;
};

class UnaryExpr : public Expression {
public:
    std::string op;
    std::unique_ptr<Expression> operand;
    UnaryExpr(const std::string& op, std::unique_ptr<Expression> operand);
    void accept(ASTVisitor* visitor) override;
};

class CallExpr : public Expression {
public:
    std::string func_name;
    std::vector<std::unique_ptr<Expression>> args;
    CallExpr(const std::string& func_name);
    void accept(ASTVisitor* visitor) override;
};

class ArrayAccess : public Expression {
public:
    std::string array_name;
    std::unique_ptr<Expression> index;
    ArrayAccess(const std::string& array_name, std::unique_ptr<Expression> index);
    void accept(ASTVisitor* visitor) override;
};

// Statements
class ExprStmt : public Statement {
public:
    std::unique_ptr<Expression> expr;
    ExprStmt(std::unique_ptr<Expression> expr);
    void accept(ASTVisitor* visitor) override;
};

class Block : public Statement {
public:
    std::vector<std::unique_ptr<Statement>> statements;
    void accept(ASTVisitor* visitor) override;
};

class IfStmt : public Statement {
public:
    std::unique_ptr<Expression> condition;
    std::unique_ptr<Statement> then_stmt;
    std::unique_ptr<Statement> else_stmt;
    IfStmt(std::unique_ptr<Expression> condition, std::unique_ptr<Statement> then_stmt,
           std::unique_ptr<Statement> else_stmt = nullptr);
    void accept(ASTVisitor* visitor) override;
};

class WhileStmt : public Statement {
public:
    std::unique_ptr<Expression> condition;
    std::unique_ptr<Statement> body;
    WhileStmt(std::unique_ptr<Expression> condition, std::unique_ptr<Statement> body);
    void accept(ASTVisitor* visitor) override;
};

class ReturnStmt : public Statement {
public:
    std::unique_ptr<Expression> value;
    ReturnStmt(std::unique_ptr<Expression> value = nullptr);
    void accept(ASTVisitor* visitor) override;
};

class BreakStmt : public Statement {
public:
    void accept(ASTVisitor* visitor) override;
};

class ContinueStmt : public Statement {
public:
    void accept(ASTVisitor* visitor) override;
};

class VarDecl : public Statement {
public:
    std::string name;
    std::string var_type;  // "int", "char", "void", or "int*", "char*", etc.
    bool is_const;
    bool is_array;
    int array_size;
    int pointer_level;  // 0 for non-pointer, 1 for *, 2 for **, etc.
    std::unique_ptr<Expression> init_value;
    VarDecl(const std::string& name, const std::string& var_type = "int", bool is_const = false, 
            bool is_array = false, int array_size = 0, int pointer_level = 0,
            std::unique_ptr<Expression> init_value = nullptr);
    void accept(ASTVisitor* visitor) override;
};

// Struct declaration
class StructDecl : public ASTNode {
public:
    std::string name;
    std::vector<std::unique_ptr<VarDecl>> members;
    StructDecl(const std::string& name);
    void accept(ASTVisitor* visitor) override;
};

// Typedef declaration
class TypedefDecl : public ASTNode {
public:
    std::string original_type;
    std::string new_name;
    TypedefDecl(const std::string& original_type, const std::string& new_name);
    void accept(ASTVisitor* visitor) override;
};

// Member access expression (struct.field or ptr->field)
class MemberAccess : public Expression {
public:
    std::unique_ptr<Expression> object;
    std::string member_name;
    bool is_arrow;  // true for ->, false for .
    MemberAccess(std::unique_ptr<Expression> object, const std::string& member_name, bool is_arrow);
    void accept(ASTVisitor* visitor) override;
};

// Function definition
class FunctionDef : public ASTNode {
public:
    std::string name;
    std::string return_type;
    std::vector<std::pair<std::string, std::string>> params;  // (type, name)
    std::unique_ptr<Block> body;
    FunctionDef(const std::string& name, const std::string& return_type);
    void accept(ASTVisitor* visitor) override;
};

// Program (root node)
class Program : public ASTNode {
public:
    std::vector<std::unique_ptr<ASTNode>> declarations;
    void accept(ASTVisitor* visitor) override;
};

// Visitor pattern for AST traversal
class ASTVisitor {
public:
    virtual ~ASTVisitor() = default;
    virtual void visit(Program* node) = 0;
    virtual void visit(FunctionDef* node) = 0;
    virtual void visit(VarDecl* node) = 0;
    virtual void visit(StructDecl* node) = 0;
    virtual void visit(TypedefDecl* node) = 0;
    virtual void visit(Block* node) = 0;
    virtual void visit(IfStmt* node) = 0;
    virtual void visit(WhileStmt* node) = 0;
    virtual void visit(ReturnStmt* node) = 0;
    virtual void visit(BreakStmt* node) = 0;
    virtual void visit(ContinueStmt* node) = 0;
    virtual void visit(ExprStmt* node) = 0;
    virtual void visit(BinaryExpr* node) = 0;
    virtual void visit(UnaryExpr* node) = 0;
    virtual void visit(CallExpr* node) = 0;
    virtual void visit(IdentExpr* node) = 0;
    virtual void visit(IntLiteralExpr* node) = 0;
    virtual void visit(CharLiteralExpr* node) = 0;
    virtual void visit(StringLiteralExpr* node) = 0;
    virtual void visit(ArrayAccess* node) = 0;
    virtual void visit(MemberAccess* node) = 0;
};

#endif // AST_H
