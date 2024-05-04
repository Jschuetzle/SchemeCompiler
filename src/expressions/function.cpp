#include "function.h"

#include "ast.h"
#include "types/simple.h"
#include <llvm/IR/Verifier.h>

ASTFunction::ASTFunction(AST& ast, std::unique_ptr<VarType> returnType, ASTFunctionParameters params, 
std::unique_ptr<ASTExpression> definition) : ASTExpression(ast), definition(std::move(definition))
{

    // Create the function type.
    std::vector<std::unique_ptr<VarType>> paramTypes;
    for (auto& p : params) // We must copy each type from the parameters.
    {
        paramTypes.push_back(std::get<0>(p)->Copy());
    }
    funcType = std::make_unique<VarTypeFunction>(std::move(returnType), std::move(paramTypes));

    // This was here in PG4, but shouldn't be needed here because functions are anonymous
    /*
    if (!ast.scopeTable.AddVariable(name, funcType->Copy()))
    {
        throw std::runtime_error("ERROR: Function or global variable with name " + name + " already exists.");
    }
    */

    // Add parameters as stack variables. It's ok for us to do it since we are the ones setting up the parameters for stack variables.
    for (auto& p : params)
    {
        this->param_names.push_back(std::get<1>(p));
        AddStackVar(std::move(p));
    }

}

void ASTFunction::AddStackVar(ASTFunctionParameter var)
{

    // Add variable to the scope table and error if it already exists.
    if (!scopeTable.AddVariable(std::get<1>(var), std::move(std::get<0>(var))))
    {
        throw std::runtime_error("ERROR: Variable " + std::get<1>(var) + " is already defined in function!");
    }
    localVariables.push_back(std::get<1>(var));

}


/*
    I REALLY want to change this function so it will actually go through the entire ancestry
    of static scoping to find the variable. However time is a thing...

   This code just checks the local scope and then the global scope. In reality, if the variable doesn't exist
   in the local scope, then the scope where the function definition occurs is looked through (and so on and so forth) 
*/
VarType* ASTFunction::GetVariableType(const std::string& name)
{
    VarType* ret;
    if (ret = scopeTable.GetVariableType(name), !ret) // Continue only if function scope table doesn't have value.
    {
        if (ret = ast.scopeTable.GetVariableType(name), !ret) // Continue only if AST scope table doesn't have value.
        {
            throw std::runtime_error("ERROR: In function,  cannot resolve variable or function " + name + "!");
        }
        else return ret;
    }
    else return ret;
}

llvm::Value* ASTFunction::GetVariableValue(const std::string& name)
{
    llvm::Value* ret;
    if (ret = scopeTable.GetVariableValue(name), !ret) // Continue only if function scope table doesn't have value.
    {
        if (ret = ast.scopeTable.GetVariableValue(name), !ret) // Continue only if AST scope table doesn't have value.
        {
            throw std::runtime_error("ERROR: In function, cannot resolve variable or function " + name + "!");
        }
        else return ret;
    }
    else return ret;
}

void ASTFunction::SetVariableValue(const std::string& name, llvm::Value* value)
{
    if (!scopeTable.SetVariableValue(name, value)) // Continue only if function scope table doesn't have value.
    {
        if (!ast.scopeTable.SetVariableValue(name, value)) // Continue only if AST scope table doesn't have value.
        {
            throw std::runtime_error("ERROR: In function, cannot resolve variable or function " + name + "!");
        }
    }
}

void ASTFunction::Define(std::unique_ptr<ASTExpression> definition)
{
    if (!this->definition) // Define only if not already defined.
    {
        this->definition = std::move(definition);
    }
    else throw std::runtime_error("ERROR: Function already has a definition!");
}

llvm::Value* ASTFunction::Compile(llvm::Module& mod, llvm::IRBuilder<>& builder, ASTFunction* func){
    return nullptr;
}

