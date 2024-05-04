#include "list.h"
#include <iostream>

#include <llvm/IR/DerivedTypes.h>

std::unique_ptr<VarType> VarTypeList::Copy()
{
    return std::make_unique<VarTypeList>(elementType->Copy());
}

//create of llvm type is a little shady to me...
llvm::Type* VarTypeList::GetLLVMType(llvm::LLVMContext& ctx)
{
    //if llvm defined listType is null, then define it
    if (!listType)
    {   
        llvm::Type* data = elementType->GetLLVMType(ctx);
        llvm::PointerType* next = llvm::PointerType::getUnqual(ctx);
        listType = llvm::StructType::get(ctx, {data, next}, false);
    }
    return listType;
}

bool VarTypeList::Equals(VarType* other)
{
    VarTypeList* casted = dynamic_cast<VarTypeList*>(other);

    // This is null if the cast fails (meaning it's not really a function type).
    if (casted)
    {
        return elementType->Equals(casted->elementType.get());
    }
    else return false;
}
