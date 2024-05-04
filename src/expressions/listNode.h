#pragma once

#include "../expression.h"
// An expression that is a constant list
class ASTExpressionListNode : public ASTExpression
{

public:

    // Constant int value.
    std::unique_ptr<ASTExpression> data;
    std::unique_ptr<ASTExpressionListNode> next;     

    // Create a new constant int expression.
    // val: Constant int value to create.
    explicit ASTExpressionListNode(AST& ast, std::unique_ptr<ASTExpression> data, std::unique_ptr<ASTExpressionListNode> next) : ASTExpression(ast), data(std::move(data)), next(std::move(next)) {}


    // Create a new constant int expression.
    // val: Constant int value to create.
    static auto Create(AST& ast, std::unique_ptr<ASTExpression> data, std::unique_ptr<ASTExpressionListNode> next)
    {
        return std::make_unique<ASTExpressionListNode>(ast, std::move(data), std::move(next));
    }

    llvm::Value* Compile(std::string name, int num, std::unique_ptr<VarType> expectedType, llvm::Module& mod, llvm::IRBuilder<>& builder, ASTFunction* func);

    // Virtual functions. See base class for details.
    std::unique_ptr<VarType> ReturnType(ASTFunction* func) override;
    bool IsLValue(ASTFunction* func) override;
    llvm::Value* Compile(llvm::Module& mod, llvm::IRBuilder<>& builder, ASTFunction* func) override;
    std::string ToString(const std::string& prefix) override;
};
