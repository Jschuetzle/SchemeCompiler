; ModuleID = 'TestMod'
source_filename = "TestMod"

define i32 @main() {
entry:
  ret i32 1
}

define i32 @fact(i32 %n) {
entry:
  %0 = icmp eq i32 %n, 0
  br i1 %0, label %contBlock, label %elseBlock

elseBlock:                                        ; preds = %entry
  %1 = sub i32 %n, 1
  %2 = call i32 @fact(i32 %1)
  %3 = mul i32 %2, %n
  br label %contBlock

contBlock:                                        ; preds = %entry, %elseBlock
  %iftmp = phi i32 [ %3, %elseBlock ], [ 1, %entry ]
  ret i32 %iftmp
}
