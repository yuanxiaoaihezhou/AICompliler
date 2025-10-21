#ifndef IR_GENERATOR_H
#define IR_GENERATOR_H

#include "ast.h"
#include "ir.h"
#include <map>
#include <string>

class IRGenerator : public ASTVisitor {
private:
    IRModule module;
    IRFunction* current_function;
    std::map<std::string, std::string> symbol_table;
    std::string last_result;
    std::string break_label;
    std::string continue_label;
    
public:
    IRGenerator();
    IRModule generate(Program* program);
    
    void visit(Program* node) override;
    void visit(FunctionDef* node) override;
    void visit(VarDecl* node) override;
    void visit(Block* node) override;
    void visit(IfStmt* node) override;
    void visit(WhileStmt* node) override;
    void visit(ReturnStmt* node) override;
    void visit(BreakStmt* node) override;
    void visit(ContinueStmt* node) override;
    void visit(ExprStmt* node) override;
    void visit(BinaryExpr* node) override;
    void visit(UnaryExpr* node) override;
    void visit(CallExpr* node) override;
    void visit(IdentExpr* node) override;
    void visit(IntLiteralExpr* node) override;
    void visit(ArrayAccess* node) override;
};

#endif // IR_GENERATOR_H
