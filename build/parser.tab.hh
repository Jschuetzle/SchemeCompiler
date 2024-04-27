/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_HOME_JSCHUETZLE_SCHOOL_COMPILERS_SCHEMECOMPILER_BUILD_PARSER_TAB_HH_INCLUDED
# define YY_YY_HOME_JSCHUETZLE_SCHOOL_COMPILERS_SCHEMECOMPILER_BUILD_PARSER_TAB_HH_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 1 "src/frontend/parser.y"

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
	//#include "../src/expressions/string.h"
	#include "../src/expressions/variable.h"
	//#include "../src/expressions/addition.h"
	//#include "../src/expressions/sub.h"
	//#include "../src/expressions/multiplication.h"
	//#include "../src/expressions/division.h"
	//#include "../src/expressions/assignment.h"
	//#include "../src/expressions/comparison.h"
	//#include "../src/expressions/and.h"
	//#include "../src/expressions/or.h"
	//#include "../src/expressions/if.h"
	#include "../src/types/simple.h"
    #include "../src/types/list.h"
	extern FILE *yyin; 

#line 78 "/home/jschuetzle/school/compilers/SchemeCompiler/build/parser.tab.hh"

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    LPAREN = 258,                  /* LPAREN  */
    RPAREN = 259,                  /* RPAREN  */
    INT_TYPE = 260,                /* INT_TYPE  */
    REAL_TYPE = 261,               /* REAL_TYPE  */
    STRING_TYPE = 262,             /* STRING_TYPE  */
    BOOL_TYPE = 263,               /* BOOL_TYPE  */
    LIST_TYPE = 264,               /* LIST_TYPE  */
    APOSTROPHE = 265,              /* APOSTROPHE  */
    BOOL_LITERAL = 266,            /* BOOL_LITERAL  */
    INT_LITERAL = 267,             /* INT_LITERAL  */
    REAL_LITERAL = 268,            /* REAL_LITERAL  */
    ID = 269,                      /* ID  */
    RELOP_GT = 270,                /* RELOP_GT  */
    RELOP_LT = 271,                /* RELOP_LT  */
    RELOP_GE = 272,                /* RELOP_GE  */
    RELOP_LE = 273,                /* RELOP_LE  */
    RELOP_EQ = 274,                /* RELOP_EQ  */
    ARITH_PLUS = 275,              /* ARITH_PLUS  */
    ARITH_MINUS = 276,             /* ARITH_MINUS  */
    ARITH_MULT = 277,              /* ARITH_MULT  */
    ARITH_DIV = 278,               /* ARITH_DIV  */
    ARITH_REMAINDER = 279,         /* ARITH_REMAINDER  */
    NULL_CHECK_OP = 280,           /* NULL_CHECK_OP  */
    BOOL_CHECK_OP = 281,           /* BOOL_CHECK_OP  */
    NUMBER_CHECK_OP = 282,         /* NUMBER_CHECK_OP  */
    REAL_CHECK_OP = 283,           /* REAL_CHECK_OP  */
    LIST_CHECK_OP = 284,           /* LIST_CHECK_OP  */
    LOGICAL_OR = 285,              /* LOGICAL_OR  */
    LOGICAL_AND = 286,             /* LOGICAL_AND  */
    LOGICAL_NOT = 287,             /* LOGICAL_NOT  */
    DEFINE = 288,                  /* DEFINE  */
    LET = 289,                     /* LET  */
    CAR = 290,                     /* CAR  */
    CDR = 291,                     /* CDR  */
    CONS = 292,                    /* CONS  */
    LAMBDA = 293,                  /* LAMBDA  */
    COND = 294,                    /* COND  */
    IF = 295,                      /* IF  */
    ELSE = 296,                    /* ELSE  */
    STRING_LITERAL = 297           /* STRING_LITERAL  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 47 "src/frontend/parser.y"

  bool boolval;
  int intval;
  double fltval;
  char *strval;
  struct node *nodeval;
  //ASTFunctionParameter *var;
  //std::vector<ASTFunctionParameter *> *vars;
  ASTExpression *exp;
  std::vector<ASTExpression *> *exprVec;
  VarType *type;
  //ASTExpressionComparisonType rel;

#line 151 "/home/jschuetzle/school/compilers/SchemeCompiler/build/parser.tab.hh"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_HOME_JSCHUETZLE_SCHOOL_COMPILERS_SCHEMECOMPILER_BUILD_PARSER_TAB_HH_INCLUDED  */
