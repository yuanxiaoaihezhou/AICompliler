#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include "ir.h"
#include <vector>
#include <map>
#include <set>

class Optimizer {
private:
    // Constant folding
    bool constantFolding(IRFunction& func);
    
    // Dead code elimination
    bool deadCodeElimination(IRFunction& func);
    
    // Constant propagation
    bool constantPropagation(IRFunction& func);
    
    // Common subexpression elimination
    bool commonSubexpressionElimination(IRFunction& func);
    
    // Helper methods
    bool isConstant(const std::string& var, const std::map<std::string, int>& constants);
    int getConstantValue(const std::string& var, const std::map<std::string, int>& constants);
    
public:
    Optimizer();
    IRModule optimize(const IRModule& module);
    IRFunction optimizeFunction(const IRFunction& func);
};

#endif // OPTIMIZER_H
