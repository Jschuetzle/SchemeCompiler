#include "function.h"

#include <llvm/IR/DerivedTypes.h>

std::unique_ptr<VarType> VarTypeFunction::Copy()
{
    // Copy each of the parameter types.
    std::vector<std::unique_ptr<VarType>> newTypes;
    for (auto& type : parameterTypes)
    {
        newTypes.push_back(type->Copy());
    }

    return std::make_unique<VarTypeFunction>(returnType->Copy(), std::move(newTypes));
}

llvm::Type* VarTypeFunction::GetLLVMType(llvm::LLVMContext& ctx)
{
    //if not done already, fetch the LLVM type
    if (!funcType) 
    {
        // Gather LLVM types of all parameters.
        std::vector<llvm::Type*> params;
        for (auto& param : parameterTypes) 
        {
            params.push_back(param->GetLLVMType(ctx));
        }
        
        //create function type with llvm
        funcType = llvm::FunctionType::get(returnType->GetLLVMType(ctx), params, false);
    }
    return funcType;
}

bool VarTypeFunction::Equals(VarType* other)
{
    VarTypeFunction* casted = dynamic_cast<VarTypeFunction*>(other);

    // This is null if the cast fails (meaning it's not really a function type)
    if (casted) 
    {
        if (!returnType->Equals(casted->returnType.get())) return false;

        if (parameterTypes.size() != casted->parameterTypes.size()) return false;
        for (int i = 0; i < parameterTypes.size(); i++) // Compare each parameter to see if it matches.
        {
            if (!parameterTypes[i]->Equals(casted->parameterTypes[i].get())) return false;
        }

        return true;
    }
    else return false;
}
