#include "addition.h"
#include <iostream>

std::unique_ptr<VarType> ASTExpressionAddition::ReturnType(ASTFunction* func)
{
    if (!returnType) // If the return type has not been gotten yet.
    {
        if (!(ASTExpression::CoerceMathTypes(ast, func, a1, a2, returnType)))
            throw std::runtime_error("ERROR: Can not coerce types in addition expression! Are they both either ints or reals?");
    }
    return std::make_unique<VarTypeSimple>(*returnType); // Make a copy of our return type :}
}

bool ASTExpressionAddition::IsLValue(ASTFunction* func)
{
    return false; // If we are adding values together, they must be usable R-Values. Adding these together just results in an R-Value.
}

llvm::Value* ASTExpressionAddition::Compile(llvm::Module& mod, llvm::IRBuilder<>& builder, ASTFunction* func)
{
    /*
       The type of the operands in the addition decide what value will be returned.
       We have to do some type checks here...are we allowing for type coercion? For simplicity, I don't want to. But if it works then sure. So we would first need a type check here.

       Also, the return type of the input function should not be apart of the conditional...it should be the return type of the operands that is checked in the conditional.
    */
    auto retType = ReturnType(func);

    if (retType.get()->Equals(&VarTypeSimple::IntType)){
        return builder.CreateAdd(a1->CompileRValue(mod, builder, func), a2->CompileRValue(mod, builder, func));
    }
    else if (retType.get()->Equals(&VarTypeSimple::RealType)){
        std::cout << "real" << std::endl;
        return builder.CreateFAdd(a1->CompileRValue(mod, builder, func), a2->CompileRValue(mod, builder, func));
    }
    else {
        throw std::runtime_error("ERROR: Can not perform addition! Are both inputs either ints or floats?"); 
    }
}

std::string ASTExpressionAddition::ToString(const std::string& prefix)
{
    std::string ret = "(+)\n";
    ret += prefix + "├──" + a1->ToString(prefix + "│  ");
    ret += prefix + "└──" + a2->ToString(prefix + "   ");
    return ret;
}
