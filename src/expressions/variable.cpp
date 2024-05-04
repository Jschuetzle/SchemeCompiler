#include "variable.h"
#include "function.h"
#include "../ast.h"

//note that the 'func' param has default value nullptr. This is because of the default value that is provided in the virtual method in the ASTExpression class
std::unique_ptr<VarType> ASTExpressionVariable::ReturnType(ASTFunction* func)
{
    //func will be null if the variable resides in the global scope
    if(!func)
       return ast.scopeTable.GetVariableType(var)->Copy();
    else
        return func->GetVariableType(var)->Copy(); // We just need to resolve the variable and copy its type.
}


//currently, I don't where this function is used and how to implement it
bool ASTExpressionVariable::IsLValue(ASTFunction* func)
{
    auto retType = ReturnType(func);
    return !dynamic_cast<VarTypeFunction*>(retType.get());
    // If the variable is a function type, then we shouldn't load from it, it's just a raw function address.
    // Otherwise, we know that the variable is really just a pointer to some memory allocated somewhere and is thus an L-Value.
}


llvm::Value* ASTExpressionVariable::Compile(llvm::Module& mod, llvm::IRBuilder<>& builder, ASTFunction* func)
{
    if(!func)
        return ast.scopeTable.GetVariableValue(var);
    else if (var == func->name)
        return mod.getFunction(var);
    else
       return func->GetVariableValue(var);
}

std::string ASTExpressionVariable::ToString(const std::string& prefix)
{
    return var + "\n";
}
