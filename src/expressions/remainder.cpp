#include "remainder.h"

std::unique_ptr<VarType> ASTExpressionRemainder::ReturnType(ASTFunction* func)
{
    if (!returnType) // If the return type has not been gotten yet.
    {
        if (!ASTExpression::CoerceMathTypes(ast, func, a1, a2, returnType)) // This will force our arguments to be the same type and outputs which one it is.
            throw std::runtime_error("ERROR: Can not coerce types in remainder expression! Are they both either ints or floats?");
    }
    return std::make_unique<VarTypeSimple>(*returnType); // Make a copy of our return type.
}

bool ASTExpressionRemainder::IsLValue(ASTFunction* func)
{   
    return false; // If we are remainding values, they must be usable R-Values. Remainding these just results in an R-Value.
}

llvm::Value* ASTExpressionRemainder::Compile(llvm::Module& mod, llvm::IRBuilder<>& builder, ASTFunction* func)
{
    // Compile the values as needed. Remember, we can only do operations on R-Values.
    auto retType = ReturnType(func);
    if (retType->Equals(&VarTypeSimple::IntType)) // Do standard remainder on integer operands since we return an int.
        return builder.CreateSRem(a1->CompileRValue(mod, builder, func), a2->CompileRValue(mod, builder, func));
    else if (retType->Equals(&VarTypeSimple::RealType)) // Do remainder on floating point operands since we return a float.
        return builder.CreateFRem(a1->CompileRValue(mod, builder, func), a2->CompileRValue(mod, builder, func));
    else // Call to return type should make this impossible, but best to keep it here just in case of a bug.
        throw std::runtime_error("ERROR: Can not perform this remainder! Are both inputs either ints or floats?");
}

std::string ASTExpressionRemainder::ToString(const std::string& prefix)
{
    std::string ret = "(%)\n";
    ret += prefix + "├──" + a1->ToString(prefix + "│  ");
    ret += prefix + "└──" + a2->ToString(prefix + "   ");
    return ret;
}