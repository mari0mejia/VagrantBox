%union {
  int myunion_int;
}

%type <myunion_int> expn;

%token <myunion_int> T_INT;
%token T_ADD
%token T_MULT

/* A token's level of precedence is determined by the token's 
 * relative position in the file to other tokens with precedence.
 * The higher/earlier in the file, the lower the precedence */
%left T_ADD
%left T_MULT

%{
  #include<iostream>
  using namespace std;
  int yylex();
  int yyerror(const char* err);
%}

%%

some_start_symbol : expn {cout << $1 << endl;}

expn : expn T_ADD expn  {$$=$1+$3;}
     | expn T_MULT expn {$$=$1*$3;}
     | T_INT {$$=$1;}

%%

int main(int argc, char** argv)
{
  double* oneofem = new double(3.141592);
  Symbol one("pi", oneofem);
  cout<<one<<endl;
  yyparse();
}

int yyerror(const char*err)
{
  cerr << err << endl;
  return 1;
}
