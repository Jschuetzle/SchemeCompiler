 #include "and.h"

 #include "function.h"

class ASTFunction;

 std::unique_ptr<VarType> ASTExpressionAnd::ReturnType(ASTFunction* func)
 {
     return VarTypeSimple::BoolType.Copy(); 
 }

 bool ASTExpressionAnd::IsLValue(ASTFunction* func)
 {
     return false;
 }

 llvm::Value* ASTExpressionAnd::Compile(llvm::Module& mod, llvm::IRBuilder<>& builder, ASTFunction* func)
 {
     // Make sure to cast both sides as booleans first.
     //ASTExpression::ImplicitCast(func, a1, &VarTypeSimple::BoolType);
     //ASTExpression::ImplicitCast(func, a2, &VarTypeSimple::BoolType);

     // not implementing implicit casting, rather just check if the expressions have returnType boolean
     if(!(a1->ReturnType(func)->Equals(&VarTypeSimple::BoolType) && (a2->ReturnType(func)->Equals(&VarTypeSimple::BoolType)))){
         throw std::runtime_error("ERROR: Type mismatch on AND expression");
     }
 
     llvm::Function* funcVal = nullptr;
     if(func){
         funcVal = (llvm::Function*) func->GetVariableValue(func->name);   
     }    
     
     llvm::Value* left = a1->CompileRValue(mod, builder, func);
     llvm::Value* right = a1->CompileRValue(mod, builder, func);
     
     return builder.CreateAnd(left, right); 
}

 std::string ASTExpressionAnd::ToString(const std::string& prefix)
 {
     std::string ret = "and\n";
     ret += prefix + "├──" + a1->ToString(prefix + "│  ");
     ret += prefix + "└──" + a2->ToString(prefix + "   ");
     return ret;
 }
