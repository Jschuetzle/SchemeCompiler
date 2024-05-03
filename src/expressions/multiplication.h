#pragma once

#include "../expression.h"

// An expression that multiplies two operands together.
class ASTExpressionMultiplication : public ASTExpression
{
    // Operands to work with.
    std::unique_ptr<ASTExpression> m1;
    std::unique_ptr<ASTExpression> m2;

    // Return type to cache.
    VarTypeSimple* returnType = nullptr;

public:
    
    // Create a new mulitplication expression.
    // m1: Left side of expression of the multiplication statement.
    // m2: Right side of expression of the multiplication statement.
    ASTExpressionMultiplication(AST& ast, std::unique_ptr<ASTExpression> m1, std::unique_ptr<ASTExpression> m2) : ASTExpression(ast), m1(std::move(m1)), m2(std::move(m2)) {}

    // Create a new mulitplication expression.
    // m1: Left side of expression of the multiplication statement.
    // m2: Right side of expression of the multiplication statement.
    static auto Create(AST& ast, std::unique_ptr<ASTExpression> m1, std::unique_ptr<ASTExpression> m2)
    {
        return std::make_unique<ASTExpressionMultiplication>(ast, std::move(m1), std::move(m2));
    }

    // Virtual functions. See base class for details.
    std::unique_ptr<VarType> ReturnType(ASTFunction* func) override;
    bool IsLValue(ASTFunction* func) override;
    llvm::Value* Compile(llvm::Module& mod, llvm::IRBuilder<>& builder, ASTFunction* func) override;
    std::string ToString(const std::string& prefix) override;
};