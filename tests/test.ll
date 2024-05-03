; ModuleID = 'TestMod'
source_filename = "TestMod"

@0 = private unnamed_addr constant [12 x i8] c"hello world\00", align 1
@s = constant i8* getelementptr inbounds ([12 x i8], [12 x i8]* @0, i32 0, i32 0)

define i32 @main() {
entry:
}
