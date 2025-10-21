#include "ir.h"
#include <sstream>

IRInstruction::IRInstruction(IROpcode opcode, const std::string& result,
                             const std::string& arg1, const std::string& arg2)
    : opcode(opcode), result(result), arg1(arg1), arg2(arg2) {}

std::string IRInstruction::toString() const {
    std::ostringstream oss;
    
    switch (opcode) {
        case IROpcode::ADD:
        case IROpcode::SUB:
        case IROpcode::MUL:
        case IROpcode::DIV:
        case IROpcode::MOD:
        case IROpcode::EQ:
        case IROpcode::NE:
        case IROpcode::LT:
        case IROpcode::LE:
        case IROpcode::GT:
        case IROpcode::GE:
        case IROpcode::AND:
        case IROpcode::OR:
            oss << result << " = " << arg1 << " " << opcodeToString(opcode) << " " << arg2;
            break;
        case IROpcode::NOT:
            oss << result << " = " << opcodeToString(opcode) << " " << arg1;
            break;
        case IROpcode::LOAD:
            oss << result << " = LOAD " << arg1;
            break;
        case IROpcode::STORE:
            oss << "STORE " << arg1 << ", " << result;
            break;
        case IROpcode::ALLOC:
            oss << result << " = ALLOC " << arg1;
            break;
        case IROpcode::LABEL:
            oss << result << ":";
            break;
        case IROpcode::JUMP:
            oss << "JUMP " << result;
            break;
        case IROpcode::BRANCH:
            oss << "BRANCH " << arg1 << ", " << result << ", " << arg2;
            break;
        case IROpcode::CALL:
            if (!result.empty()) {
                oss << result << " = ";
            }
            oss << "CALL " << arg1;
            break;
        case IROpcode::RETURN:
            oss << "RETURN";
            if (!result.empty()) {
                oss << " " << result;
            }
            break;
        case IROpcode::PARAM:
            oss << "PARAM " << result;
            break;
        case IROpcode::MOVE:
            oss << result << " = " << arg1;
            break;
        case IROpcode::CONST:
            oss << result << " = " << arg1;
            break;
    }
    
    return oss.str();
}

std::string IRInstruction::opcodeToString(IROpcode opcode) {
    switch (opcode) {
        case IROpcode::ADD: return "+";
        case IROpcode::SUB: return "-";
        case IROpcode::MUL: return "*";
        case IROpcode::DIV: return "/";
        case IROpcode::MOD: return "%";
        case IROpcode::EQ: return "==";
        case IROpcode::NE: return "!=";
        case IROpcode::LT: return "<";
        case IROpcode::LE: return "<=";
        case IROpcode::GT: return ">";
        case IROpcode::GE: return ">=";
        case IROpcode::AND: return "&&";
        case IROpcode::OR: return "||";
        case IROpcode::NOT: return "!";
        default: return "UNKNOWN";
    }
}

IRFunction::IRFunction(const std::string& name, const std::string& return_type)
    : name(name), return_type(return_type), temp_counter(0), label_counter(0) {}

std::string IRFunction::newTemp() {
    return "t" + std::to_string(temp_counter++);
}

std::string IRFunction::newLabel() {
    return "L" + std::to_string(label_counter++);
}

void IRFunction::addInstruction(const IRInstruction& instr) {
    instructions.push_back(instr);
}

void IRModule::addFunction(const IRFunction& func) {
    functions.push_back(func);
}

std::string IRModule::toString() const {
    std::ostringstream oss;
    
    for (const auto& func : functions) {
        oss << "function " << func.name << "(" << func.return_type << "):\n";
        for (const auto& param : func.params) {
            oss << "  param " << param << "\n";
        }
        for (const auto& instr : func.instructions) {
            oss << "  " << instr.toString() << "\n";
        }
        oss << "\n";
    }
    
    return oss.str();
}
