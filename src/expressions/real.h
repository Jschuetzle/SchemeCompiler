#pragma once

#include "../expression.h"

// An expression that is a constant real
class ASTExpressionReal : public ASTExpression
{
    // Constant real value.
    double value;

public:

    // Create a new constant real expression.
    // val: Constant real value to create.
    explicit ASTExpressionReal(AST& ast, double val) : ASTExpression(ast), value(val) {}

    // Create a new constant real expression.
    // val: Constant real value to create.
    static auto Create(AST& ast, double val)
    {
        return std::make_unique<ASTExpressionReal>(ast, val);
    }

    // Virtual functions. See base class for details.
    std::unique_ptr<VarType> ReturnType(ASTFunction* func) override;
    bool IsLValue(ASTFunction* func) override;
    llvm::Value* Compile(llvm::Module& mod, llvm::IRBuilder<>& builder, ASTFunction* func) override;
    std::string ToString(const std::string& prefix) override;
};
