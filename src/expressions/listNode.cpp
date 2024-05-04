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

llvm::Value* ASTExpressionListNode::Compile(std::string name, int num, std::unique_ptr<VarType> expectedType, llvm::Module& mod, llvm::IRBuilder<>& builder, ASTFunction* func){
    /*
        Lists that are defined globally have to be compiled a bit differently than normal global variables.
        Therefore, there must be two cases in this compile method
            1. The list is being compiled globally
            2. The list is being compiled locally
        The scope of the list is determined by whether "func" is a nullptr or not
    */

    VarTypeList* listType = dynamic_cast<VarTypeList*>(expectedType.get());

    if(!data->ReturnType(func)->Equals(listType->elementType.get())){
        throw std::runtime_error("List element does not match expected type");
    }

    llvm::Type* listNodeType = expectedType->GetLLVMType(builder.getContext());
    auto* compData = data->Compile(mod, builder, func);
    llvm::Value* ptrData;
    llvm::Value* nextPtr;

    if(next){
        ptrData = next->Compile(name, num+1, std::move(expectedType), mod, builder, func);
        nextPtr = builder.CreatePointerCast(ptrData, llvm::PointerType::getUnqual(listNodeType));
    } else {
        nextPtr = llvm::ConstantPointerNull::get(llvm::PointerType::getUnqual(listNodeType));
    }

    std::vector<llvm::Constant*> values;
    values.push_back((llvm::Constant*) compData);
    values.push_back((llvm::Constant*) nextPtr);
    llvm::Constant* structInit = llvm::ConstantStruct::get((llvm::StructType*) listNodeType, values);

    //need to decide if the list should be traversed here or in the parent list compilation
    // if(next){s
    //     next->Compile
    // }

    std::string newName;
    if(num == 0){
        newName = name;
    } else {
        newName = name + std::to_string(num);
    }
    
    llvm::Value* var;
    if(func){
        llvm::Value* localStruct = builder.CreateAlloca(listNodeType, nullptr, newName);
        var = builder.CreateStore(structInit, localStruct);
    } else {
        var = new llvm::GlobalVariable(mod, listNodeType, true, llvm::GlobalValue::ExternalLinkage, structInit, newName);
        mod.getOrInsertGlobal(newName, listNodeType);
    }
    
    return var;
}

llvm::Value* ASTExpressionListNode::Compile(llvm::Module& mod, llvm::IRBuilder<>& builder, ASTFunction* func){
    return nullptr;
}

std::string ASTExpressionListNode::ToString(const std::string& prefix)
{
    return data->ToString(prefix + "|  "); 
}
