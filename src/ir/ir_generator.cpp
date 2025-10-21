#include "ir_generator.h"
#include <stdexcept>

IRGenerator::IRGenerator() : current_function(nullptr) {}

IRModule IRGenerator::generate(Program* program) {
    program->accept(this);
    return module;
}

void IRGenerator::visit(Program* node) {
    for (auto& decl : node->declarations) {
        decl->accept(this);
    }
}

void IRGenerator::visit(FunctionDef* node) {
    IRFunction func(node->name, node->return_type);
    current_function = &func;
    
    // Add parameters
    for (const auto& param : node->params) {
        func.params.push_back(param.second);
        symbol_table[param.second] = param.second;
    }
    
    // Generate function body
    if (node->body) {
        node->body->accept(this);
    }
    
    module.addFunction(func);
    current_function = nullptr;
    symbol_table.clear();
}

void IRGenerator::visit(VarDecl* node) {
    if (!current_function) {
        // Global variable
        module.global_vars[node->name] = 0;
        return;
    }
    
    // Local variable
    std::string var_name = node->name;
    
    if (node->is_array) {
        std::string size_str = std::to_string(node->array_size);
        current_function->addInstruction(IRInstruction(IROpcode::ALLOC, var_name, size_str));
    } else {
        // Allocate space for variable
        current_function->addInstruction(IRInstruction(IROpcode::ALLOC, var_name, "4"));
    }
    
    symbol_table[var_name] = var_name;
    
    // Initialize if there's an init value
    if (node->init_value) {
        node->init_value->accept(this);
        current_function->addInstruction(IRInstruction(IROpcode::STORE, var_name, last_result));
    }
}

void IRGenerator::visit(Block* node) {
    for (auto& stmt : node->statements) {
        stmt->accept(this);
    }
}

void IRGenerator::visit(IfStmt* node) {
    std::string then_label = current_function->newLabel();
    std::string else_label = current_function->newLabel();
    std::string end_label = current_function->newLabel();
    
    // Evaluate condition
    node->condition->accept(this);
    std::string cond_result = last_result;
    
    if (node->else_stmt) {
        current_function->addInstruction(IRInstruction(IROpcode::BRANCH, then_label, cond_result, else_label));
    } else {
        current_function->addInstruction(IRInstruction(IROpcode::BRANCH, then_label, cond_result, end_label));
    }
    
    // Then branch
    current_function->addInstruction(IRInstruction(IROpcode::LABEL, then_label));
    node->then_stmt->accept(this);
    current_function->addInstruction(IRInstruction(IROpcode::JUMP, end_label));
    
    // Else branch
    if (node->else_stmt) {
        current_function->addInstruction(IRInstruction(IROpcode::LABEL, else_label));
        node->else_stmt->accept(this);
        current_function->addInstruction(IRInstruction(IROpcode::JUMP, end_label));
    }
    
    // End label
    current_function->addInstruction(IRInstruction(IROpcode::LABEL, end_label));
}

void IRGenerator::visit(WhileStmt* node) {
    std::string loop_label = current_function->newLabel();
    std::string body_label = current_function->newLabel();
    std::string end_label = current_function->newLabel();
    
    std::string old_break = break_label;
    std::string old_continue = continue_label;
    break_label = end_label;
    continue_label = loop_label;
    
    // Loop condition
    current_function->addInstruction(IRInstruction(IROpcode::LABEL, loop_label));
    node->condition->accept(this);
    std::string cond_result = last_result;
    current_function->addInstruction(IRInstruction(IROpcode::BRANCH, body_label, cond_result, end_label));
    
    // Loop body
    current_function->addInstruction(IRInstruction(IROpcode::LABEL, body_label));
    node->body->accept(this);
    current_function->addInstruction(IRInstruction(IROpcode::JUMP, loop_label));
    
    // End label
    current_function->addInstruction(IRInstruction(IROpcode::LABEL, end_label));
    
    break_label = old_break;
    continue_label = old_continue;
}

void IRGenerator::visit(ReturnStmt* node) {
    if (node->value) {
        node->value->accept(this);
        current_function->addInstruction(IRInstruction(IROpcode::RETURN, last_result));
    } else {
        current_function->addInstruction(IRInstruction(IROpcode::RETURN));
    }
}

void IRGenerator::visit(BreakStmt* node) {
    if (!break_label.empty()) {
        current_function->addInstruction(IRInstruction(IROpcode::JUMP, break_label));
    }
}

