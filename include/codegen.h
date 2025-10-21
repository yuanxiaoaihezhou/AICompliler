#ifndef CODEGEN_H
#define CODEGEN_H

#include "ir.h"
#include <string>
#include <map>
#include <vector>
#include <sstream>

class CodeGenerator {
private:
    std::map<std::string, int> var_offsets;
    int stack_offset;
    
    void generatePrologue(const IRFunction& func);
    void generateEpilogue();
    void generateInstruction(const IRInstruction& instr);
    int getVarOffset(const std::string& var);
    void allocateVar(const std::string& var, int size);
    
public:
    CodeGenerator();
    std::string generate(const IRModule& module);
    std::string generateFunction(const IRFunction& func);
};

#endif // CODEGEN_H
