#include "ast.h"

// IntLiteralExpr
IntLiteralExpr::IntLiteralExpr(int value) : value(value) {
    type = ASTNodeType::INT_LITERAL_EXPR;
}

void IntLiteralExpr::accept(ASTVisitor* visitor) {
    visitor->visit(this);
}

// CharLiteralExpr
CharLiteralExpr::CharLiteralExpr(int value) : value(value) {
    type = ASTNodeType::CHAR_LITERAL_EXPR;
}

void CharLiteralExpr::accept(ASTVisitor* visitor) {
    visitor->visit(this);
}

// StringLiteralExpr
StringLiteralExpr::StringLiteralExpr(const std::string& value) : value(value) {
    type = ASTNodeType::STRING_LITERAL_EXPR;
}

void StringLiteralExpr::accept(ASTVisitor* visitor) {
    visitor->visit(this);
}

// IdentExpr
IdentExpr::IdentExpr(const std::string& name) : name(name) {
    type = ASTNodeType::IDENT_EXPR;
}

void IdentExpr::accept(ASTVisitor* visitor) {
    visitor->visit(this);
}

// BinaryExpr
BinaryExpr::BinaryExpr(const std::string& op, std::unique_ptr<Expression> left,
                       std::unique_ptr<Expression> right)
    : op(op), left(std::move(left)), right(std::move(right)) {
    type = ASTNodeType::BINARY_EXPR;
}

void BinaryExpr::accept(ASTVisitor* visitor) {
    visitor->visit(this);
}

// UnaryExpr
UnaryExpr::UnaryExpr(const std::string& op, std::unique_ptr<Expression> operand)
    : op(op), operand(std::move(operand)) {
    type = ASTNodeType::UNARY_EXPR;
}

void UnaryExpr::accept(ASTVisitor* visitor) {
    visitor->visit(this);
}

// CallExpr
CallExpr::CallExpr(const std::string& func_name) : func_name(func_name) {
    type = ASTNodeType::CALL_EXPR;
}

void CallExpr::accept(ASTVisitor* visitor) {
    visitor->visit(this);
}

// ArrayAccess
ArrayAccess::ArrayAccess(const std::string& array_name, std::unique_ptr<Expression> index)
    : array_name(array_name), index(std::move(index)) {
    type = ASTNodeType::ARRAY_ACCESS;
}

void ArrayAccess::accept(ASTVisitor* visitor) {
    visitor->visit(this);
}

// ExprStmt
ExprStmt::ExprStmt(std::unique_ptr<Expression> expr) : expr(std::move(expr)) {
    type = ASTNodeType::EXPR_STMT;
}

void ExprStmt::accept(ASTVisitor* visitor) {
    visitor->visit(this);
}

// Block
void Block::accept(ASTVisitor* visitor) {
    type = ASTNodeType::BLOCK;
    visitor->visit(this);
}

// IfStmt
IfStmt::IfStmt(std::unique_ptr<Expression> condition, std::unique_ptr<Statement> then_stmt,
               std::unique_ptr<Statement> else_stmt)
    : condition(std::move(condition)), then_stmt(std::move(then_stmt)), 
      else_stmt(std::move(else_stmt)) {
    type = ASTNodeType::IF_STMT;
}

void IfStmt::accept(ASTVisitor* visitor) {
    visitor->visit(this);
}

// WhileStmt
WhileStmt::WhileStmt(std::unique_ptr<Expression> condition, std::unique_ptr<Statement> body)
    : condition(std::move(condition)), body(std::move(body)) {
    type = ASTNodeType::WHILE_STMT;
}

void WhileStmt::accept(ASTVisitor* visitor) {
    visitor->visit(this);
}

// ReturnStmt
ReturnStmt::ReturnStmt(std::unique_ptr<Expression> value) : value(std::move(value)) {
    type = ASTNodeType::RETURN_STMT;
}

void ReturnStmt::accept(ASTVisitor* visitor) {
    visitor->visit(this);
}

// BreakStmt
void BreakStmt::accept(ASTVisitor* visitor) {
    type = ASTNodeType::BREAK_STMT;
    visitor->visit(this);
}

// ContinueStmt
void ContinueStmt::accept(ASTVisitor* visitor) {
    type = ASTNodeType::CONTINUE_STMT;
    visitor->visit(this);
}

// VarDecl
VarDecl::VarDecl(const std::string& name, const std::string& var_type, bool is_const, 
                 bool is_array, int array_size, int pointer_level,
                 std::unique_ptr<Expression> init_value)
    : name(name), var_type(var_type), is_const(is_const), is_array(is_array), 
      array_size(array_size), pointer_level(pointer_level), init_value(std::move(init_value)) {
    type = ASTNodeType::VAR_DECL;
}

void VarDecl::accept(ASTVisitor* visitor) {
    visitor->visit(this);
}

// StructDecl
StructDecl::StructDecl(const std::string& name) : name(name) {
    type = ASTNodeType::STRUCT_DECL;
}

void StructDecl::accept(ASTVisitor* visitor) {
    visitor->visit(this);
}

// TypedefDecl
TypedefDecl::TypedefDecl(const std::string& original_type, const std::string& new_name)
    : original_type(original_type), new_name(new_name) {
    type = ASTNodeType::TYPEDEF_DECL;
}

void TypedefDecl::accept(ASTVisitor* visitor) {
    visitor->visit(this);
}

// MemberAccess
MemberAccess::MemberAccess(std::unique_ptr<Expression> object, const std::string& member_name, bool is_arrow)
    : object(std::move(object)), member_name(member_name), is_arrow(is_arrow) {
    type = ASTNodeType::MEMBER_ACCESS;
}

void MemberAccess::accept(ASTVisitor* visitor) {
    visitor->visit(this);
}

// FunctionDef
FunctionDef::FunctionDef(const std::string& name, const std::string& return_type)
    : name(name), return_type(return_type) {
    type = ASTNodeType::FUNCTION_DEF;
}

void FunctionDef::accept(ASTVisitor* visitor) {
    visitor->visit(this);
}

// Program
void Program::accept(ASTVisitor* visitor) {
    type = ASTNodeType::PROGRAM;
    visitor->visit(this);
}
