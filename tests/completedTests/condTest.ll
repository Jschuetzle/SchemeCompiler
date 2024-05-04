; ModuleID = 'TestMod'
source_filename = "TestMod"

@b = constant i32 %iftmp

define i32 @main() {
entry:
  br i1 false, label %thenBlock, label %elseBlock

thenBlock:                                        ; preds = %entry
  br label %contBlock

elseBlock:                                        ; preds = %entry
  br label %contBlock

contBlock:                                        ; preds = %elseBlock, %thenBlock
  %iftmp = phi i32 [ 14, %thenBlock ], [ 49, %elseBlock ]
  ret i32 1
}
