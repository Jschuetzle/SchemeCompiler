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
    std::cout << name << std::endl;

    if(scopeTable.AddVariable(name, expr->ReturnType()->Copy()))
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
    }

    //Then, compile all the global expressions...expressions are nameless, so just put iter # inside string
    for (int i=0; i<globalExprs.size(); i++)
    {
        std::cout << "INFO: Compiling expression " + i << std::endl;
        globalExprs.at(i).get()->Compile(builder);
    }

    compiled = true;
}

std::string AST::ToString()
{
    std::string output = module.getModuleIdentifier() + "\n";
    for (int i = 0; i < globalVarList.size() - 1; i++)
        output += "├──" + globalVars[globalVarList.at(i)]->ToString("│  ");

    if(globalExprs.size() == 0){
        output += "└──" + globalVars[globalVarList.back()]->ToString("   "); 
    } else {
        output += "├──" + globalVars[globalVarList.back()]->ToString("|  ");
        for (int i=0; i<globalExprs.size() - 1; i++)
           output += "├──" + globalExprs.at(i).get()->ToString("|  ");

        output += "└──" + globalExprs.back().get()->ToString("   ");  
    }
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
