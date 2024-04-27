#pragma once

#include "../varType.h"

// Type that represents a list
class VarTypeList : public VarType
{
private:

    // Prefetched LLVM type.
    llvm::Type* listType = nullptr;

public:

    // element type
    std::unique_ptr<VarType> elementType;

    // Create a list type.
    // elementType: What type the list holds
    VarTypeList(std::unique_ptr<VarType> elementType) : elementType(std::move(elementType)) {}

    // Virtual functions. See base class for details.
    virtual std::unique_ptr<VarType> Copy() override;
    virtual llvm::Type* GetLLVMType(llvm::LLVMContext& ctx) override;
    virtual bool Equals(VarType* other) override;

};
