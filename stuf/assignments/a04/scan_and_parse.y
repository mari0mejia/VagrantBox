  /* directives for the bison parser */
%token T_NOUN
%token T_VERB
%token T_PREPOSITION
%token T_ARTICLE

%{
   #include<iostream>
   //The function prototype for the scanner's scanning function.
   //the parser code that is generated calls this function
   //repeatedly, each time it needs a terminal token
   int yylex(void);
   void yyerror(const char* err_txt)
   {
     std::cerr << "OUCH: " << err_txt << '\n';
   }
%}
%%
  /* parser rules. The rules that describe the grammar */
sentence: %empty


sentence:
        sentence T_NOUN T_VERB {
        std::cout << "Great, you're well on your way to complete sentences\n";
        }
        T_NOUN T_VERB prepositional_phrase {std::cout << "with prepositions!\n";}

prepositional_phrase:
        T_PREPOSITION T_ARTICLE T_NOUN

%%
 /* literally copied into the generated code */
int main()
{
  yyparse();
}

