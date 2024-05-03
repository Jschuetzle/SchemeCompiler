#include "or.h"

#include "function.h"

std::unique_ptr<VarType> ASTExpressionOr::ReturnType(ASTFunction* func)
{
    return VarTypeSimple::BoolType.Copy(); // a || b is always a boolean.
}

bool ASTExpressionOr::IsLValue(ASTFunction* func)
{
    return false; // || operator works on two R-Values to produce an R-Value.
}

llvm::Value* ASTExpressionOr::Compile(llvm::Module& mod, llvm::IRBuilder<>& builder, ASTFunction* func) // Hm, this isn't the most efficient approach. I can think of a much easier way...
{

    // Make sure to cast both sides as booleans first.
    //ASTExpression::ImplicitCast(func, a1, &VarTypeSimple::BoolType);
    //ASTExpression::ImplicitCast(func, a2, &VarTypeSimple::BoolType);

    if(!(a1->ReturnType(func)->Equals(&VarTypeSimple::BoolType) && (a2->ReturnType(func)->Equals(&VarTypeSimple::BoolType)))){
        throw std::runtime_error("ERROR: Type mismatch on OR expression");
    }

    // Create blocks. Check right is if left is false, and we need to check the right one too. Continue block happens if true.
    llvm::Function* funcVal = nullptr;
    if(func){
        funcVal = (llvm::Function*) func->GetVariableValue(func->name);
    }

     
     llvm::Value* left = a1->CompileRValue(mod, builder, func);
     llvm::Value* right = a1->CompileRValue(mod, builder, func);
     
     return builder.CreateOr(left, right); 
}

std::string ASTExpressionOr::ToString(const std::string& prefix)
{
    std::string ret = "or\n";
    ret += prefix + "├──" + a1->ToString(prefix + "│  ");
    ret += prefix + "└──" + a2->ToString(prefix + "   ");
    return ret;
}
