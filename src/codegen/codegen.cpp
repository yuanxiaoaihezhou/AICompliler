#include "codegen.h"
#include <sstream>
#include <iostream>

CodeGenerator::CodeGenerator() : stack_offset(0) {}

std::string CodeGenerator::generate(const IRModule& module) {
    std::ostringstream result;
    
    // Assembly header
    result << ".text\n";
    result << ".global main\n\n";
    
    // Generate each function
    for (const auto& func : module.functions) {
        result << generateFunction(func);
    }
    
    return result.str();
}

std::string CodeGenerator::generateFunction(const IRFunction& func) {
    std::ostringstream result;
    var_offsets.clear();
    stack_offset = 0;
    
    result << func.name << ":\n";
    
    // Prologue
    result << "    pushq %rbp\n";
    result << "    movq %rsp, %rbp\n";
    
    // Reserve space for local variables (simplified)
    int local_space = func.temp_counter * 8 + 128;  // Extra space for safety
    if (local_space > 0) {
        result << "    subq $" << local_space << ", %rsp\n";
    }
    
    // Generate instructions
    for (const auto& instr : func.instructions) {
        result << "    # " << instr.toString() << "\n";
        
        switch (instr.opcode) {
            case IROpcode::CONST:
                result << "    movq $" << instr.arg1 << ", %rax\n";
                result << "    movq %rax, -" << (stack_offset += 8) << "(%rbp)\n";
                var_offsets[instr.result] = stack_offset;
                break;
                
            case IROpcode::LOAD:
                if (var_offsets.find(instr.arg1) != var_offsets.end()) {
                    result << "    movq -" << var_offsets[instr.arg1] << "(%rbp), %rax\n";
                } else {
                    result << "    movq " << instr.arg1 << "(%rip), %rax\n";
                }
                result << "    movq %rax, -" << (stack_offset += 8) << "(%rbp)\n";
                var_offsets[instr.result] = stack_offset;
                break;
                
            case IROpcode::STORE:
                if (var_offsets.find(instr.arg1) != var_offsets.end()) {
                    result << "    movq -" << var_offsets[instr.arg1] << "(%rbp), %rax\n";
                } else {
                    result << "    movq $" << instr.arg1 << ", %rax\n";
                }
                if (var_offsets.find(instr.result) == var_offsets.end()) {
                    var_offsets[instr.result] = (stack_offset += 8);
                }
                result << "    movq %rax, -" << var_offsets[instr.result] << "(%rbp)\n";
                break;
                
            case IROpcode::ADD:
                result << "    movq -" << var_offsets[instr.arg1] << "(%rbp), %rax\n";
                result << "    movq -" << var_offsets[instr.arg2] << "(%rbp), %rbx\n";
                result << "    addq %rbx, %rax\n";
                result << "    movq %rax, -" << (stack_offset += 8) << "(%rbp)\n";
                var_offsets[instr.result] = stack_offset;
                break;
                
            case IROpcode::SUB:
                result << "    movq -" << var_offsets[instr.arg1] << "(%rbp), %rax\n";
                result << "    movq -" << var_offsets[instr.arg2] << "(%rbp), %rbx\n";
                result << "    subq %rbx, %rax\n";
                result << "    movq %rax, -" << (stack_offset += 8) << "(%rbp)\n";
                var_offsets[instr.result] = stack_offset;
                break;
                
            case IROpcode::MUL:
                result << "    movq -" << var_offsets[instr.arg1] << "(%rbp), %rax\n";
                result << "    movq -" << var_offsets[instr.arg2] << "(%rbp), %rbx\n";
                result << "    imulq %rbx, %rax\n";
                result << "    movq %rax, -" << (stack_offset += 8) << "(%rbp)\n";
                var_offsets[instr.result] = stack_offset;
                break;
                
            case IROpcode::DIV:
                result << "    movq -" << var_offsets[instr.arg1] << "(%rbp), %rax\n";
                result << "    cqto\n";
                result << "    movq -" << var_offsets[instr.arg2] << "(%rbp), %rbx\n";
                result << "    idivq %rbx\n";
                result << "    movq %rax, -" << (stack_offset += 8) << "(%rbp)\n";
                var_offsets[instr.result] = stack_offset;
                break;
                
            case IROpcode::LABEL:
                result << instr.result << ":\n";
                break;
                
            case IROpcode::JUMP:
                result << "    jmp " << instr.result << "\n";
                break;
                
            case IROpcode::BRANCH:
                result << "    movq -" << var_offsets[instr.arg1] << "(%rbp), %rax\n";
                result << "    cmpq $0, %rax\n";
                result << "    jne " << instr.result << "\n";
                result << "    jmp " << instr.arg2 << "\n";
                break;
                
            case IROpcode::RETURN:
                if (!instr.result.empty() && var_offsets.find(instr.result) != var_offsets.end()) {
                    result << "    movq -" << var_offsets[instr.result] << "(%rbp), %rax\n";
                }
                result << "    movq %rbp, %rsp\n";
                result << "    popq %rbp\n";
                result << "    ret\n";
                break;
                
            case IROpcode::CALL:
                // Simplified call generation
                result << "    call " << instr.arg1 << "\n";
                if (!instr.result.empty()) {
                    result << "    movq %rax, -" << (stack_offset += 8) << "(%rbp)\n";
                    var_offsets[instr.result] = stack_offset;
                }
                break;
                
            case IROpcode::ALLOC:
                // Allocate space
                var_offsets[instr.result] = (stack_offset += std::stoi(instr.arg1));
                break;
                
            default:
                result << "    # Unimplemented instruction\n";
                break;
        }
    }
    
    // Epilogue (if no explicit return)
    result << "    movq %rbp, %rsp\n";
    result << "    popq %rbp\n";
    result << "    ret\n\n";
    
    return result.str();
}
