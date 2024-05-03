%code requires {
	#include <stdio.h>
	#include <stdlib.h>
	#include <vector>
	#include <string>
	#include <cstring>
	#include <variant>
	#include <iostream>
	//all of these includes are done as relative paths starting from the build/ directory, since that's where CMake places parser.tab.cc
	#include "../src/ast.h"
	//#include "../src/expressions/call.h"
	#include "../src/expressions/int.h"
	#include "../src/expressions/real.h"
    #include "../src/expressions/bool.h"
	#include "../src/expressions/string.h"
    #include "../src/expressions/list.h"
    #include "../src/expressions/listNode.h"
	#include "../src/expressions/variable.h"
	#include "../src/expressions/addition.h"
	#include "../src/expressions/sub.h"
	#include "../src/expressions/multiplication.h"
	#include "../src/expressions/division.h"
    #include "../src/expressions/remainder.h"
    //#include "../src/expressions/negative.h"
	#include "../src/expressions/comparison.h"
	#include "../src/expressions/and.h"
	#include "../src/expressions/or.h"
    #include "../src/expressions/not.h"
	//#include "../src/expressions/if.h"
	#include "../src/types/simple.h"
    #include "../src/types/list.h"
	extern FILE *yyin; 
}

%{
#include "parser.tab.hh"

  extern int yylex(void);
  void yyerror(const char *s);
  void save_to_dot(FILE *);
  int trav_and_write(FILE *, node *);

  AST ast("TestMod");
%}

%start program

%define parse.error verbose

 /* You'll notice that the union has many more types than previously. Read over it to make sure you know what everything does.
  * In particular, node that we do not store objects (or structs) in the union. Instead, it is better practice to store pointers. */
%union {
  bool boolval;
  int intval;
  double fltval;
  char *strval;
  struct node *nodeval;
  ASTExpression *exp;
  ASTExpressionListNode *exprVec;
  VarType *type;
  ASTExpressionComparisonType rel;
}

%token LPAREN RPAREN INT_TYPE REAL_TYPE STRING_TYPE BOOL_TYPE LIST_TYPE APOSTROPHE BOOL_LITERAL INT_LITERAL REAL_LITERAL ID RELOP_GT RELOP_LT RELOP_GE RELOP_LE RELOP_EQ ARITH_PLUS ARITH_MINUS ARITH_MULT ARITH_DIV ARITH_REMAINDER NULL_CHECK_OP BOOL_CHECK_OP NUMBER_CHECK_OP REAL_CHECK_OP LIST_CHECK_OP LOGICAL_OR LOGICAL_AND LOGICAL_NOT DEFINE LET CAR CDR CONS LAMBDA COND IF ELSE STRING_LITERAL 

%type <boolval> BOOL_LITERAL
%type <strval> ID STRING_LITERAL
%type <intval> intLit INT_LITERAL
%type <fltval> realLit REAL_LITERAL
%type <exp> expr datum
%type <exprVec> datumList
%type <type> type
%type <rel> relop

%expect 2 // Shift/reduce conflict when resolving two versions of if statement

%%

/*
Originally, I wasn't going to force definitions to come first, i.e. expressions could be called whenever.
However, this makes the AST.Compile function difficult. Based on the fields stored in the AST class, we would
either have to combine the fields in a clever way to ensure the correct compilation order, or we would have to 
treat everything as an expression (which is not desired since we want a distinction between define and let).

Therefore, the grammar must enforce that all binding syntax comes before expressions
*/
program: globalDefList expr {
    auto e = ast.AddExpressionCall(std::unique_ptr<ASTExpression>($2));       
};

globalDefList: | globalDefList LPAREN DEFINE ID expr RPAREN {
    //this will essentially just add the name and the expr to the varList and varMap fields in AST
    //we don't need to worry about adding to scopeTable just yet...that will happen when we compile expressions in AST.compile 
    auto v = ast.AddGlobalVariable($4, std::unique_ptr<ASTExpression>($5));     
} ;


type: BOOL_TYPE {
  $$ = new VarTypeSimple(VarTypeSimple::BoolType);
}| INT_TYPE {
  $$ = new VarTypeSimple(VarTypeSimple::IntType);
}| REAL_TYPE {
  $$ = new VarTypeSimple(VarTypeSimple::RealType);
}| STRING_TYPE {
  $$ = new VarTypeSimple(VarTypeSimple::StringType);
} | LIST_TYPE RELOP_LT type RELOP_GT {
  //our grammar technically allows for the storage of variable sized lists in lists, as long as the element types are consistent
  $$ = new VarTypeList(std::unique_ptr<VarType>($3));
};

