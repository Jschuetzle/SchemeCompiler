#pragma once

#include "../expression.h"

// Check types
enum ASTExpressionCheckType
{
    NullType,
    BoolType,
    IntType,
    RealType,
    ListType
};

// An expression that checks type
class ASTExpressionCheck : public ASTExpression
{
    // Type to check against
    ASTExpressionCheckType type;

    // The operand to check
    std::unique_ptr<ASTExpression> operand;

    // Return type to cache
    VarTypeSimple* returnType = nullptr;

public:
    // Create a new check expression
    // Type: type to check against
    // Operand: operand to check
    ASTExpressionCheck(AST& ast, ASTExpressionCheckType type, std::unique_ptr<ASTExpression> operand) : ASTExpression(ast), type(type), operand(std::move(operand)) {}

    // Create a new check expression
    // Type: type to check against
    // Operand: operand to check
    static auto Create(AST& ast, ASTExpressionCheckType type, std::unique_ptr<ASTExpression> operand)
    {
        return std::make_unique<ASTExpressionCheck>(ast, type, std::move(operand));
    }

    // Virtual functions. See base class for details
    std::unique_ptr<VarType> ReturnType(ASTFunction* func) override;
    bool IsLValue(ASTFunction* func) override;
    llvm::Value* Compile(llvm::Module& mod, llvm::IRBuilder<>& builder, ASTFunction* func) override;
    std::string ToString(const std::string& prefix) override;
};