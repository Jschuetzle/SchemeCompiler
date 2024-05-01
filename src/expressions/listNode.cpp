#include "listNode.h"

std::unique_ptr<VarType> ASTExpressionListNode::ReturnType(ASTFunction* func)
{
    //returns the type of the data contained in the list node
    return data->ReturnType()->Copy();
}

bool ASTExpressionListNode::IsLValue(ASTFunction* func)
{
    //technically, an array/list can be an Lvalue because it can be on the left side of the assignment operator
    //however in our language, you can't directly manipulate what's in the list. You can only car, cdr, or cons to 
    //change the list. Therefore, it's an Rvalue
    return false;
}

llvm::Value* ASTExpressionListNode::Compile(llvm::IRBuilder<>& builder, ASTFunction* func)
{
   //there is a possibility that I have to do type checking here
   //but I think I'm going to handle that in the parser
   return data->Compile(builder, func); 
}

std::string ASTExpressionListNode::ToString(const std::string& prefix)
{
    return data->ToString(prefix + "|  "); 
}
