#include "ast.h"

#include <iostream>
#include <llvm/Bitcode/BitcodeWriter.h>
//#include "llvm/ADT/StringRef.h"

//#include <llvm/Transforms/InstCombine/InstCombine.h> // This causes an error on my machine.
#include <llvm/Transforms/Scalar.h>
#include <llvm/Transforms/Scalar/GVN.h>
#include <llvm/Transforms/Utils.h>

AST::AST(const std::string modName) : module(modName, context), builder(context), fpm(&module)
{

    // This requires the above includes that don't work on my machine, so I can't really add these default optimizations.

    // Promote allocas to registers.
    fpm.add(llvm::createPromoteMemoryToRegisterPass());

    // Do simple "peephole" optimizations and bit-twiddling optzns.
    //fpm.add(llvm::createInstructionCombiningPass());

    // Reassociate expressions.
    fpm.add(llvm::createReassociatePass());

    // Do simple "peephole" optimizations and bit-twiddling optzns.
    //fpm.add(llvm::createInstructionCombiningPass());

    // Reassociate expressions.
    fpm.add(llvm::createReassociatePass());

    // Eliminate Common SubExpressions.
    fpm.add(llvm::createGVNPass());

    // Simplify the control flow graph (deleting unreachable blocks, etc).
    fpm.add(llvm::createCFGSimplificationPass());

    // Finally initialize.
    fpm.doInitialization();

}


ASTExpression* AST::AddGlobalVariable(const std::string& name, std::unique_ptr<ASTExpression> expr)
{
    //this was previoulsy done in the function constructor, but now that we have global variables, we can place it here
    //since ASTExpression has a default constructor
    if(!scopeTable.AddVariable(name, expr->ReturnType()->Copy()))
        throw std::runtime_error("ERROR: Global Variable with name " + name + " already exists.");

    //update fields of ast class
    globalVarList.push_back(name);
    globalVars[name] = std::move(expr);

    return globalVars[name].get();
}


ASTExpression* AST::GetGlobalVariable(const std::string& name)
{
    auto found = globalVars.find(name);
    if (found != globalVars.end()) return found->second.get();
    else throw std::runtime_error("ERROR: Global variable " + name + " can't be found in the global binding map");
}

ASTExpression* AST::AddExpressionCall(std::unique_ptr<ASTExpression> expr)
{
    globalExprs.push_back(std::move(expr));
    return globalExprs.back().get();
}

