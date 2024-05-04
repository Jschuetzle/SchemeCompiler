# This will run our compiler on a C file, export its LLVM ASM to a .ll file and output how it went to the log.
# The LLVM ASM is than ran and its outut sent to the execution log.
function runTest {
        ./run.sh -i tests/$1.scm -fAsm -o tests/$1.ll &> tests/$1.log
        /usr/bin/time -o tests/$1Time.log -f "%E real,%U user,%S sys" lli tests/$1.ll &> tests/$1Execution.log
}

# Run all the C file tests.
for file in "tests"/*.scm; do
        file=$(basename $file .scm)
        echo "Testing: $file"
        runTest $file
done
