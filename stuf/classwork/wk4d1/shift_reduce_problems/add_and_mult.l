%option noyywrap
%option nodefault


%{
  #include "add_and_mult.tab.h"
  #include<string>
  using std::stoi;
%}

%%

[0-9]+  { yylval.myunion_int=stoi(yytext); return T_INT; }
"+"     { return T_ADD; }
"*"     { return T_MULT; }

[ \t\n]      /* do nothing */
.            /* do nothing */

%%
