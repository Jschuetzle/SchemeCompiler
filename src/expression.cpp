#include "expression.h"
#include "ast.h"
#include "expressions/bool2Int.h"


llvm::Value* ASTExpression::CompileRValue(llvm::IRBuilder<>& builder, ASTFunction* func)
{
    llvm::Value* raw = Compile(builder, func); // First get the naturally compiled value.
    if (IsLValue(func)) // If the value is an L-Value, we need to load it.
    {
        return builder.CreateLoad(ReturnType(func)->GetLLVMType(builder.getContext()), raw); // Use the return type from this expression to load the needed value.
    }
    else return raw; // It's already an R-Value.
}

void ASTExpression::Compile(llvm::Module& mod, llvm::IRBuilder<>& builder, ASTFunction* func)
{

    // We can compile the expression itself, we just don't return anything since this is an expression and not a return statement.

    //that is debatable...all expressions return something in scheme...
    //this certainly does not seem correct to me
    Compile(builder, func);

}

bool ASTExpression::CoerceMathTypes(ASTFunction* func, std::unique_ptr<ASTExpression>& a1, std::unique_ptr<ASTExpression>& a2, VarTypeSimple*& outCoercedType) 
{
    // Gather return types
    auto r1 = a1->ReturnType(func);
    auto r2 = a2->ReturnType(func);

    // Check that r1 and r2 are the same
    if (!(r1.get()->Equals(r2.get())))
        return false;
    
    // Check the r1 and r2 are numbers and set return type
    if (r1->Equals(&VarTypeSimple::IntType))
        outCoercedType = &VarTypeSimple::IntType;
    else if (r1->Equals(&VarTypeSimple::RealType))
        outCoercedType = &VarTypeSimple::RealType;
    else
        return false;
    
    return true;
}

bool ASTExpression::CoerceTypes(AST& ast, ASTFunction* func, std::unique_ptr<ASTExpression>& a1, std::unique_ptr<ASTExpression>& a2, VarTypeSimple*& outCoercedType)
{
    // just convert bool to int and call convert math types
    if (a1->ReturnType(func)->Equals(&VarTypeSimple::BoolType))
    {
        auto tmp = std::move(a1);
        a1 = std::make_unique<ASTExpressionBool2Int>(ast, std::move(tmp));
    }

    if (a2->ReturnType(func)->Equals(&VarTypeSimple::BoolType))
    {
        auto tmp = std::move(a2);
        a2 = std::make_unique<ASTExpressionBool2Int>(ast, std::move(tmp));
    }

    return CoerceMathTypes(func, a1, a2, outCoercedType);
}
