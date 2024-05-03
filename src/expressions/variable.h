#pragma once

#include <utility>

#include "../expression.h"

// An expression that resolves a variable.
class ASTExpressionVariable : public ASTExpression
{
    // Referenced variable.
    std::string var;

public:

    // Resolve a variable. Functions can be variables too
    // var: Name of the variable to reference.
    explicit ASTExpressionVariable(AST& ast, std::string varName) : ASTExpression(ast), var(std::move(varName)) {}


    // Resolve a variable. Functions are variables too!
    // var: Name of the variable to reference.
    static auto Create(AST& ast, const std::string& var)
    {
        return std::make_unique<ASTExpressionVariable>(ast, var);
    }

    // Virtual functions. See base class for details.
    std::unique_ptr<VarType> ReturnType(ASTFunction* func) override;
    bool IsLValue(ASTFunction* func) override;
    llvm::Value* Compile(llvm::Module& mod, llvm::IRBuilder<>& builder, ASTFunction* func) override;
    std::string ToString(const std::string& prefix) override;
};
