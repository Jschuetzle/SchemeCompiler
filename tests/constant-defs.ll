; ModuleID = 'TestMod'
source_filename = "TestMod"

@b = constant i32 35
@f = constant double 3.140000e+00

define i32 @main() {
entry:
  %exprResult = alloca i32, align 4
  store i32 26, i32* %exprResult, align 4
  %loadedVal = load i32, i32* %exprResult, align 4
  ret i32 %loadedVal
}
