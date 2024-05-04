; ModuleID = 'TestMod'
source_filename = "TestMod"

@a = constant i32 8
@b = constant double 5.000000e+00
@c = constant i1 false
@d = constant i1 false
@e = constant i1 true
@f = constant i1 %3
@g = constant i1 %7
@h = constant i1 %10
@i = constant i1 %13

define i32 @main() {
entry:
  %0 = load i32, i32 8, align 4
  %1 = sitofp i32 %0 to double
  %2 = load double, double 5.000000e+00, align 8
  %3 = fcmp olt double %1, %2
  %4 = load double, double 5.000000e+00, align 8
  %5 = load i32, i32 8, align 4
  %6 = sitofp i32 %5 to double
  %7 = fcmp ole double %4, %6
  %8 = load double, double 5.000000e+00, align 8
  %9 = load double, double 5.000000e+00, align 8
  %10 = fcmp ogt double %8, %9
  %11 = load double, double 5.000000e+00, align 8
  %12 = load double, double 5.000000e+00, align 8
  %13 = fcmp oge double %11, %12
  ret i32 1
}
