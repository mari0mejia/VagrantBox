T_INT
Terminal token, or just simply, a "terminal"

T_IDENTIFIER("main")
                ^semantic value of the terminal token

T_INT T_IDENTFIER("main") T_PAREN TRPAREN T_LCURLY T_IDENTIFIER("cout") T_LEFTSHIFT
T_STRING_LITERAL("hi there")...< some string
int main()
{
    cout<<"hi there\n";
    return 0;
}
int main(){cout<<"hi there\n";return 0;}

int 
main
(){cout<<"hi there\n";return 0;}



