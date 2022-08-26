 /* eliminate the need to link in the fl library */
%option noyywrap

 /* No longer allow default characters to 
    fall through scanner*/
%option nodefault


%{
  #include "simple_arithmetic.tab.h"
  #include<string>
%}

%%

[0-9]+  { yylval=std::stoi(yytext); return T_INT_CONSTANT; }
"+"     { return T_PLUS; }
"-"     { return T_MINUS; }
"*"     { return T_MULTIPLY; }
"/"     { return T_DIVIDE; }
"^"     { return T_EXP; }
[ \t\n]      /* do nothing */
.       { return T_ERROR; }

%%
