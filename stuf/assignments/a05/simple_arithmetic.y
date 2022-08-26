%token T_INT_CONSTANT;
%token T_PLUS;
%token T_MINUS;
%token T_MULTIPLY;
%token T_DIVIDE;
%token T_PRINT;
%token T_SIZE;
%token T_ERROR;
%token T_EXP;

%{
  #include<iostream>
  #include <cmath>
  using std::cout;
  using std::cerr;
  using std::endl;

  /* we must provide the prototype to yylex() since its source is
     in a separate file */
  int yylex();

  /* we must write this function. Generated code calls it */
  int yyerror(const char* err);
%}


%%

start_symbol 
    : expr  { cout << " = " << $1 << endl; }
    ;

expr
    :  
    expr T_PLUS OP {$$ = $1 + $3;}
  | expr T_MINUS OP {$$ = $1 - $3;}
  | OP
    ;

OP
    :
      OP T_EXP nbr { $$ = pow($1,$3);}
    | OP T_MULTIPLY nbr { $$ = $1 * $3;}
    | OP T_DIVIDE nbr { $$ = $1 / $3;}
    | nbr
    ;

nbr 
    :
    T_INT_CONSTANT
    ;
    

%%

int main(int argc, char** argv)
{
  yyparse();
}

int yyerror(const char* err)
{
  cerr << err << endl;
  return 0;
}
