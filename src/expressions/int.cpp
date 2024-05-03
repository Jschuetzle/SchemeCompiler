#include "int.h"
#include <iostream>

std::unique_ptr<VarType> ASTExpressionInt::ReturnType(ASTFunction* func)
{
    return VarTypeSimple::IntType.Copy(); 
}

bool ASTExpressionInt::IsLValue(ASTFunction* func)
{
    return false;
}

llvm::Value* ASTExpressionInt::Compile(llvm::Module& mod, llvm::IRBuilder<>& builder, ASTFunction* func)
{
    return llvm::ConstantInt::get(VarTypeSimple::IntType.GetLLVMType(builder.getContext()), value); 
}

std::string ASTExpressionInt::ToString(const std::string& prefix)
{
    return std::to_string(value) + "\n";
}
