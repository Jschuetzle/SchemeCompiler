#pragma once

#include "../expression.h"

// An expression that divides two operands.
class ASTExpressionRemainder : public ASTExpression
{
    // Operands to work with.
    std::unique_ptr<ASTExpression> a1;
    std::unique_ptr<ASTExpression> a2;

    // Return type to cache.
    VarTypeSimple* returnType = nullptr;

public:
    
    // Create a new remainder expression.
    // d1: Left side of expression of the remainder statement.
    // d2: Right side of expression of the remainder statement.
    ASTExpressionRemainder(AST& ast, std::unique_ptr<ASTExpression> a1, std::unique_ptr<ASTExpression> a2) : ASTExpression(ast), a1(std::move(a1)), a2(std::move(a2)) {}

    // Create a new remainder expression.
    // d1: Left side of expression of the remainder statement.
    // d2: Right side of expression of the remainder statement.
    static auto Create(AST& ast, std::unique_ptr<ASTExpression> a1, std::unique_ptr<ASTExpression> a2)
    {
        return std::make_unique<ASTExpressionRemainder>(ast, std::move(a1), std::move(a2));
    }

    // Virtual functions. See base class for details.
    std::unique_ptr<VarType> ReturnType(ASTFunction* func) override;
    bool IsLValue(ASTFunction* func) override;
    llvm::Value* Compile(llvm::IRBuilder<>& builder, ASTFunction* func) override;
    std::string ToString(const std::string& prefix) override;
};