#pragma once

#include "../expression.h"

// For an if condition statement.
class ASTExpressionIf : public ASTExpression
{

    // Condition to check.
    std::unique_ptr<ASTExpression> condition;

    // Then statement to execute.
    std::unique_ptr<ASTExpression> thenExpression;

    // Else statement to execute.
    std::unique_ptr<ASTExpression> elseExpression;

public:

    // Create a new if statement.
    // condition: Condition to check.
    // thenStatement: Statement to execute if the condition is true.
    // elseStatement: Statement to execute if the condition is false.
    ASTExpressionIf(AST& ast, std::unique_ptr<ASTExpression> condition, std::unique_ptr<ASTExpression> thenExpression, std::unique_ptr<ASTExpression> elseExpression) : ASTExpression(ast), condition(std::move(condition)), thenExpression(std::move(thenExpression)), elseExpression(std::move(elseExpression)) {}

    // Create a new if statement.
    // condition: Condition to check.
    // thenStatement: Statement to execute if the condition is true.
    // elseStatement: Statement to execute if the condition is false.
    static auto Create(AST& ast, std::unique_ptr<ASTExpression> condition, std::unique_ptr<ASTExpression> thenExpression, std::unique_ptr<ASTExpression> elseExpression)
    {
        return std::make_unique<ASTExpressionIf>(ast, std::move(condition), std::move(thenExpression), std::move(elseExpression));
    }

    // Virtual functions. See base class for details.
    std::unique_ptr<VarType> ReturnType(ASTFunction* func) override;
    bool IsLValue(ASTFunction* func) override;
    llvm::Value* Compile(llvm::Module& mod, llvm::IRBuilder<>& builder, ASTFunction* func) override;
    std::string ToString(const std::string& prefix) override;
};