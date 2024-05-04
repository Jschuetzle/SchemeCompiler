#include "list.h"
#include <iostream>

ASTExpressionList::ASTExpressionList(AST& ast, std::unique_ptr<VarType> elementType, std::unique_ptr<ASTExpressionListNode> head) : ASTExpression(ast), head(std::move(head)){
    listType = std::make_unique<VarTypeList>(std::move(elementType));
}


std::unique_ptr<VarType> ASTExpressionList::ReturnType(ASTFunction* func)
{
    return listType->Copy();
}

bool ASTExpressionList::IsLValue(ASTFunction* func)
{
    //technically, an array/list can be an Lvalue because it can be on the left side of the assignment operator
    //however in our language, you can't directly manipulate what's in the list. You can only car, cdr, or cons to 
    //change the list. Therefore, it's an Rvalue
    return false;
}

llvm::Value* ASTExpressionList::Compile(std::string name, llvm::Module& mod,  llvm::IRBuilder<>& builder, ASTFunction* func) {
    return head->Compile(name, 0, listType->Copy(), mod, builder, func);
    
    /*
    llvm::Value* lastValue = nullptr;
    // Iterate through each node in the linked list
    while (currentNode) {
        if (currentNode->data) {
            // Compile the expression in the current node
            lastValue = currentNode->Compile(mod, builder, func);
            if (!lastValue) {
                throw std::runtime_error("Compilation error in expression list node.");
            }
        }
        // Move to the next node
        currentNode = currentNode->next.get();
    }

    // Return the value of the last expression evaluated, or nullptr if the list is empty
    // I need to see where this compile function is being used, because I DON'T THINK RETURNING THE LAST VALUE IS WHAT WE WANT
    return lastValue;
    */
}

llvm::Value* ASTExpressionList::Compile(llvm::Module& mod, llvm::IRBuilder<>& builder, ASTFunction* func){
    return nullptr;
}

std::string ASTExpressionList::ToString(const std::string& prefix) {
    std::string output = "list\n";
    /*
    ASTExpressionListNode* currentNode = head.get();
    if (currentNode != nullptr) {
        while (currentNode->next != nullptr) {
            if (currentNode->data) {
                output += prefix + "├──" + (currentNode->data == nullptr ? "nullptr\n" : currentNode->data->ToString(prefix + "│  "));
            } else {
                output += prefix + "├──nullptr\n";
            }
            currentNode = currentNode->next.get();
        }
        // Handle the last node separately to close the branch with a '└──'
        if (currentNode->data) {
            output += prefix + "└──" + (currentNode->data == nullptr ? "nullptr\n" : currentNode->data->ToString(prefix + "   "));
        } else {
            output += prefix + "└──nullptr\n";
        }
    }*/
    return output;
}
