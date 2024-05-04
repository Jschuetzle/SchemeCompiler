#include <iostream>
#include "negative.h"

std::unique_ptr<VarType> ASTExpressionNegation::ReturnType(ASTFunction* func)
{
    return VarTypeSimple::IntType.Copy();
}

bool ASTExpressionNegation::IsLValue(ASTFunction* func)
{
    return false;
}

llvm::Value* ASTExpressionNegation::Compile(llvm::Module& mod, llvm::IRBuilder<>& builder, ASTFunction* func)
{
    auto retType = ReturnType(func);
    // Negate should invert numbers
    if (retType->Equals(&VarTypeSimple::RealType))
        return builder.CreateFMul(operand->CompileRValue(mod, builder, func), llvm::ConstantFP::get(VarTypeSimple::RealType.GetLLVMType(builder.getContext()), llvm::APFloat(-1.0)));
    else if (retType->Equals(&VarTypeSimple::IntType))
        return builder.CreateMul(operand->CompileRValue(mod, builder, func), llvm::ConstantInt::get(VarTypeSimple::IntType.GetLLVMType(builder.getContext()), -1));
    else
        throw std::runtime_error("ERROR: Expected number or boolean operand in negation but got another type instead.");
}

std::string ASTExpressionNegation::ToString(const std::string& prefix)
{
    return "(-)\n" + prefix + "└──" + operand->ToString(prefix + "  ");
}