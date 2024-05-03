 #include "not.h"

 #include "function.h"

class ASTFunction;

 std::unique_ptr<VarType> ASTExpressionNot::ReturnType(ASTFunction* func)
 {
     return VarTypeSimple::BoolType.Copy(); 
 }

 bool ASTExpressionNot::IsLValue(ASTFunction* func)
 {
     return false; // not operator works on two R-Values to produce an R-Value.
 }

 llvm::Value* ASTExpressionNot::Compile(llvm::Module& mod, llvm::IRBuilder<>& builder, ASTFunction* func)
 {
     // Make sure to cast the operand as a boolean first.
     //ASTExpression::ImplicitCast(func, a1, &VarTypeSimple::BoolType);

     // not implementing implicit casting, rather just check if the expressions have returnType boolean
     if(!a1->ReturnType(func)->Equals(&VarTypeSimple::BoolType)){
         throw std::runtime_error("ERROR: Type mismatch on NOT expression");
     }
 
     llvm::Function* funcVal = nullptr;
     if(func){
         funcVal = (llvm::Function*) func->GetVariableValue(func->name);   
     }    
    
     llvm::Value* operand = a1->CompileRValue(mod, builder, func);
     return builder.CreateNot(operand);
}

 std::string ASTExpressionNot::ToString(const std::string& prefix)
 {
     std::string ret = "not\n";
     ret += prefix + "   └──" + a1->ToString(prefix + "   ");
     return ret;
 }
