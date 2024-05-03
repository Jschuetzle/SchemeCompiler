 #pragma once

 #include "../expression.h"

 // An expression that negates a single boolean expression
 class ASTExpressionNot : public ASTExpression
 {
     // Operand to work with.
     std::unique_ptr<ASTExpression> a1;

 public:

     // Create a new expression
     // a1: expression of the not statement.
     ASTExpressionNot(AST& ast, std::unique_ptr<ASTExpression> a1) : ASTExpression(ast), a1(std::move(a1)) {}

     // Create a new expression.
     // a1: expression of the not statement.
     static auto Create(AST& ast, std::unique_ptr<ASTExpression> a1)
     {
         return std::make_unique<ASTExpressionNot>(ast, std::move(a1));
     }

     // Virtual functions. See base class for details.
     std::unique_ptr<VarType> ReturnType(ASTFunction* func) override;
     bool IsLValue(ASTFunction* func) override;
     llvm::Value* Compile(llvm::Module& mod, llvm::IRBuilder<>& builder, ASTFunction* func) override;
     std::string ToString(const std::string& prefix) override;
 };
