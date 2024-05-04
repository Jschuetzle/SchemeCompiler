; ModuleID = 'TestMod'
source_filename = "TestMod"

@s = constant i32 %0

define i32 @main() {
entry:
  %0 = call i32 @f(i32 5, i32 4)
  ret i32 1
}

define i32 @f(i32 %a, i32 %b) {
entry:
  %0 = add i32 %b, %a
  ret i32 %0
}
