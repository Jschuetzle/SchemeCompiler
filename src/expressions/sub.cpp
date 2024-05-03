#include "sub.h"

std::unique_ptr<VarType> ASTExpressionSubtraction::ReturnType(ASTFunction* func)
{
    if (!returnType) // If the return type has not been gotten yet.
    {
        if (!ASTExpression::CoerceMathTypes(ast, func, s1, s2, returnType)) // This will force our arguments to be the same type and outputs which one it is.
            throw std::runtime_error("ERROR: Can not coerce types in subtraction expression! Are they both either ints or floats?");
    }
    return std::make_unique<VarTypeSimple>(*returnType); // Make a copy of our return type.
}

bool ASTExpressionSubtraction::IsLValue(ASTFunction* func)
{
    return false; // If we are subtracting values, they must be using R-values. Subtracting these values just results in an R-value.
}

llvm::Value* ASTExpressionSubtraction::Compile(llvm::Module& mod, llvm::IRBuilder<>& builder, ASTFunction* func)
{
    // Compile the values as needed. Remember, we can only do operations on R-values.
    auto retType = ReturnType(func);
    if (retType->Equals(&VarTypeSimple::IntType)) // Do standard subtraction on integer operands since we return an int.
        return builder.CreateSub(s1->CompileRValue(mod, builder, func), s2->CompileRValue(mod, builder, func));
    else if (retType->Equals(&VarTypeSimple::RealType)) // Do subtraction on floating point operands since we return a float.
        return builder.CreateFSub(s1->CompileRValue(mod, builder, func), s2->CompileRValue(mod, builder, func));
    else
        throw std::runtime_error("ERROR: Can not perform subtraction! Are both inputs either ints or floats?");
}

std::string ASTExpressionSubtraction::ToString(const std::string& prefix)
{
    std::string ret = "(-)\n";
    ret += prefix + "├──" + s1->ToString(prefix + "| ");
    ret += prefix + "└──" + s2->ToString(prefix + "  ");
    return ret;
}