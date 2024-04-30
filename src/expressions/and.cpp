#include "and.h"

//#include "../function.h"

std::unique_ptr<VarType> ASTExpressionAnd::ReturnType(ASTFunction* func)
{
    return VarTypeSimple::BoolType.Copy(); 
}

bool ASTExpressionAnd::IsLValue(ASTFunction* func)
{
    return false; // && operator works on two R-Values to produce an R-Value.
}

llvm::Value* ASTExpressionAnd::Compile(llvm::IRBuilder<>& builder, ASTFunction* func)
{
    // Make sure to cast both sides as booleans first.
    //ASTExpression::ImplicitCast(func, a1, &VarTypeSimple::BoolType);
    //ASTExpression::ImplicitCast(func, a2, &VarTypeSimple::BoolType);

    // not implementing implicit casting, rather just check if the expressions have returnType boolean
    if(!(a1->ReturnType(func)->Equals(&VarTypeSimple::BoolType) && (a2->ReturnType(func)->Equals(&VarTypeSimple::BoolType)))){
        throw std::runtime_error("ERROR: Type mismatch on AND expression");
    }

    llvm::Function* funcVal = nullptr;
    if(func){
      funcVal = (llvm::Function*) func->GetVariableValue(func->name);   
    } 
    
    // Create blocks. Check right is if left is true. Continue block happens if false.
    llvm::BasicBlock* checkRight = llvm::BasicBlock::Create(builder.getContext(), "checkRight", funcVal);
    llvm::BasicBlock* cont = llvm::BasicBlock::Create(builder.getContext(), "cont", funcVal);

    // If left is true, then we need to check right. Otherwise branch to the continue block where we know the final result will be set as false.
    llvm::Value* tmp = a1->CompileRValue(builder, func);
    builder.CreateCondBr(tmp, checkRight, cont); // If left value is true, we branch to checking the right one, else continue.

    // Compile the right expression if needed.
    builder.SetInsertPoint(checkRight);
    tmp = a2->CompileRValue(builder, func);
    builder.CreateBr(cont);
    
    // tmp holds truthyness of operation.
    builder.SetInsertPoint(cont);
    return tmp;
}

std::string ASTExpressionAnd::ToString(const std::string& prefix)
{
    std::string ret = "(&&)\n";
    ret += prefix + "├──" + a1->ToString(prefix + "│  ");
    ret += prefix + "└──" + a2->ToString(prefix + "   ");
    return ret;
}
