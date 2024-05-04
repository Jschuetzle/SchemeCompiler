; ModuleID = 'TestMod'
source_filename = "TestMod"

@a = constant i32 5
@b = constant double 3.700000e+00
@c = constant i1 true
@d = constant i1 true
@e = constant i1 false
@f = constant i1 true
@g = constant i1 false
@h = constant i1 true
@i = constant i1 false
@j = constant i1 false

define i32 @main() {
entry:
  ret i32 1
}
