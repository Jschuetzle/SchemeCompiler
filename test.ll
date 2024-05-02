; ModuleID = 'TestMod'
source_filename = "TestMod"

@a = constant i1 true
@b = constant i1 false
@c = constant i32 4
@h = constant i32 -8
@f = constant double 3.140000e+00

@str = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 10)
  ret i32 10
}
