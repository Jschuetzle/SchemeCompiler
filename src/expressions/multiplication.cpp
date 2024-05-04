#include "multiplication.h"

std::unique_ptr<VarType> ASTExpressionMultiplication::ReturnType(ASTFunction* func)
{
    if (!returnType) // If the return type has not been gotten yet.
    {
        if (!ASTExpression::CoerceMathTypes(ast, func, m1, m2, returnType)) // This will force our arguments to be the same type and outputs which one it is.
            throw std::runtime_error("ERROR: Can not coerce types in multiplication expression! Are they both either ints or reals?");
    }
    return std::make_unique<VarTypeSimple>(*returnType); // Make a copy of our return type.
}

bool ASTExpressionMultiplication::IsLValue(ASTFunction* func)
{   
    return false; // If we are multiplying values together, they must be usable R-Values. Multiplying these together just results in an R-Value.
}

llvm::Value* ASTExpressionMultiplication::Compile(llvm::Module& mod, llvm::IRBuilder<>& builder, ASTFunction* func)
{
    // Compile the values as needed. Remember, we can only do operations on R-Values.
    auto retType = ReturnType(func);
    
    if (retType->Equals(&VarTypeSimple::IntType)) // Do standard multiplication on integer operands since we return an int.
        return builder.CreateMul(m1->CompileRValue(mod, builder, func), m2->CompileRValue(mod, builder, func));
    else if (retType->Equals(&VarTypeSimple::RealType)) // Do multiplication on floating point operands since we return a float.
        return builder.CreateFMul(m1->CompileRValue(mod, builder, func), m2->CompileRValue(mod, builder, func));
    else // Call to return type should make this impossible, but best to keep it here just in case of a bug.
        throw std::runtime_error("ERROR: Can not perform this multiplication! Are both inputs either ints or floats?");
}

std::string ASTExpressionMultiplication::ToString(const std::string& prefix)
{
    std::string ret = "(*)\n";
    ret += prefix + "├──" + m1->ToString(prefix + "│  ");
    ret += prefix + "└──" + m2->ToString(prefix + "   ");
    return ret;
}