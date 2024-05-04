#include "if.h"

#include "function.h"

std::unique_ptr<VarType> ASTExpressionIf::ReturnType(ASTFunction* func)
{
    // This is a bit of a strange case. We don't know for certain what the return type is unless both if and else return something.
    // In the case they both return something, have to make sure the return types match.

    // If we don't have an else statement, then we can't guarantee a return.
    if (!elseExpression) return nullptr;

    // Get return types. Return if either do not return anything.
    auto thenRet = thenExpression->ReturnType(func);
    auto elseRet = elseExpression->ReturnType(func);
    if (!thenRet || !elseRet) return nullptr;

    // Check for matching return types.
    if (thenRet->Equals(elseRet.get())) return std::move(thenRet); // Return if equal.
    else throw std::runtime_error("ERROR: If/Else statements both return a value but their return types don't match!");
}

bool ASTExpressionIf::IsLValue(ASTFunction* func)
{
    return false;
}

llvm::Value* ASTExpressionIf::Compile(llvm::Module& mod, llvm::IRBuilder<>& builder, ASTFunction* func)
{
    // Compile the condition. TODO: TO BOOLEAN CAST CONVERSION?
    if (!condition->ReturnType(func)->Equals(&VarTypeSimple::BoolType))
        throw std::runtime_error("ERROR: Expected condition that returns a boolean value but got another type instead!");
    llvm::Value* cond = condition->Compile(mod, builder, func);

    // Create blocks.
    llvm::BasicBlock* thenBlock = llvm::BasicBlock::Create(builder.getContext(), "thenBlock");
    llvm::BasicBlock* elseBlock = nullptr;
    if (elseExpression) elseBlock = llvm::BasicBlock::Create(builder.getContext(), "elseBlock");
    llvm::BasicBlock* contBlock = llvm::BasicBlock::Create(builder.getContext(), "contBlock");

    // Make jumps to blocks.
    builder.CreateCondBr(cond, thenBlock, elseBlock ? elseBlock : contBlock); // Use else as false if exists, otherwise go to continuation.

    // Compile the then block and then jump to continuation block.
    builder.SetInsertPoint(thenBlock);
    llvm::Value* thenVal = thenExpression->Compile(mod, builder, func);
    builder.CreateBr(contBlock); // Only create branch if no return encountered.

    // Compile the else block if applicable.
    llvm::Value* elseVal = llvm::Constant::getNullValue(thenVal->getType());
    if (elseBlock)
    {
        builder.SetInsertPoint(elseBlock);
        elseVal = elseExpression->Compile(mod, builder, func);
        builder.CreateBr(contBlock); // Only create branch if no return encountered.
    }

    builder.SetInsertPoint(contBlock);
    llvm::PHINode* phi = builder.CreatePHI(thenVal->getType(), 2, "iftmp");

    phi->addIncoming(thenVal, thenBlock);
    phi->addIncoming(elseVal, elseBlock);

    return phi;
}

std::string ASTExpressionIf::ToString(const std::string& prefix)
{
    std::string output = "if " + condition->ToString("") + "\n";
    output += prefix + (thenExpression ? "├──" : "└──") + thenExpression->ToString(prefix + "   ");
    if (elseExpression) output += prefix + (elseExpression ? "├──" : "└──") + elseExpression->ToString(prefix + "   ");
    return output;
}