llvm::Value* ASTFunction::Compile(std::string name, llvm::Module& mod, llvm::IRBuilder<>& builder, ASTFunction* func)
{

    // First, add a new function declaration to our scope.
    auto function = llvm::Function::Create((llvm::FunctionType*) funcType->GetLLVMType(builder.getContext()), llvm::GlobalValue::LinkageTypes::ExternalLinkage, name, mod);

    // Add function to scope table now that we pass in name
    if(!func)
        ast.scopeTable.AddVariable(name, funcType->Copy(), function);
    else
       func->scopeTable.AddVariable(name, funcType->Copy(), function);

    //name assigned for recursion purposes
    this->name = name;

    // Set parameter names.
    unsigned idx = 0;
    for (auto& arg : function->args()) arg.setName(param_names[idx++]);

    // Only continue if the function has a definition.
    if (!definition) return nullptr;

    // Create a new basic block to start insertion into.
    llvm::BasicBlock* funcEntry = llvm::BasicBlock::Create(builder.getContext(), "entry", function);

    // Save current insertion point
    llvm::IRBuilder<>::InsertPoint originalInsertPoint = builder.saveIP();
    builder.SetInsertPoint(funcEntry);

    /*
        So there's a lot going on here and it needs a bit of explanation.
        In LLVM, registers can only be assigned once which is not what we want for mutable variables.
        In order to combat this, we allocate memory on the stack to an LLVM register (llvm::Value*).
        This works, as while the memory location itself is constant, we can load and store to that stack location as much as we want.
        We can allocate stack memory with an "alloca" instruction.
        This must be done in the entry block for stack allocations to be automatically optimized to registers on the target machine where appropriate.
        Once we allocate memory on the stack for each stack variable, we must then store it to the scope table so we know where each variable "lives".
        Now that we did that, we can use our GetVariableValue and SetVariableValue functions to get the pointer to the variable, which we can load from or store to!
        Note that how the stored stack variables are pointers to a value is what makes us classify it as an L-Value!
    */

    
    for (auto& localVar : localVariables)
    {
        scopeTable.SetVariableValue(
            localVar,
            builder.CreateAlloca(scopeTable.GetVariableType(localVar)->GetLLVMType(builder.getContext()), nullptr, localVar)
        );
    }

    // Now we need to store the initial values of the function arguments into their stack equivalents.
    for (auto& arg : function->args())
    {
        builder.CreateStore(&arg, scopeTable.GetVariableValue(arg.getName().data())); // We are storing the argument into the pointer to the stack variable gotten by fetching it from the scope table.
    }

    

    // Check the function body to make sure it returns what we expect it to.
    std::unique_ptr<VarType> retType = definition->ReturnType(this);
    bool satisfiesType = false;

    if (retType) satisfiesType = retType->Equals(funcType->returnType.get()); // If we return something, make sure we return what is expected.
    if (!satisfiesType)
    {
        throw std::runtime_error("ERROR: Function has a return type mismatch error");
    }

    // Generate the function.
    ASTExpressionList* castedListExpr = dynamic_cast<ASTExpressionList*>(definition.get());
    llvm::Value* returnValue;
    if(castedListExpr){
        returnValue = castedListExpr->Compile("", mod, builder, func);
    } else {
        returnValue = definition->Compile(mod, builder, this);
    }
    

    // Add an implicit return void if necessary.
    if (!retType)
    {
        builder.CreateRetVoid();
    }

    builder.CreateRet(returnValue);  //need to put an llvm::Value* here
    
    // Verify and optimize the function.
    llvm::verifyFunction(*function, &llvm::errs());
    ast.fpm.run(*function);
    
    // Restore original insertion point
    builder.restoreIP(originalInsertPoint);

    return function;
}

std::unique_ptr<VarType> ASTFunction::ReturnType(ASTFunction* func){
    return funcType->Copy();
}

bool ASTFunction::IsLValue(ASTFunction* func){
    return false;
}


std::string ASTFunction::ToString(const std::string& prefix)
{
    std::string output = "lambda\n";
    output += prefix + "└──" + (definition == nullptr ? "nullptr\n" : definition->ToString(prefix + "   "));
    return output;
}
