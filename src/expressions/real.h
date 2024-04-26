#pragma once

#include "../expression.h"

// An expression that is a constant real
class ASTExpressionReal : public ASTExpression
{
    // Constant real value.
    float value;

public:

    // Create a new constant real expression.
    // val: Constant real value to create.
    explicit ASTExpressionReal(float val) : value(val) {}

    // Create a new constant real expression.
    // val: Constant real value to create.
    static auto Create(float val)
    {
        return std::make_unique<ASTExpressionReal>(val);
    }

    // Virtual functions. See base class for details.
    std::unique_ptr<VarType> ReturnType(ASTFunction& func) override;
    bool IsLValue(ASTFunction& func) override;
    llvm::Value* Compile(llvm::IRBuilder<>& builder, ASTFunction& func) override;
    std::string ToString(const std::string& prefix) override;
};
