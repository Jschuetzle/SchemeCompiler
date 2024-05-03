#include "expression.h"
#include "ast.h"
#include "expressions/bool2Int.h"
#include "expressions/int2Float.h"


llvm::Value* ASTExpression::CompileRValue(llvm::Module& mod, llvm::IRBuilder<>& builder, ASTFunction* func)
{
    llvm::Value* raw = Compile(mod, builder, func); // First get the naturally compiled value.
    if (IsLValue(func)) // If the value is an L-Value, we need to load it.
    {
        return builder.CreateLoad(ReturnType(func)->GetLLVMType(builder.getContext()), raw); // Use the return type from this expression to load the needed value.
    }
    else return raw; // It's already an R-Value.
}

llvm::Value* ASTExpression::Compile(llvm::Module& mod, llvm::IRBuilder<>& builder, ASTFunction* func)
{

    // We can compile the expression itself, we just don't return anything since this is an expression and not a return statement.

    //that is debatable...all expressions return something in scheme...
    //this certainly does not seem correct to me
    return Compile(mod, builder, func);

}

bool ASTExpression::CoerceMathTypes(AST& ast, ASTFunction* func, std::unique_ptr<ASTExpression>& a1, std::unique_ptr<ASTExpression>& a2, VarTypeSimple*& outCoercedType) 
{
    // Gather return types
    auto r1 = a1->ReturnType(func);
    auto r2 = a2->ReturnType(func);

    // Make sure r1 is either a float or an int
    bool r1Float = r1->Equals(&VarTypeSimple::RealType);
    if (!(r1Float) && !(r1->Equals(&VarTypeSimple::IntType))) return false;

    // Make sure r2 is either a float or an int
    bool r2Float = r2->Equals(&VarTypeSimple::RealType);
    if (!(r2Float) && !(r2->Equals(&VarTypeSimple::IntType))) return false;

    // Do casting as needed
    if (r1Float)
    {
        // both are floats, no conversion needed
        if (r2Float)
        {
            outCoercedType = &VarTypeSimple::RealType;
        }
        // cast r2 to a float
        else
        {
            outCoercedType = &VarTypeSimple::RealType;
            auto tmp = std::move(a2);
            a2 = std::make_unique<ASTExpressionInt2Float>(ast, std::move(tmp));
        }
    }
    else
    {
        // Cast r1 to float
        if (r2Float)
        {
            outCoercedType = &VarTypeSimple::RealType;
            auto tmp = std::move(a1);
            a1 = std::make_unique<ASTExpressionInt2Float>(ast, std::move(tmp));
        }
        // both are ints, no casting needed
        else
        {
            outCoercedType = &VarTypeSimple::IntType;
        }
    }

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

    return CoerceMathTypes(ast, func, a1, a2, outCoercedType);
}
