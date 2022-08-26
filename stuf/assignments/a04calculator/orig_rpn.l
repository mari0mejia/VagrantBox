%option noyywrap
%option nodefault
%{
  #include<stack>
  #include<string>
  #include<iostream>
  using std::cout;
  using std::cerr;
  using std::endl;
  using std::stoi;
  std::stack<int> rpnstack;
  bool stack_error(int expected);
%}

%%

[0-9]+  { rpnstack.push(stoi(yytext)); }

"+"     { if(stack_error(2)) break; int a=rpnstack.top(); rpnstack.pop(); int b=rpnstack.top(); rpnstack.pop(); rpnstack.push(a+b);}

"-"       { if(stack_error(2)) break; int a=rpnstack.top(); rpnstack.pop(); int b=rpnstack.top(); rpnstack.pop(); rpnstack.push(a-b);}

"*"      { if(stack_error(2)) break; int a=rpnstack.top(); rpnstack.pop(); int b=rpnstack.top(); rpnstack.pop(); rpnstack.push(a*b);}

\/      { if(stack_error(2)) break; int a=rpnstack.top(); rpnstack.pop(); int b=rpnstack.top(); rpnstack.pop(); rpnstack.push(a/b);}

\.      { if(stack_error(1)) break; cout << rpnstack.top() << endl; rpnstack.pop(); }

\?      { cout << "stack=" << rpnstack.size() << endl; }

[ \t\n]      /* do nothing */

.       { cerr << "DOES NOT COMPUTE!" << endl; }

%%

int main(int argc, char** argv)
{
  cout << "Welcome to rpn calculator. <ctrl>-d to quit"<<endl;
  yylex();
  return 0;
}

bool stack_error(int expected)
{
  if(rpnstack.size() < expected)
  {
    cerr << "Stack size to small!\n";
    return true;
  }
  return false;
}
