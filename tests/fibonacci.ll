; ModuleID = 'TestMod'
source_filename = "TestMod"

declare i32 @puts(i8*)
declare i8* @itoa(i32, i8*)

define i32 @main() {
entry:
  %factResult = call i32 @fib(i32 4)
  %buf = alloca [20 x i8], align 1
  %bufptr = bitcast [20 x i8]* %buf to i8*
  %str = call i8* @itoa(i32 %factResult, i8* %bufptr)
  %putsResult = call i32 @puts(i8* %str)
  ret i32 0
}

define i32 @fib(i32 %n) {
entry:
  %0 = icmp eq i32 1, %n
  br i1 %0, label %contBlock, label %elseBlock

elseBlock:                                        ; preds = %entry
  %1 = sub i32 %n, 1
  %2 = call i32 @fib(i32 %1)
  %3 = sub i32 %n, 2
  %4 = call i32 @fib(i32 %3)
  %5 = add i32 %4, %2
  br label %contBlock

contBlock:                                        ; preds = %entry, %elseBlock
  %iftmp = phi i32 [ %5, %elseBlock ], [ 1, %entry ]
  ret i32 %iftmp
}
