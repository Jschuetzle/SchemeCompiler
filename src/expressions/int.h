#pragma once

#include "../expression.h"
#include <iostream>

// An expression that is a constant int.
class ASTExpressionInt : public ASTExpression
{
    // Constant int value.
    int value;

public:

    // Create a new constant int expression.
    // val: Constant int value to create.
    explicit ASTExpressionInt(AST& ast, int val) : ASTExpression(ast), value(val) {}


    // Create a new constant int expression.
    // val: Constant int value to create.
    static auto Create(AST& ast, int val)
    {
        return std::make_unique<ASTExpressionInt>(ast, val);
    }

    // Virtual functions. See base class for details.
    std::unique_ptr<VarType> ReturnType(ASTFunction* func) override;
    bool IsLValue(ASTFunction* func) override;
    llvm::Value* Compile(llvm::Module& mod, llvm::IRBuilder<>& builder, ASTFunction* func) override;
    std::string ToString(const std::string& prefix) override;
};
