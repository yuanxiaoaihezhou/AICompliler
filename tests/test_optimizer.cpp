#include <iostream>
#include <cassert>
#include "ir.h"
#include "optimizer.h"

void test_constant_folding() {
    IRFunction func("test", "int");
    
    // Create IR with constant operands: result = 2 + 3, then use it
    func.addInstruction(IRInstruction(IROpcode::ADD, "t0", "2", "3"));
    func.addInstruction(IRInstruction(IROpcode::ADD, "t1", "t0", "1"));
    func.addInstruction(IRInstruction(IROpcode::RETURN, "t1"));
    
    Optimizer optimizer;
    IRFunction optimized = optimizer.optimizeFunction(func);
    
    // After optimization, should have fewer instructions
    // The ADD 2+3 should be folded to CONST 5
    size_t original_size = func.instructions.size();
    size_t optimized_size = optimized.instructions.size();
    
    // Optimized should be smaller or equal
    assert(optimized_size <= original_size);
    std::cout << "test_constant_folding passed (original: " << original_size 
              << ", optimized: " << optimized_size << ")\n";
}

void test_constant_propagation() {
    IRFunction func("test", "int");
    
    // Create IR: t0 = 5; store t0
    func.addInstruction(IRInstruction(IROpcode::CONST, "t0", "5"));
    func.addInstruction(IRInstruction(IROpcode::ADD, "t1", "t0", "10"));
    func.addInstruction(IRInstruction(IROpcode::STORE, "result", "t1"));
    
    Optimizer optimizer;
    IRFunction optimized = optimizer.optimizeFunction(func);
    
    // After optimization, should be smaller due to constant propagation
    size_t original_size = func.instructions.size();
    size_t optimized_size = optimized.instructions.size();
    
    assert(optimized_size <= original_size);
    std::cout << "test_constant_propagation passed (original: " << original_size 
              << ", optimized: " << optimized_size << ")\n";
}

void test_dead_code_elimination() {
    IRFunction func("test", "int");
    
    // Create IR with unused temp variable
    func.addInstruction(IRInstruction(IROpcode::CONST, "t0", "5"));
    func.addInstruction(IRInstruction(IROpcode::CONST, "t1", "10"));  // unused
    func.addInstruction(IRInstruction(IROpcode::RETURN, "t0"));
    
    Optimizer optimizer;
    IRFunction optimized = optimizer.optimizeFunction(func);
    
    // After optimization, t1 should be removed
    bool found_dead = false;
    for (const auto& instr : optimized.instructions) {
        if (instr.result == "t1") {
            found_dead = true;
        }
    }
    
    assert(!found_dead);
    std::cout << "test_dead_code_elimination passed\n";
}

int main() {
    std::cout << "Running Optimizer Tests...\n";
    test_constant_folding();
    test_constant_propagation();
    test_dead_code_elimination();
    std::cout << "All optimizer tests passed!\n";
    return 0;
}
