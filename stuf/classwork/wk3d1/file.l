%option noyywrap
%{
 #include<iostream>
 using std::cout;
%}
%%
"cat" {return T_NOUN;}
"car" {return T_NOUN;}
"moose" {return T_NOUN;}
"run" {return T_VERB;}
"play" {return T_VERB;}

[0-9]+ {cout<<"NUBER\n";}
. {cout<<"CHARACTER\n";}
%%


//scanner no longer drives this its the parser
