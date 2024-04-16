%{
#include "parser.tab.hh"
#include <sstream>

static std::stringstream ss;
%}

%option noyywrap

%x strlit

%%
"(" { return LPAREN;}
")" { return RPAREN;}

"#f" | "#t" { yylval.boolval = strcmp(yytext, "#t") == 0; return BOOL_LITERAL; }
-?[0-9]+ { yylval.intval = atoi(yytext); return INT_LITERAL;}
-?([0-9]*\.[0-9]+([eE][-+]?[0-9]+)?)  { yylval.fltval = atof(yytext); return REAL_LITERAL;}
[a-zA-Z_]+[a-zA-Z0-9_]*     { yyval.strval = yytext; return ID; }

">" { return RELOP_GT; }
"<" { return RELOP_LT; }
">=" { return RELOP_GE; }
"<=" { return RELOP_LE; }
"eq?" { return RELOP_EQ;}
"+" { return ARITH_PLUS; }
"-" { return ARITH_MINUS; }
"*" { return ARITH_MULT; }
"/" { return ARITH_DIV; }
"remainder" { return ARITH_REMAINDER; }

"null?" { return NULL_CHECK_OP; }
"boolean?" { return BOOL_CHECK_OP; }
"number?" { return NUMBER_CHECK_OP; }
"real?" { return REAL_CHECK_OP; }
"list?" { return LIST_CHECK_OP; }
"or" { return LOGICAL_OR; }
"and" { return LOGICAL_AND; }
"not" { return LOGICAL_NOT; }
"set!" { return SET; }
"define" { return DEFINE; }
"let" { return LET; }
"car" { return CAR; }
"cdr" { return CDR; }
"cons" { return CONS; }
"lambda" { return LAMBDA; }
"cond" { return COND; }
"if" { return IF; }
"else" { return ELSE; }
"'"    { return APOSTROPHE; } 
\"                  { BEGIN strlit; ss.str(std::string()); }
<strlit>[^\\"\n]*   { ss << yytext;}
<strlit>\\n         { ss << '\n';}
<strlit>\\t         { ss << '\t';}
<strlit>\\[\\"]     { ss << yytext[1]; /*escaped quote or backslash*/ }
<strlit>\"          { yylval.strval = strdup((char *) ss.str().c_str()); BEGIN 0; return STRING_LITERAL; }
<strlit>\\.         { printf("Invalid escape character '%s'\n", yytext); }
<strlit>\n          { printf("Found newline in string\n"); }

[\t\n ]             {}
. { printf("Unrecognized character %c\n", *yytext); }
%%

  /*int main(int argc, char **argv) {
  int tok;

  while((tok = yylex())) {
    printf("%d", tok);
    if(tok == INT_LITERAL) printf(" = %d\n", yyintval);
    else if (tok == FLOAT_LITERAL) printf(" = %0.3f\n", yyfltval);
    else if (tok == STRING_LITERAL) printf(" = %s\n", yytext);
    else if (tok == ID) printf(" called %s\n", yytext);
    else printf("\n");
  }
}
  */
