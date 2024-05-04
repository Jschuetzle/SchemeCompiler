#include "check.h"

std::unique_ptr<VarType> ASTExpressionCheck::ReturnType(ASTFunction* func)
{
    return VarTypeSimple::BoolType.Copy();
}

bool ASTExpressionCheck::IsLValue(ASTFunction* func)
{
    return false;
}

llvm::Value* ASTExpressionCheck::Compile(llvm::Module& mod, llvm::IRBuilder<>& builder, ASTFunction* func)
{
    llvm::Type* checkType;
    llvm::Type* operandType = operand->Compile(mod, builder, func)->getType();

    switch (type)
    {
        // both of the below are list types. might have to handle differently, but not sure yet
        // case Null: break; // check if list is empty
        // case List: break;
        case BoolType: checkType = VarTypeSimple::BoolType.GetLLVMType(builder.getContext()); break;
        case IntType: checkType = VarTypeSimple::IntType.GetLLVMType(builder.getContext()); break;
        case RealType: checkType = VarTypeSimple::RealType.GetLLVMType(builder.getContext()); break;
        default: throw new std::runtime_error("ERROR: Invalid check type. How did this happen.");
    }

    return llvm::ConstantInt::get(VarTypeSimple::BoolType.GetLLVMType(builder.getContext()), checkType == operandType);
}

std::string ASTExpressionCheck::ToString(const std::string& prefix)
{
    std::string op = "";
    switch (type)
    {
        case NullType: op = "null?"; break;
        case BoolType: op = "boolean?"; break;
        case IntType: op = "int?"; break;
        case RealType: op = "real?"; break;
        case ListType: op = "list?"; break;
    }

    std::string ret = "(" + op + ")\n";
    ret += prefix + "└──" + operand->ToString(prefix + "   ");
    return ret;
}