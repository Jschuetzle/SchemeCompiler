; ModuleID = 'TestMod'
source_filename = "TestMod"

@a = constant i32 3
@b = constant double 7.000000e+00
@c = constant i32 3
@d = constant double %3
@e = constant i32 %6
@f = constant double %10
@g = constant double %12
@h = constant double %16
@i = constant i32 %18

define i32 @main() {
entry:
  %0 = load i32, i32 3, align 4
  %1 = sitofp i32 %0 to double
  %2 = load double, double 7.000000e+00, align 8
  %3 = fadd double %1, %2
  %4 = load i32, i32 3, align 4
  %5 = load i32, i32 3, align 4
  %6 = sub i32 %4, %5
  %7 = load double, double 7.000000e+00, align 8
  %8 = load i32, i32 3, align 4
  %9 = sitofp i32 %8 to double
  %10 = fmul double %7, %9
  %11 = load double, double 7.000000e+00, align 8
  %12 = fdiv double %11, 2.000000e+00
  %13 = load double, double 7.000000e+00, align 8
  %14 = load i32, i32 3, align 4
  %15 = sitofp i32 %14 to double
  %16 = frem double %13, %15
  %17 = load double, double 7.000000e+00, align 8
  %18 = mul double %17, i32 -1
  ret i32 1
}
