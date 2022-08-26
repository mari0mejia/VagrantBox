/*directives for bison parser*/

%token T_NOUN
%token T_VERB
%{
    //the function protoype for the scanners scanning funciton
    //the paser that is generated calls this function
    //repeatedly ecah time it needs a terminal token 
    int yylex(void);
    void yyerror(const char* err_txt)
    {
        std::cerr <<"OUCH: "<<err_txt<<endl;
    }
%}
%%
/*parser rules. The rules that describe the grammar*/
//action to take when given the following sentence
sentence:
    T_NOUN T_VERB {std::cout<<"output\n";} //only works for noun following a verb
%%

//what do the directives do?
/*literally copied into the generated code from flex file*/
int main()
{
    yyparse();//busy wait to match each token to a rule
}

