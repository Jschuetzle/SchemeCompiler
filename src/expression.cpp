#include "expression.h"
#include "ast.h"


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
