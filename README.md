# Scheme compiler
This project is the development of a compiler that translates Scheme, a functional programming language, into LLVM intermediate representation. The compiler focuses on a subset of the Scheme language, emphasizing immutability and static typing to enhance simplicity and performance.

## Running Tests
All tests can be run by running the `./executeTests.sh` script in the terminal.

## Test Cases Description
Each test case demonstrates specific functionalities of the compiler:
**completed tests**
* check.scm: Tests for correct type checking (`int?`, `real?`, `boolean?`).
* compOpsTest.scm: Validates basic comparison operations (`eq?`, `<`, `<=`, `>`, `>=`).
* constant-defs.scm: Ensures definitions (`define`) are processed correctly.
* funcall.scm: Tests lambda function calls, ensuring they work as expected.
* list.scm: Ensures lists can be created, and are implemented as linked lists.
* mathOpsTest.scm: Assesses basic mathematical operations (`remainder`, `+`, `-`, `*`, `/`).
* varTest.scm: Verifies variable handling.

**needs to be tested**
* basicRecursion.scm: Ensures our Scheme program is able to use basic recursion.
* listIter.scm: Basic function to iterrate through a list.
* localVsGlobal.scm: Ensures variables are scoped correctly, and local variables are not accessible outside of a function.

**needs to be verified**
* condTest.scm: Ensures conditional statements work as epxpected.
* recurse.scm: 

### Display a Control Flow graph of testMod.bc:
1. In the bin directory, run `opt -dot-cfg-only testMod.bc`. This will create a **.main.dot** file, which contains a text representation of testMod's CFG.
2. Run `dot .main.dot -Tsvg -o testMod.svg`. This allows graphviz to interpret **.main.dot** into an image file named **testMod.svg**, which can be opened via the browser.