void AST::Compile()
{
    std::vector<ASTFunctionParameter> emptyParams;
    auto* mainFunc = new ASTFunction(*this, VarTypeSimple::IntType.Copy(), std::move(emptyParams));
    auto func = llvm::Function::Create((llvm::FunctionType*) mainFunc->funcType->GetLLVMType(context), llvm::GlobalValue::LinkageTypes::ExternalLinkage, "main", module);

    llvm::BasicBlock* entry = llvm::BasicBlock::Create(context, "entry", func);
    builder.SetInsertPoint(entry);

    // First, compile each binding...const auto& on the string names because these shouldn't be modified
    for (const auto& varName : globalVarList)
    {
        std::cout << "INFO: Compiling binding " + varName + "." << std::endl;

        //compile the expression to obtain the value
        std::unique_ptr<VarType> type;

        //check if the object being returned is a function, and if so get the function type instead of the return type
        ASTFunction* castedFuncExpr = dynamic_cast<ASTFunction*>(globalVars[varName].get());
        ASTExpressionList* castedListExpr = dynamic_cast<ASTExpressionList*>(globalVars[varName].get());

        if(!castedFuncExpr){
            if(!castedListExpr){
                type = globalVars[varName]->ReturnType()->Copy();  
            } else {
                type = castedListExpr->listType->Copy();
            }
        } else {
            type = castedFuncExpr->funcType->Copy();
        }

        llvm::Type* llvmtype = type->GetLLVMType(context);

        //two cases for dealing with global constants and functions
        VarTypeFunction* castedFuncType = dynamic_cast<VarTypeFunction*>(type.get());
        VarTypeList* castedListType = dynamic_cast<VarTypeList*>(type.get());

        if(!castedFuncType){  
            if(!castedListType){
                //create an llvm::GlobalVariable* that is added to the module
                auto* value = globalVars[varName]->Compile(module, builder);
                auto* gVar = new llvm::GlobalVariable(module, llvmtype, true, llvm::GlobalValue::ExternalLinkage, (llvm::Constant*) value, varName);
                module.getOrInsertGlobal(varName, llvmtype);
                scopeTable.SetVariableValue(varName, value);
            } else {
                auto* listValue = castedListExpr->Compile(varName, module, builder, nullptr);
                scopeTable.SetVariableValue(varName, listValue);
            }
        } else {

            auto* funcValue = castedFuncExpr->Compile(varName, module, builder, castedFuncExpr);
            //scopeTable.SetVariableValue(varName, funcValue);
        }
    }

    //Then, compile all the global expressions...expressions are nameless, so just put iter # inside string

    // std::vector<ASTFunctionParameter> emptyParams;
    // // Create the 'main' function and its entry point
    // auto* mainFunc = new ASTFunction(*this, VarTypeSimple::IntType.Copy(), std::move(emptyParams));

    // auto func = llvm::Function::Create((llvm::FunctionType*) mainFunc->funcType->GetLLVMType(context), llvm::GlobalValue::LinkageTypes::ExternalLinkage, "main", module);

    // llvm::BasicBlock* entry = llvm::BasicBlock::Create(context, "entry", func);
    // builder.SetInsertPoint(entry);

    // //need to create a stack variable to store our expression result into, otherwise the code will get optimized away.
    // if(globalExprs.size() != 0){
    //     ASTExpression* e = globalExprs.at(0).get();
    //     llvm::Value* val = e->Compile(builder, mainFunc);
    
    //     //need to create a stack variable to store our expression result into, otherwise the code will get optimized away. 
    //     llvm::AllocaInst *exprResultAlloc;

    //     if(e->ReturnType(mainFunc)->Copy()->Equals(&VarTypeSimple::IntType)){

    //         exprResultAlloc = builder.CreateAlloca(llvm::Type::getInt32Ty(context), nullptr, "exprResult"); 
    //         builder.CreateStore(val, exprResultAlloc);
            
    //         // Load the value from the stack
    //         llvm::Value* loadedVal = builder.CreateLoad(exprResultAlloc->getAllocatedType(), exprResultAlloc, "loadedVal");

    //         // Return the loaded value
    //         builder.CreateRet(loadedVal);

    //     } else if (e->ReturnType(mainFunc)->Copy()->Equals(&VarTypeSimple::RealType)){
    //         exprResultAlloc = builder.CreateAlloca(llvm::Type::getDoubleTy(context), nullptr, "exprResult"); 
    //         builder.CreateStore(val, exprResultAlloc);
    //         // Load the double value from the stack
    //         llvm::Value* loadedDouble = builder.CreateLoad(exprResultAlloc->getAllocatedType(), exprResultAlloc, "loadedDouble");
            
    //         // Cast the loaded double to an integer
    //         llvm::Value* doubleToInt = builder.CreateFPToSI(loadedDouble, llvm::Type::getInt32Ty(context), "doubleToInt");
            
    //         // Return the casted integer value
    //         builder.CreateRet(doubleToInt);

    //     } else {
    //         exprResultAlloc = builder.CreateAlloca(llvm::Type::getInt1Ty(context), nullptr, "exprResult");  
    //         builder.CreateStore(val, exprResultAlloc);

    //         // Load the boolean value from the stack
    //         llvm::Value* loadedBool = builder.CreateLoad(exprResultAlloc->getAllocatedType(), exprResultAlloc, "loadedBool");
            
    //         // Cast the loaded boolean to an integer (ZExt - Zero Extension is typically used for boolean to integer)
    //         llvm::Value* boolToInt = builder.CreateZExt(loadedBool, llvm::Type::getInt32Ty(context), "boolToInt");
            
    //         // Return the casted integer value
    //         builder.CreateRet(boolToInt);
    //     } 
    // }
    
    // Create return instruction and insert it into the end of the entry block
    llvm::Value *returnValue = llvm::ConstantInt::get(context, llvm::APInt(32, 1, true));
    builder.CreateRet(returnValue);

    compiled = true;
}

std::string AST::ToString()
{ 
    std::string output = module.getModuleIdentifier() + "\n";

    std::string binding_name;

    /* go through bindings (if any) and print each of them in the form
        binding
            --> name
            --> compiled expression
    */

    if(globalVarList.size() > 0){
        for (int i = 0; i < globalVarList.size() - 1; i++){ 
            binding_name = globalVarList.at(i);
            output += AST::bindingString(binding_name, std::move(globalVars[binding_name]), false);
        }
   
        //print out the final binding differently if there are no expressions following it
        binding_name = globalVarList.at(globalVarList.size()-1); 

        if(globalExprs.size() == 0){

            output += AST::bindingString(binding_name, std::move(globalVars[binding_name]), true);
        }
    }

    if(globalExprs.size() > 0){ 
        for (int i=0; i<globalExprs.size() - 1; i++){
           output += "├──return\n   ├──" + globalExprs.at(i).get()->ToString("      ");
        }

        output += "└──return\n   └──" + globalExprs.back().get()->ToString("      ");  
    }
    return output;
}


// prints out the binding string for some name and expression. "end" specifies whether ├── or └── is used
std::string AST::bindingString(std::string name, std::unique_ptr<ASTExpression> expr, bool end){ 
    std::string output = end ? "└──binding\n" : "├──binding\n";
    output += "|  ├──" + name + "\n|  └──" + expr->ToString("      ");
    return output; 
}


void AST::WriteLLVMAssemblyToFile(const std::string& outFile)
{
    if (!compiled) throw std::runtime_error("ERROR: Module " + std::string(module.getName().data()) + " not compiled!");
    if (outFile == "") throw std::runtime_error("ERROR: Writing assembly to standard out is not supported!");
    std::error_code err;
 
    llvm::raw_fd_ostream outLl(outFile, err);
    module.print(outLl, nullptr);
    outLl.close();
}

void AST::WriteLLVMBitcodeToFile(const std::string& outFile)
{
    if (!compiled) throw std::runtime_error("ERROR: Module " + std::string(module.getName().data()) + " not compiled!");
    if (outFile == "") throw std::runtime_error("ERROR: Writing bitcode to standard out is not supported!");
    std::error_code err;
    llvm::raw_fd_ostream outBc(outFile, err);
    llvm::WriteBitcodeToFile(module, outBc);
    outBc.close();
}
