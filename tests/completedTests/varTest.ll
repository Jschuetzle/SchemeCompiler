; ModuleID = 'TestMod'
source_filename = "TestMod"

@a = constant i32 5
@c = constant i32 5
@b = constant i32 %2

define i32 @main() {
entry:
  %0 = load i32, i32 5, align 4
  %1 = load i32, i32 5, align 4
  %2 = add i32 %0, %1
  ret i32 1
}
