#pragma once

#include <utility>

#include "../expression.h"

// An expression that is a constant string.
class ASTExpressionString : public ASTExpression
{
    // Constant string value.
    std::string value;

public:

    // Create a new constant string expression.
    // str: Constant string value to create.
    explicit ASTExpressionString(AST& ast, std::string  str) : ASTExpression(ast), value(std::move(str)) {}

    // Create a new constant string expression.
    // str: Constant string value to create.
    static auto Create(AST& ast, const std::string& str)
    {
        return std::make_unique<ASTExpressionString>(ast, str);
    }

    // Virtual functions. See base class for details.
    std::unique_ptr<VarType> ReturnType(ASTFunction* func) override;
    bool IsLValue(ASTFunction* func) override;
    llvm::Value* Compile(llvm::Module& mod, llvm::IRBuilder<>& builder, ASTFunction* func) override;
    std::string ToString(const std::string& prefix) override;
};