// productions for all possible expressions
expr: datum 
    // lambda expressions
    | LPAREN LAMBDA type LPAREN paramList RPAREN expr RPAREN {
        $$ = $7;
    }
    | LPAREN LET LPAREN bindList bind RPAREN expr RPAREN {
        
    }
    // boolean operations
    | LPAREN LOGICAL_AND expr expr RPAREN {
        $$ = new ASTExpressionAnd(ast, std::unique_ptr<ASTExpression>($3), std::unique_ptr<ASTExpression>($4));
    } 

    | LPAREN LOGICAL_OR expr expr RPAREN {
        $$ = new ASTExpressionOr(ast, std::unique_ptr<ASTExpression>($3), std::unique_ptr<ASTExpression>($4));
    }

    | LPAREN LOGICAL_NOT expr RPAREN {
        $$ = new ASTExpressionNot(ast, std::unique_ptr<ASTExpression>($3));
    }



    /*
    | LPAREN ID exprList RPAREN  ; function calls
    | LPAREN LET LPAREN bindList bind RPAREN expr RPAREN   ; local variable bindings
    | LPAREN IF expr expr expr RPAREN 
    | LPAREN IF expr expr RPAREN  
    | LPAREN COND caseList LPAREN ELSE expr RPAREN RPAREN
    */    
    // math operations
    | LPAREN ARITH_PLUS expr expr RPAREN {
      $$ = new ASTExpressionAddition(ast, std::unique_ptr<ASTExpression>($3), std::unique_ptr<ASTExpression>($4));
    }

    | LPAREN ARITH_MINUS expr expr RPAREN {
      $$ = new ASTExpressionSubtraction(ast, std::unique_ptr<ASTExpression>($3), std::unique_ptr<ASTExpression>($4));
    }

    | LPAREN ARITH_MULT expr expr RPAREN {
      $$ = new ASTExpressionMultiplication(ast, std::unique_ptr<ASTExpression>($3), std::unique_ptr<ASTExpression>($4));
    }

    | LPAREN ARITH_DIV expr expr RPAREN {
      $$ = new ASTExpressionDivision(ast, std::unique_ptr<ASTExpression>($3), std::unique_ptr<ASTExpression>($4));
    }

    | LPAREN ARITH_REMAINDER expr expr RPAREN {
      $$ = new ASTExpressionRemainder(ast, std::unique_ptr<ASTExpression>($3), std::unique_ptr<ASTExpression>($4));
    } 
/*
    | LPAREN ARITH_MINUS expr RPAREN {  //unary minus
      $$ = new ASTExpressionNegation(ast, std::unique_ptr<ASTExpression>($3));
    }
    | LPAREN CONS expr expr RPAREN
    | LPAREN CAR expr RPAREN
    | LPAREN CDR expr RPAREN */
    | LPAREN relop expr expr RPAREN {
      $$ = new ASTExpressionComparison(ast, $2, std::unique_ptr<ASTExpression>($3), std::unique_ptr<ASTExpression>($4));
    }; /*
    | LPAREN NULL_CHECK_OP expr RPAREN
    | LPAREN BOOL_CHECK_OP expr RPAREN
    | LPAREN NUMBER_CHECK_OP expr RPAREN
    | LPAREN REAL_CHECK_OP expr RPAREN
    | LPAREN LIST_CHECK_OP expr RPAREN  ;
   */ 

//nonterminals inside production body for lambda expression
paramList: | paramList type ID  ;
bindList: | bind bindList  ;
bind: LPAREN ID expr RPAREN  ;

//used when binding pairs are needed in a let
//caseList: | caseList LPAREN expr expr RPAREN  ;

//used for repeated expressions inside list declarations
datumList:
    datum datumList {
        //created node with value of datum and next pointer to the rest of the datumlist
        $$ = new ASTExpressionListNode(ast, std::unique_ptr<ASTExpression>($1), std::unique_ptr<ASTExpressionListNode>($2));
    } |    {
        //initialized to null b/c the next pointer of the final linkedlist node is NULL
        $$ = nullptr;
    };

