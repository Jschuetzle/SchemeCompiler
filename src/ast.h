#pragma once

#include "expression.h"
#include "expressions/function.h"
#include "expressions/list.h"
#include "scopeTable.h"
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>

class AST
{

    // Main LLVM context. Note: If we want support for multiple compilation units, this should be moved elsewhere.
    llvm::LLVMContext context;

    // Module containing all functions.
    llvm::Module module;

    // Builder to build IR code in functions.
    llvm::IRBuilder<> builder;

    // list of binding names; needed in order to compile bindings in the order they appear
    std::vector<std::string> globalVarList;

    // Map that stores the (name, expr) bindings
    std::map<std::string, std::unique_ptr<ASTExpression>> globalVars;

    // List to store all the expressions that are supposed to be executed in the global scope
    std::vector<std::unique_ptr<ASTExpression>> globalExprs;

    // If the module has been compiled or not.
    bool compiled = false;

public:

    // Function pass manager for function optimizations.
    llvm::legacy::FunctionPassManager fpm;

    // Scope table for bindings
    ScopeTable scopeTable;

    // Create a new abstract syntax tree.
    // modName: Name of the module to create.
    AST(std::string modName);


    //Add a global variable to the AST
    //name: Name of the global binding to add
    //expr: ASTExpression that will be bound to the name
    //Returns: a pointer to the ASTExpression in the new global variable
    ASTExpression* AddGlobalVariable(const std::string& name, std::unique_ptr<ASTExpression> expr);
	   
    //Get a global variable from a name
    //name: Name of global variable to fetch
    //Returns: A pointer to the global variable. Throws an exception if it does not exist.
    ASTExpression* GetGlobalVariable(const std::string& name); 

    //Add an expression to the AST (which will be executed from global context)
    //expr: Expression to be executed
    //returns: a pointer to the newly added expression
    ASTExpression* AddExpressionCall(std::unique_ptr<ASTExpression> expr);

     // Compile the AST. This must be done before exporting any object files
    void Compile();

    // Get a string representation of the AST.
    std::string ToString();

    //helper method for ToString. Creates the string of a binding
    std::string bindingString(std::string name, std::unique_ptr<ASTExpression> expr, bool end);

    // Write LLVM assembly (.ll) to file. Must be done after compilation.
    // outFile: Where to write the .ll file.
    void WriteLLVMAssemblyToFile(const std::string& outFile);

    // Write LLVM bitcode (.bc) to file. Must be done after compilation.
    // outFile: Where to write the .bc file.
    void WriteLLVMBitcodeToFile(const std::string& outFile);

};
