; ModuleID = 'TestMod'
source_filename = "TestMod"

define i32 @main() {
entry:
  ret i32 1
}

define i32 @fact(i32 %n) {
entry:
  %0 = sub i32 %n, 1
  %1 = call i32 @fact(i32 %0)
  ret i32 %1
}
