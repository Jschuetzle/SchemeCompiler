; ModuleID = 'TestMod'
source_filename = "TestMod"

@a = constant i1 true
@b = constant i1 false
@0 = private unnamed_addr constant [12 x i8] c"hello world\00", align 1
@c = constant i8* getelementptr inbounds ([12 x i8], [12 x i8]* @0, i32 0, i32 0)
@d = constant i32 43
@e = constant double 3.140000e+00
@f = constant i32 -2

define i32 @main() {
entry:
  ret i32 1
}