void IRGenerator::visit(ContinueStmt* node) {
    if (!continue_label.empty()) {
        current_function->addInstruction(IRInstruction(IROpcode::JUMP, continue_label));
    }
}

void IRGenerator::visit(ExprStmt* node) {
    if (node->expr) {
        node->expr->accept(this);
    }
}

void IRGenerator::visit(BinaryExpr* node) {
    // Handle assignment specially
    if (node->op == "=") {
        // Get the variable name from the left side
        IdentExpr* ident = dynamic_cast<IdentExpr*>(node->left.get());
        if (!ident) {
            throw std::runtime_error("Left side of assignment must be an identifier");
        }
        
        // Evaluate the right side
        node->right->accept(this);
        std::string right_result = last_result;
        
        // Store the value
        current_function->addInstruction(IRInstruction(IROpcode::STORE, ident->name, right_result));
        last_result = right_result;
        return;
    }
    
    node->left->accept(this);
    std::string left_result = last_result;
    
    node->right->accept(this);
    std::string right_result = last_result;
    
    std::string temp = current_function->newTemp();
    
    IROpcode opcode;
    if (node->op == "+") opcode = IROpcode::ADD;
    else if (node->op == "-") opcode = IROpcode::SUB;
    else if (node->op == "*") opcode = IROpcode::MUL;
    else if (node->op == "/") opcode = IROpcode::DIV;
    else if (node->op == "%") opcode = IROpcode::MOD;
    else if (node->op == "==") opcode = IROpcode::EQ;
    else if (node->op == "!=") opcode = IROpcode::NE;
    else if (node->op == "<") opcode = IROpcode::LT;
    else if (node->op == "<=") opcode = IROpcode::LE;
    else if (node->op == ">") opcode = IROpcode::GT;
    else if (node->op == ">=") opcode = IROpcode::GE;
    else if (node->op == "&&") opcode = IROpcode::AND;
    else if (node->op == "||") opcode = IROpcode::OR;
    else throw std::runtime_error("Unknown binary operator: " + node->op);
    
    current_function->addInstruction(IRInstruction(opcode, temp, left_result, right_result));
    last_result = temp;
}

void IRGenerator::visit(UnaryExpr* node) {
    node->operand->accept(this);
    std::string operand_result = last_result;
    
    std::string temp = current_function->newTemp();
    
    if (node->op == "-") {
        current_function->addInstruction(IRInstruction(IROpcode::SUB, temp, "0", operand_result));
    } else if (node->op == "!") {
        current_function->addInstruction(IRInstruction(IROpcode::NOT, temp, operand_result));
    } else if (node->op == "+") {
        temp = operand_result;  // Unary plus does nothing
    }
    
    last_result = temp;
}

void IRGenerator::visit(CallExpr* node) {
    // Push arguments
    for (auto& arg : node->args) {
        arg->accept(this);
        current_function->addInstruction(IRInstruction(IROpcode::PARAM, last_result));
    }
    
    // Call function
    std::string temp = current_function->newTemp();
    current_function->addInstruction(IRInstruction(IROpcode::CALL, temp, node->func_name));
    last_result = temp;
}

void IRGenerator::visit(IdentExpr* node) {
    // Load variable value
    std::string temp = current_function->newTemp();
    current_function->addInstruction(IRInstruction(IROpcode::LOAD, temp, node->name));
    last_result = temp;
}

void IRGenerator::visit(IntLiteralExpr* node) {
    std::string temp = current_function->newTemp();
    current_function->addInstruction(IRInstruction(IROpcode::CONST, temp, std::to_string(node->value)));
    last_result = temp;
}

void IRGenerator::visit(CharLiteralExpr* node) {
    std::string temp = current_function->newTemp();
    current_function->addInstruction(IRInstruction(IROpcode::CONST, temp, std::to_string(node->value)));
    last_result = temp;
}

void IRGenerator::visit(StringLiteralExpr* node) {
    // For now, treat strings as pointers to const data
    // In a real implementation, this would create a string constant in .rodata
    std::string temp = current_function->newTemp();
    current_function->addInstruction(IRInstruction(IROpcode::CONST, temp, "\"" + node->value + "\""));
    last_result = temp;
}

void IRGenerator::visit(ArrayAccess* node) {
    node->index->accept(this);
    std::string index_result = last_result;
    
    std::string temp = current_function->newTemp();
    // This is simplified - real array access needs offset calculation
    current_function->addInstruction(IRInstruction(IROpcode::LOAD, temp, node->array_name + "[" + index_result + "]"));
    last_result = temp;
}
