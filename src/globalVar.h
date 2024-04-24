#pragma once

#include "varType.h"
#include <llvm/IR/GlobalVariable.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <string>
#include <memory>

// Forward declaration
class AST;

class ASTGlobalVar {
private:

    // Reference to the AST, for consistency and possibly scope management
    AST& ast;  

public:

    std::string name;  // Name of the global variable

    std::unique_ptr<VarType> type;  // Type of the global variable

    // Constructor
    ASTGlobalVar(AST& ast, const std::string& name, std::unique_ptr<VarType> type);
 
    // Get a string representation of this global variable
    // prefix: the prefix of this node's children. Not exactly sure what this means yet, but it has to do with the Log output
    std::string ToString(const std::string& prefix);

    // Compile the variable, needed during codegen phase to show LLVM our global exists.
    // mod: Module to add the global variable to.
    // builder: IR builder used to build instructions.
    void Compile(llvm::Module& mod, llvm::IRBuilder<>& builder); 
};

