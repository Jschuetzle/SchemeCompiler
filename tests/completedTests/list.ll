; ModuleID = 'TestMod'
source_filename = "TestMod"

@l2 = constant { i32, ptr } { i32 7, { i32, ptr }* null }
@l1 = constant { i32, ptr } { i32 6, { i32, ptr }* @l2 }
@l = constant { i32, ptr } { i32 8, { i32, ptr }* @l1 }

define i32 @main() {
entry:
  ret i32 1
}
