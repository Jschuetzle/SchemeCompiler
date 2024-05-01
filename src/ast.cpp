#include "ast.h"

#include <iostream>
#include <llvm/Bitcode/BitcodeWriter.h>

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
    if(!scopeTable.AddVariable(name, expr->ReturnType()->Copy()))
        throw std::runtime_error("ERROR: Global Variable with name " + name + " already exists.");
    globalVarList.push_back(name);
    globalVars[name] = std::move(expr);
    return globalVars[name].get();
}


ASTExpression* AST::GetGlobalVariable(const std::string& name)
{
    auto found = globalVars.find(name);
    if (found != globalVars.end()) return found->second.get();
    else throw std::runtime_error("ERROR: Global variable " + name + " can't be found in the ast binding map");
}

ASTExpression* AST::AddExpressionCall(std::unique_ptr<ASTExpression> expr)
{
    globalExprs.push_back(std::move(expr));
    return globalExprs.back().get();
}

void AST::Compile()
{

    // First, compile each binding
    for (auto& varName : globalVarList)
    {
        std::cout << "INFO: Compiling binding " + varName + "." << std::endl;
        globalVars[varName]->Compile(builder);
        std::unique_ptr<VarType> type = globalVars[varName]->ReturnType()->Copy();
        llvm::Type* llvmtype = type->GetLLVMType(context); 
       
        //need to add functionality for adding functions that are bound in define statements 
        //only do this if the type is int, real, bool, char

        AST::CreateGlobalConstant(varName, std::move(type), llvmtype);
    }
    //Then, compile all the global expressions...expressions are nameless, so just put iter # inside string
    for (int i=0; i<globalExprs.size(); i++)
    {
        std::cout << "INFO: Compiling expression " + i << std::endl;
        globalExprs.at(i).get()->Compile(builder);
    }

    compiled = true;
}

//creates an inserts a global variable into the llvm module
llvm::GlobalVariable* AST::CreateGlobalConstant(std::string name, std::unique_ptr<VarType> type, llvm::Type* llvmtype) {
    
         
    //takes in var name and var type
    module.getOrInsertGlobal(name, llvmtype);

    llvm::GlobalVariable* gVar = module.getNamedGlobal(name);

    //used to be CommonLinkage...just a note here if anything messes up
    gVar->setLinkage(llvm::GlobalValue::ExternalLinkage);

    //set the alignment of the global based on the VarType
    if(type->Equals(&VarTypeSimple::IntType) || type->Equals(&VarTypeSimple::RealType))
        gVar->setAlignment(llvm::MaybeAlign(4));
    else if(type->Equals(&VarTypeSimple::BoolType))
        gVar->setAlignment(llvm::MaybeAlign(1));

    return gVar;
}

std::string AST::ToString()
{ 
    //print out all defintions 
    std::string output = module.getModuleIdentifier() + "\n";

    std::string binding_name;
    if(globalVarList.size() > 0){
        for (int i = 0; i < globalVarList.size() - 1; i++){ 
            binding_name = globalVarList.at(i);
            output += AST::bindingString(binding_name, std::move(globalVars[binding_name]), false);
        }
   
        //print out the final binding differently if there are no expressions following it
        binding_name = globalVarList.at(globalVarList.size()-1); 
        std::cout << "binding name " << binding_name << std::endl;

        if(!globalVars[binding_name]){
                std::cout << "type bound to is null" << std::endl;
        }
        if(globalExprs.size() == 0){

            output += AST::bindingString(binding_name, std::move(globalVars[binding_name]), true);
        }
    }

    std::cout << "printing exprs"  << std::endl;

    if(globalExprs.size() > 0){ 
        for (int i=0; i<globalExprs.size() - 1; i++){
           output += "├──return\n   ├──" + globalExprs.at(i).get()->ToString("|  ");
        }

        output += "└──return\n   └──" + globalExprs.back().get()->ToString("   ");  
    }
    return output;
}

std::string AST::bindingString(std::string name, std::unique_ptr<ASTExpression> expr, bool end){ 
    std::string output = end ? "└───binding\n" : "├──binding\n";
    output += "   ├──" + name + "\n   └──" + expr->ToString("│  ");
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
