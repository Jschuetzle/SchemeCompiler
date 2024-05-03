#include "real.h"

std::unique_ptr<VarType> ASTExpressionReal::ReturnType(ASTFunction* func)
{
    return VarTypeSimple::RealType.Copy();
}

bool ASTExpressionReal::IsLValue(ASTFunction* func)
{
    return false;
}

llvm::Value* ASTExpressionReal::Compile(llvm::Module& mod, llvm::IRBuilder<>& builder, ASTFunction* func)
{
    return llvm::ConstantFP::get(VarTypeSimple::RealType.GetLLVMType(builder.getContext()), value); // Simply just create an double constant to return.
}

std::string ASTExpressionReal::ToString(const std::string& prefix)
{
    return std::to_string(value) + "\n";
}
