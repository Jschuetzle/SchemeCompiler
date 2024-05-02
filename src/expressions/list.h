#pragma once

#include "../expression.h"
#include "listNode.h"

// An expression that represents a linked list object
class ASTExpressionList : public ASTExpression
{
    // Constant int value.
        std::unique_ptr<ASTExpressionListNode> head;

        std::unique_ptr<VarTypeList> listType;

public:

    // Create a new constant int expression.
    // val: Constant int value to create.
    explicit ASTExpressionList(AST& ast, std::unique_ptr<VarType> elementType, std::unique_ptr<ASTExpressionListNode> head);


    // Create a new constant int expression.
    // val: Constant int value to create.
    static auto Create(AST& ast, std::unique_ptr<VarType> elementType, std::unique_ptr<ASTExpressionListNode> head)
    {
        return std::make_unique<ASTExpressionList>(ast, std::move(elementType), std::move(head));
    }

    // Virtual functions. See base class for details.
    std::unique_ptr<VarType> ReturnType(ASTFunction* func) override;
    bool IsLValue(ASTFunction* func) override;
    llvm::Value* Compile(llvm::IRBuilder<>& builder, ASTFunction* func) override;
    std::string ToString(const std::string& prefix) override;
};
