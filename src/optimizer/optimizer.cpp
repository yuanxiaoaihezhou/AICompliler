#include "optimizer.h"
#include <algorithm>

Optimizer::Optimizer() {}

IRModule Optimizer::optimize(const IRModule& module) {
    IRModule optimized_module;
    optimized_module.global_vars = module.global_vars;
    
    for (const auto& func : module.functions) {
        optimized_module.functions.push_back(optimizeFunction(func));
    }
    
    return optimized_module;
}

IRFunction Optimizer::optimizeFunction(const IRFunction& func) {
    IRFunction optimized = func;
    
    bool changed = true;
    int iterations = 0;
    const int max_iterations = 10;
    
    // Iterate until no more changes or max iterations
    while (changed && iterations < max_iterations) {
        changed = false;
        changed |= constantFolding(optimized);
        changed |= constantPropagation(optimized);
        changed |= deadCodeElimination(optimized);
        iterations++;
    }
    
    return optimized;
}

bool Optimizer::constantFolding(IRFunction& func) {
    bool changed = false;
    std::vector<IRInstruction> new_instructions;
    
    for (auto& instr : func.instructions) {
        // Try to fold binary operations with constant operands
        if (instr.opcode == IROpcode::ADD || instr.opcode == IROpcode::SUB ||
            instr.opcode == IROpcode::MUL || instr.opcode == IROpcode::DIV ||
            instr.opcode == IROpcode::MOD) {
            
            // Check if both operands are constants (start with digit or minus)
            bool arg1_is_const = !instr.arg1.empty() && 
                                 (std::isdigit(instr.arg1[0]) || instr.arg1[0] == '-');
            bool arg2_is_const = !instr.arg2.empty() && 
                                 (std::isdigit(instr.arg2[0]) || instr.arg2[0] == '-');
            
            if (arg1_is_const && arg2_is_const) {
                int val1 = std::stoi(instr.arg1);
                int val2 = std::stoi(instr.arg2);
                int result = 0;
                
                switch (instr.opcode) {
                    case IROpcode::ADD: result = val1 + val2; break;
                    case IROpcode::SUB: result = val1 - val2; break;
                    case IROpcode::MUL: result = val1 * val2; break;
                    case IROpcode::DIV: 
                        if (val2 != 0) result = val1 / val2; 
                        else goto keep_instruction;
                        break;
                    case IROpcode::MOD: 
                        if (val2 != 0) result = val1 % val2;
                        else goto keep_instruction;
                        break;
                    default: break;
                }
                
                // Replace with constant assignment
                IRInstruction new_instr(IROpcode::CONST, instr.result, std::to_string(result));
                new_instructions.push_back(new_instr);
                changed = true;
                continue;
            }
        }
        
        keep_instruction:
        new_instructions.push_back(instr);
    }
    
    func.instructions = new_instructions;
    return changed;
}

bool Optimizer::constantPropagation(IRFunction& func) {
    bool changed = false;
    std::map<std::string, std::string> constants;
    std::vector<IRInstruction> new_instructions;
    
    for (auto& instr : func.instructions) {
        // Track constant assignments
        if (instr.opcode == IROpcode::CONST) {
            constants[instr.result] = instr.arg1;
            new_instructions.push_back(instr);
            continue;
        }
        
        // Replace uses of constants
        IRInstruction new_instr = instr;
        
        if (constants.find(instr.arg1) != constants.end()) {
            new_instr.arg1 = constants[instr.arg1];
            changed = true;
        }
        
        if (constants.find(instr.arg2) != constants.end()) {
            new_instr.arg2 = constants[instr.arg2];
            changed = true;
        }
        
        // Clear constants on store (conservative approach)
        if (instr.opcode == IROpcode::STORE || instr.opcode == IROpcode::CALL) {
            constants.clear();
        }
        
        new_instructions.push_back(new_instr);
    }
    
    func.instructions = new_instructions;
    return changed;
}

bool Optimizer::deadCodeElimination(IRFunction& func) {
    bool changed = false;
    std::set<std::string> used_vars;
    std::set<std::string> defined_vars;
    
    // First pass: collect all used and defined variables
    for (const auto& instr : func.instructions) {
        // Add uses
        if (!instr.arg1.empty() && instr.arg1[0] == 't') {
            used_vars.insert(instr.arg1);
        }
        if (!instr.arg2.empty() && instr.arg2[0] == 't') {
            used_vars.insert(instr.arg2);
        }
        
        // Add definitions
        if (!instr.result.empty() && instr.result[0] == 't') {
            defined_vars.insert(instr.result);
        }
    }
    
    // Second pass: remove instructions that define unused temporaries
    std::vector<IRInstruction> new_instructions;
    for (const auto& instr : func.instructions) {
        // Keep instructions with side effects
        if (instr.opcode == IROpcode::STORE || 
            instr.opcode == IROpcode::CALL ||
            instr.opcode == IROpcode::RETURN ||
            instr.opcode == IROpcode::JUMP ||
            instr.opcode == IROpcode::BRANCH ||
            instr.opcode == IROpcode::LABEL ||
            instr.opcode == IROpcode::PARAM ||
            instr.opcode == IROpcode::ALLOC) {
            new_instructions.push_back(instr);
            continue;
        }
        
        // Keep if result is used
        if (instr.result.empty() || used_vars.find(instr.result) != used_vars.end()) {
            new_instructions.push_back(instr);
        } else {
            changed = true;
        }
    }
    
    func.instructions = new_instructions;
    return changed;
}

bool Optimizer::commonSubexpressionElimination(IRFunction& func) {
    // Simple CSE implementation
    // TODO: Implement a more sophisticated version
    return false;
}

bool Optimizer::isConstant(const std::string& var, const std::map<std::string, int>& constants) {
    return constants.find(var) != constants.end();
}

int Optimizer::getConstantValue(const std::string& var, const std::map<std::string, int>& constants) {
    return constants.at(var);
}