/* 
    All the explicit values that a programmar can declare in code. Note that ONLY the list
    datum requires a type in it's declaration. This is needed in order for the compiler do to do
    type checks on all the elements it has to process when compiling the AST nodes for a list
*/
datum: 
    intLit {
        $$ = new ASTExpressionInt(ast, $1);
    }  
    | realLit {
        $$ = new ASTExpressionReal(ast, $1);
    }
    | ID {
        $$ = new ASTExpressionVariable(ast, $1);
    }
    | BOOL_LITERAL {
        $$ = new ASTExpressionBool(ast, $1);
    }
    | LIST_TYPE RELOP_LT type RELOP_GT APOSTROPHE LPAREN datumList RPAREN {
        $$ = new ASTExpressionList(ast, std::unique_ptr<VarType>($3), std::unique_ptr<ASTExpressionListNode>($7));
    }
    | STRING_LITERAL {
        $$ = new ASTExpressionString(ast, $1);
    };

intLit: INT_LITERAL | ARITH_MINUS INT_LITERAL {$$ = -1 * $2;};
realLit: REAL_LITERAL | ARITH_MINUS REAL_LITERAL {$$ = -1 * $2;};

relop: RELOP_LT {
  $$ = ASTExpressionComparisonType::LessThan;
} | RELOP_LE {
  $$ = ASTExpressionComparisonType::LessThanOrEqual;
} | RELOP_GT {
  $$ = ASTExpressionComparisonType::GreaterThan;
} | RELOP_GE {
  $$ = ASTExpressionComparisonType::GreaterThanOrEqual;
} | RELOP_EQ {
  $$ = ASTExpressionComparisonType::Equal;
};


%%
int main(int argc, char **argv) {

  // Arg flags:
  bool showHelp = false; // Show the help and exit.
  std::string openFile = ""; // File to open. Nothing for standard in.
  std::string outFile = ""; // File to write to. Nothing for standard out.
  int outputFormat = 3; // 0 - LLVM Assembly. 1 - LLVM Bitcode. 2 - Object (TODO). 3 - AST tree.
  bool printAST = true; // If to print the AST to console.

  // Read the arguments. Don't count the first which is the executable name.
  for (int i = 1; i < argc; i++)
  {
    bool hasNextArg = i + 1 < argc;
    std::string arg(argv[i]);
    if (arg == "-i" && hasNextArg)
    {
      i++;
      openFile = argv[i];
    }
    else if (arg == "-o" && hasNextArg)
    {
      i++;
      outFile = argv[i];
    }
    else if (arg == "-nPrint")
    {
      printAST = false;
    }
    else if (arg == "-fAsm")
    {
      outputFormat = 0;
    }
    else if (arg == "-fBc")
    {
      outputFormat = 1;
    }
    else if (arg == "-fObj")
    {
      outputFormat = 2;
    }
    else if (arg == "-fAst")
    {
      outputFormat = 3;
    }
    else
    {
      showHelp = true;
    }
  }
  printAST &= outputFormat != 3 && outFile != ""; // Always print AST by default in addition to whatever is being output.

  // Show help if needed.
  if (showHelp)
  {
    printf("Usage: LLVM-Lab [options]\n");
    printf("\nOptions:\n\n");
    printf("-h              Show this help screen.\n");
    printf("-i [input]      Read from an input file (reads from console by default).\n");
    printf("-o [output]     Write to an output file (writes to console by default).\n");
    printf("-nPrint         If to not print the AST to the console.\n");
    printf("-fAsm           Output format is in LLVM assembly.\n");
    printf("-fAst           Output format is an abstract syntax tree.\n");
    printf("-fBc            Output format is in LLVM bitcode.\n");
    printf("-fObj           Output format is an object file.\n");
    return 1;
  }

  // Fetch input.
  if (openFile != "")
  {
    yyin = fopen(openFile.c_str(), "r");
  }

  if (yyparse() == 1)
  {
    printf("Irrecoverable error state, aborting\n");
    return 1;
  }

  // Close input if needed.
  if (openFile != "")
  {
    fclose(yyin);
  }

  // Do the compilation.
  ast.Compile();

  // Print AST if needed.
  if (printAST) std::cout << ast.ToString() << std::endl;

  // Export data.
  if (outputFormat == 0)
  {
    ast.WriteLLVMAssemblyToFile(outFile);
  }
  else if (outputFormat == 1)
  {
    ast.WriteLLVMBitcodeToFile(outFile);
  }
  else if (outputFormat == 2)
  {
    std::cout << "OBJ exporting not supported yet." << std::endl;
  }
  else
  {
    std::cout << ast.ToString() << std::endl;
  }
  return 0;
}

void yyerror(const char *s)
{
  fprintf(stderr, "error: %s\n", s);
}
