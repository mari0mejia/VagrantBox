    //CREATE TWO REGULAR EXPERESSIONS
    //int 
    //[a-z]+
    //get this working, try some words such as "int" or "integer", THEN
    //reverse their order
    /*definitions*/

%option noyywrap
%{
    #include<iostream>
    using std::cout;

%}

%%
int {cout <<"just int";}
1 {cout << "ON";cout<<"E!";}
[abc]+ {cout<<"ALPHABET!";}
%%


%%
int main()
{
    yylex();
    return 0;
}
%%
