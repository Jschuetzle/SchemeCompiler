 #pragma once

 #include "../expression.h"

 // An expression that ands two boolean expressions together.
 class ASTExpressionAnd : public ASTExpression
 {
     // Operands to work with.
     std::unique_ptr<ASTExpression> a1;
     std::unique_ptr<ASTExpression> a2;

 public:

     // Create a new expression.
     // a1: Left side expression of the and statement.
     // a2: Right side expression of the and statement.
     ASTExpressionAnd(AST& ast, std::unique_ptr<ASTExpression> a1, std::unique_ptr<ASTExpression> a2) : ASTExpression(ast), a1(std::move(a1)), a2(std::move(a2)) {}

     // Create a new expression.
     // a1: Left side expression of the and statement.
     // a2: Right side expressioni of the and statment.
     static auto Create(AST& ast, std::unique_ptr<ASTExpression> a1, std::unique_ptr<ASTExpression> a2)
     {
         return std::make_unique<ASTExpressionAnd>(ast, std::move(a1), std::move(a2));
     }

     // Virtual functions. See base class for details.
     std::unique_ptr<VarType> ReturnType(ASTFunction* func) override;
     bool IsLValue(ASTFunction* func) override;
     llvm::Value* Compile(llvm::Module& mod, llvm::IRBuilder<>& builder, ASTFunction* func) override;
     std::string ToString(const std::string& prefix) override;
 };
