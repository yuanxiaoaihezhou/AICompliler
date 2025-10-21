#ifndef IR_H
#define IR_H

#include <string>
#include <vector>
#include <memory>
#include <map>

enum class IROpcode {
    // Arithmetic
    ADD, SUB, MUL, DIV, MOD,
    // Comparison
    EQ, NE, LT, LE, GT, GE,
    // Logical
    AND, OR, NOT,
    // Memory
    LOAD, STORE, ALLOC,
    // Control flow
    LABEL, JUMP, BRANCH,
    // Function
    CALL, RETURN, PARAM,
    // Move
    MOVE,
    // Constant
    CONST
};

class IRInstruction {
public:
    IROpcode opcode;
    std::string result;
    std::string arg1;
    std::string arg2;
    
    IRInstruction(IROpcode opcode, const std::string& result = "",
                  const std::string& arg1 = "", const std::string& arg2 = "");
    std::string toString() const;
    static std::string opcodeToString(IROpcode opcode);
};

class IRFunction {
public:
    std::string name;
    std::string return_type;
    std::vector<std::string> params;
    std::vector<IRInstruction> instructions;
    int temp_counter;
    int label_counter;
    
    IRFunction(const std::string& name, const std::string& return_type);
    std::string newTemp();
    std::string newLabel();
    void addInstruction(const IRInstruction& instr);
};

class IRModule {
public:
    std::vector<IRFunction> functions;
    std::map<std::string, int> global_vars;
    
    void addFunction(const IRFunction& func);
    std::string toString() const;
};

#endif // IR_H
