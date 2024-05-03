#pragma once

#include "types/simple.h"
#include "types/list.h"
#include "varType.h"
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Value.h>
#include <memory>

// Forward declarations.
class AST;
class ASTFunction;

// Abstract class for expressions (funcalls, math/logic operators, built infunctions like if, cond, lambda)
class ASTExpression {
public:

    AST& ast;

    explicit ASTExpression(AST& ast) : ast(ast) {}

    // Must make the destructor virtual to make the compiler happy.
    virtual ~ASTExpression() = default;

    // Get the return type of this expression.
    // func: Function that contains this expression.
    // Returns: A variable type that this expression returns.
    /*
       note...default value is null because it may be the case where the variables referenced 
       in the expression are in the global scope and not the function scope. IMPORTANT: the 
       default value provided here extends to all the overriden versions of this method.
       If it is not properly handled in the subclass implementations, seg faults will occur
    */
    virtual std::unique_ptr<VarType> ReturnType(ASTFunction* func = nullptr) = 0;


    // Returns if the result is an L-Value. See the design document for details.
    // func: Function that contains this expression.
    // Returns: If this expression results in an L-Value.
    virtual bool IsLValue(ASTFunction* func = nullptr) = 0;


    // Compile the expression and get its returned value.
    // builder: LLVM IR builder.
    // func: Function that contains this expression.
    // Returns: An LLVM value. Can be null if void is returned, you may need ReturnType for the current expression or arguments to type check.
    virtual llvm::Value* Compile(llvm::Module& mod,  llvm::IRBuilder<>& builder, ASTFunction* func = nullptr) = 0;

    
    // Recursively build a string representation of this expression.
    // Prefix: the string to be inserted on the left side of the tree.
    // Returns: a string representation of this expression, in the form of a tree data structure. 
    virtual std::string ToString(const std::string& prefix) = 0;


    // Compile the expression as an R-Value (load from it if it is an L-Value already).
    // builder: LLVM IR builder.
    // func: Function that contains this expression.
    // Returns: An LLVM value. Can be null if void is returned.
    llvm::Value* CompileRValue(llvm::Module& mod, llvm::IRBuilder<>& builder, ASTFunction* func = nullptr);

    // Coerce math types.
    // As of now it does not coerce types, but only checks if they are the same.
    // func: Function that contains this expression.
    // a1: Reference to the first expression operand.
    // a2: Reference to the second expression operand.
    // outCoercedType: Where to output the type.
    // Returns: If types are the same and numbers.
    static bool CoerceMathTypes(AST& ast, ASTFunction* func, std::unique_ptr<ASTExpression>& a1, std::unique_ptr<ASTExpression>& a2, VarTypeSimple*& outCoercedType);

    // Coerce types including bool to int
    static bool CoerceTypes(AST& ast, ASTFunction* func, std::unique_ptr<ASTExpression>& a1, std::unique_ptr<ASTExpression>& a2, VarTypeSimple*& outCoercedType);

    // DO NOT CALL THIS FROM EXPRESSION SUBCLASSES! THERE'S A GOOD CHANCE IT WON'T DO WHAT YOU WANT IT TO!
    //void Compile(llvm::Module& mod, llvm::IRBuilder<>& builder, ASTFunction* func);
};
