/*** Definition Section has one variable
which can be accessed inside yylex()
and main() 
%s ",yytext);}
***/

%{
    //defenitions
%}
%option noyywrap

%%
 /*Rules*/
(the|The) {printf("T_DEFINITE_ARTICLE");}
(quick|brown|lazy) {printf("T_ADJECTIVE");}
(fox|dog|wall|bottles|liquid) {printf("T_NOUN");}
(jumped|see) {printf("T_VERB");}
(over|on|of) {printf("T_PREPOSITION");}
[0-9]{2} {printf("T_INTEGER");}
\. {printf("T_PERIOD");}
(I) {printf("T_PRONOUN");}
\n {printf("\n");}
%%

//int yywrap(){return 0;}
int main(){

// Explanation:
// yywrap() - wraps the above rule section
/* yyin - takes the file pointer
		which contains the input*/
/* yylex() - this is the main flex function
		which runs the Rule Section*/
// yytext is the text in the buffer

yylex();

return 0;
}
