%option noyywrap
%{
 #include<iostream>
 #include"scan_and_parse.tab.h"
 using std::cout;
%}
%%

"cat"   { return T_NOUN; }
"car"   { return T_NOUN; }
"moose" { return T_NOUN; }
"run"   { return T_VERB; }
"play"  { return T_VERB; }
"in"    { return T_PREPOSITION; }
"the"   { return T_ARTICLE; }

%%
