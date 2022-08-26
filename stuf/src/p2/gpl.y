// Mon Apr 27 16:03:16 PDT 2020
// bison syntax indicating C++ declarations required by both the parser and scanner
%code requires {
  #include <string>
  #include "Scope_manager.h"
  class Expression;
  class Variable;
  struct Parameter;
  class Statement;
  #ifndef P1
    #include "types_and_ops.h"  //include in all projects except the first
    using namespace GPL;
  #endif
  #ifdef GRAPHICS
    #include "Window.h"
  #endif
}

// bison syntax to indicate the beginning of a C/C++ code section
%{

extern int yylex();  // prototype of function generated by flex
extern int yyerror(const char *); // used to print errors
extern int line_count;            // current line in the input; from record.l

#include "error.h"      // class for printing errors (used by gpl)
#include <iostream>

// bison syntax indicating the end of a C/C++ code section

%} 


// turn on verbose (longer) error messages
%define parse.error verbose

%union {
 GPL::Type union_gpl_type;
 int            union_int;
 double         union_double;
 std::string*   union_string;  // MUST be a pointer to a string
};
%destructor { delete $$; } <union_string>


// tokens declared here


%token T_INT                 "int"
%token T_DOUBLE              "double"
%token T_STRING              "string"
%token T_TRIANGLE            "triangle"
%token T_PIXMAP              "pixmap"
%token T_CIRCLE              "circle"
%token T_RECTANGLE           "rectangle"
%token T_TEXTBOX             "textbox"

%token T_FORWARD             "forward"
%token T_INITIALIZATION      "initialization"
%token T_TERMINATION         "termination"
%token T_ON                  "on"
%token T_ANIMATION           "animation"
%token T_IF                  "if"
%token T_FOR                 "for"
%token T_ELSE                "else"
%token <union_int> T_EXIT            "exit"  /* value is line number */
%token <union_int> T_PRINT           "print"  /* value is line number */
%token T_TRUE                "true"
%token T_FALSE               "false"

%token T_SPACE               "space"
%token T_LEFTARROW           "leftarrow"
%token T_RIGHTARROW          "rightarrow"
%token T_UPARROW             "uparrow"
%token T_DOWNARROW           "downarrow"
%token T_LEFTMOUSE_DOWN      "leftmouse_down"
%token T_MIDDLEMOUSE_DOWN    "middlemouse_down"
%token T_RIGHTMOUSE_DOWN     "rightmouse_down"
%token T_LEFTMOUSE_UP        "leftmouse_up"
%token T_MIDDLEMOUSE_UP      "middlemouse_up"
%token T_RIGHTMOUSE_UP       "rightmouse_up"
%token T_MOUSE_MOVE          "mouse_move"
%token T_MOUSE_DRAG          "mouse_drag"
%token T_F1                  "f1"
%token T_AKEY                "akey"
%token T_SKEY                "skey"
%token T_DKEY                "dkey"
%token T_FKEY                "fkey"
%token T_HKEY                "hkey"
%token T_JKEY                "jkey"
%token T_KKEY                "kkey"
%token T_LKEY                "lkey"
%token T_WKEY                "wkey"
%token T_ZKEY                "zkey"

%token T_LPAREN              "("
%token T_RPAREN              ")"
%token T_LBRACE              "{"
%token T_RBRACE              "}"
%token T_LBRACKET            "["
%token T_RBRACKET            "]"
%token T_SEMIC               ";"
%token T_COMMA               ","
%token T_PERIOD              "."

%token T_ASSIGN              "="
%token T_PLUS_ASSIGN         "+="
%token T_MINUS_ASSIGN        "-="
%token T_PLUS_PLUS           "++"
%token T_MINUS_MINUS         "--"

%token T_SIN                 "sin"
%token T_COS                 "cos"
%token T_TAN                 "tan"
%token T_ASIN                "asin"
%token T_ACOS                "acos"
%token T_ATAN                "atan"
%token T_SQRT                "sqrt"
%token T_ABS                 "abs"
%token T_FLOOR               "floor"
%token T_RANDOM              "random"

%token T_TOUCHES             "touches"
%token T_NEAR                "near"

%token T_MULTIPLY            "*"
%token T_DIVIDE              "/"
%token T_PLUS                "+"
%token T_MINUS               "-"
%token T_MOD                 "%"

%token T_LESS                "<"
%token T_GREATER             ">"
%token T_LESS_EQUAL          "<="
%token T_GREATER_EQUAL       ">="
%token T_EQUAL               "=="
%token T_NOT_EQUAL           "!="
%token T_NOT                 "!"
%token T_AND                 "&&"
%token T_OR                  "||"
%token UNARY_OPS
%token IF_NO_ELSE

%token <union_int> T_INT_CONSTANT    "int constant"
%token <union_double> T_DOUBLE_CONSTANT "double constant"
%token <union_string> T_STRING_CONSTANT "string constant"
%token <union_string> T_ID;
//%token <union_int> T_ID            	 "identifier"
%type <union_gpl_type> simple_type;

 /* special token that does not match any production */
 /* used for characters that are not part of the language */
%token T_ERROR               "error"
/* Bitwise Comma , */
/* Bitwise Assignment = += -= *= /= %= >>= <<= &= ^= |=*/
/* Bitwise Conditional ? */
/* Bitwise Logical OR || */
/* Bitwise Logical And && */
/* Bitwise OR | */
/*Bitwise XOR ^ */
/*Bitwise AND & */
/*Equality == != */
/*Relational < <= > >=*/
/*Shift << >>*/
%nonassoc IF_NO_ELSE
%nonassoc T_ELSE
%left T_OR
%left T_AND 
%left T_EQUAL T_NOT_EQUAL 
%left T_LESS T_LESS_EQUAL T_GREATER T_GREATER_EQUAL 

%left T_PLUS T_MINUS /*Additive*/
%left T_MULTIPLY T_DIVIDE T_MOD/*Multiplicative*/
/*Unary + - ! ~ ++ -- (type)* & sizeof */
%nonassoc UNARY_OPS 
%nonassoc T_NOT
/*Postfix () [] -> . ++ -- */
%nonassoc T_NEAR T_TOUCHES

%%

// updated October 2021

//---------------------------------------------------------------------
program:
    declaration_list block_list


//---------------------------------------------------------------------
declaration_list:
    declaration_list declaration
    | %empty


//---------------------------------------------------------------------
declaration:
    variable_declaration T_SEMIC
    | object_declaration T_SEMIC
    | forward_declaration T_SEMIC


//---------------------------------------------------------------------
variable_declaration:
    simple_type  T_ID  optional_initializer
    {
        Scope_manager& scopemgr=Scope_manager::instance();//how to refere to scope manager
        if(scopemgr.defined_in_current_scope(*$2))
        {
            Error::error(Error::PREVIOUSLY_DECLARED_VARIABLE,*$2);
            delete $2;
            break;
        }
        switch($1)
        {
            case GPL::INT:
            {
                int* ptr = new int(42);
                scopemgr.add_to_current_scope(std::make_shared<Symbol>(*$2,ptr));
                delete $2;
                break;
            }
            case GPL::DOUBLE:
            {
                double* ptr = new double(3.14159);
                scopemgr.add_to_current_scope(std::make_shared<Symbol>(*$2,ptr));
                delete $2;
                break;
            }
            case GPL::STRING:
            {
                std::string* ptr = new std::string("Hello world");
                scopemgr.add_to_current_scope(std::make_shared<Symbol>(*$2,ptr));
                delete $2;
                break;
            }
            
            default:
            {
                assert(false);
            }
        }
    }
    | simple_type  T_ID T_LBRACKET T_INT_CONSTANT T_RBRACKET
    {
        Scope_manager& scopemgr=Scope_manager::instance();//how to refere to scope manager
        if(scopemgr.defined_in_current_scope(*$2))
        {
            Error::error(Error::PREVIOUSLY_DECLARED_VARIABLE,*$2);
            delete $2;
            break;
        }
        if(!($4))
        {
            Error::error(Error::INVALID_ARRAY_SIZE,*$2,std::to_string($4));
            delete $2;
            break;
        }
        else{
            switch($1)
            {
                case GPL::INT:
                {
                    int* ptr = new int[$4]{0};
                    // for(auto i = 0; i < $4; ++i)
                    // {
                    // }
                    for(auto i = 0; i < $4; ++i)
                    {
                        ptr[i] = 42;
                    }
                    scopemgr.add_to_current_scope(std::make_shared<Symbol>(*$2,ptr,$4));
                    delete $2;
                    //std::cout <<"this is the value for argument 4 " << $4 <<"\n";
                    break;
                }
                case GPL::DOUBLE:
                {
                    double* ptr = new double[$4];
                    for(auto i = 0; i < $4; ++i)
                    {
                        ptr[i] = 3.14159;
                    }
                    scopemgr.add_to_current_scope(std::make_shared<Symbol>(*$2,ptr,$4));
                    delete $2;
                    break;
                }
                case GPL::STRING:
                {
                    std::string* ptr = new std::string[$4];
                    for(auto i = 0; i < $4; ++i)
                    {
                        ptr[i] = "Hello world";
                    }
                    scopemgr.add_to_current_scope(std::make_shared<Symbol>(*$2,ptr,$4));
                    delete $2;
                    break;
                }
                
                default:
                {
                    assert(false);
                }
            }
        }
        
    }

//---------------------------------------------------------------------
simple_type:
    T_INT       {$$=GPL::INT;}
    | T_DOUBLE  {$$=GPL::DOUBLE;}
    | T_STRING  {$$=GPL::STRING;}


//---------------------------------------------------------------------
optional_initializer:
    T_ASSIGN expression
    | %empty


//---------------------------------------------------------------------
object_declaration:
    object_type T_ID parameter_list_or_empty {$2; /*Change*/}
    | object_type T_ID T_LBRACKET expression T_RBRACKET {$2; /*Change*/}


//---------------------------------------------------------------------
object_type:
    T_TRIANGLE
    | T_PIXMAP
    | T_CIRCLE
    | T_RECTANGLE
    | T_TEXTBOX


//---------------------------------------------------------------------
parameter_list_or_empty :
    T_LPAREN parameter_list T_RPAREN
    | T_LPAREN T_RPAREN
    | %empty


//---------------------------------------------------------------------
parameter_list :
    parameter_list T_COMMA parameter
    | parameter


//---------------------------------------------------------------------
parameter:
    T_ID T_ASSIGN expression


//---------------------------------------------------------------------
block_list:
    block_list block
    | %empty


//---------------------------------------------------------------------
block:
    initialization_block
    | termination_block
    | animation_block
    | on_block


//---------------------------------------------------------------------
initialization_block:
    T_INITIALIZATION statement_block


//---------------------------------------------------------------------
termination_block:
    T_TERMINATION statement_block


//---------------------------------------------------------------------
forward_declaration:
    T_FORWARD T_ANIMATION T_ID T_LPAREN animation_parameter T_RPAREN { $3; /*Change*/}


//---------------------------------------------------------------------
animation_parameter:
    object_type T_ID { $2; /*Change*/}
    | object_type


//---------------------------------------------------------------------
animation_block:
    animation_declaration statement_block


//---------------------------------------------------------------------
animation_declaration:
    T_ANIMATION T_ID T_LPAREN object_type T_ID T_RPAREN { $2; $5;/*Change*/}


//---------------------------------------------------------------------
on_block:
    T_ON keystroke statement_block


//---------------------------------------------------------------------
keystroke:
    T_SPACE
    | T_UPARROW
    | T_DOWNARROW
    | T_LEFTARROW
    | T_RIGHTARROW
    | T_LEFTMOUSE_DOWN
    | T_MIDDLEMOUSE_DOWN
    | T_RIGHTMOUSE_DOWN
    | T_LEFTMOUSE_UP
    | T_MIDDLEMOUSE_UP
    | T_RIGHTMOUSE_UP
    | T_MOUSE_MOVE
    | T_MOUSE_DRAG
    | T_AKEY
    | T_SKEY
    | T_DKEY
    | T_FKEY
    | T_HKEY
    | T_JKEY
    | T_KKEY
    | T_LKEY
    | T_WKEY
    | T_ZKEY
    | T_F1


//---------------------------------------------------------------------
statement_or_block_of_statements:
    statement_block
    | statement


//---------------------------------------------------------------------
statement_block:
    T_LBRACE statement_list T_RBRACE
    | T_LBRACE T_RBRACE



//---------------------------------------------------------------------
statement_list:
    statement_list statement
    | statement


//---------------------------------------------------------------------
statement:
    if_statement
    | for_statement
    | assign_statement T_SEMIC
    | print_statement T_SEMIC
    | exit_statement T_SEMIC


//---------------------------------------------------------------------
if_statement:
    T_IF T_LPAREN expression T_RPAREN statement_or_block_of_statements %prec IF_NO_ELSE
    | T_IF T_LPAREN expression T_RPAREN statement_or_block_of_statements T_ELSE statement_or_block_of_statements 


//---------------------------------------------------------------------
for_statement:
    T_FOR T_LPAREN assign_statement_or_empty T_SEMIC expression T_SEMIC assign_statement_or_empty T_RPAREN statement_or_block_of_statements


//---------------------------------------------------------------------
print_statement:
    T_PRINT T_LPAREN expression T_RPAREN


//---------------------------------------------------------------------
exit_statement:
    T_EXIT T_LPAREN expression T_RPAREN


//---------------------------------------------------------------------
assign_statement_or_empty:
    assign_statement
    | %empty


//---------------------------------------------------------------------
assign_statement:
    variable T_ASSIGN expression
    | variable T_PLUS_ASSIGN expression
    | variable T_MINUS_ASSIGN expression
    | variable T_PLUS_PLUS
    | variable T_MINUS_MINUS


//---------------------------------------------------------------------
variable:
    T_ID
    | T_ID T_LBRACKET expression T_RBRACKET
    | T_ID T_PERIOD T_ID { $1;$3; /*Change*/}
    | T_ID T_LBRACKET expression T_RBRACKET T_PERIOD T_ID { $1;$6; /*Change*/}


//---------------------------------------------------------------------
expression:
    primary_expression
    | expression T_OR expression
    | expression T_AND expression
    | expression T_LESS_EQUAL expression
    | expression T_GREATER_EQUAL  expression
    | expression T_LESS expression
    | expression T_GREATER  expression
    | expression T_EQUAL expression
    | expression T_NOT_EQUAL expression
    | expression T_PLUS expression
    | expression T_MINUS expression
    | expression T_MULTIPLY expression
    | expression T_NEAR expression
    | expression T_TOUCHES expression
    | expression T_DIVIDE expression
    | expression T_MOD expression
    | T_MINUS  expression %prec UNARY_OPS
    | T_NOT  expression
    | T_SIN T_LPAREN expression T_RPAREN
    | T_COS T_LPAREN expression T_RPAREN
    | T_TAN T_LPAREN expression T_RPAREN
    | T_ASIN T_LPAREN expression T_RPAREN
    | T_ACOS T_LPAREN expression T_RPAREN
    | T_ATAN T_LPAREN expression T_RPAREN
    | T_SQRT T_LPAREN expression T_RPAREN
    | T_ABS T_LPAREN expression T_RPAREN
    | T_FLOOR T_LPAREN expression T_RPAREN
    | T_RANDOM T_LPAREN expression T_RPAREN


//---------------------------------------------------------------------
primary_expression:
    T_LPAREN  expression T_RPAREN
    | variable
    | T_INT_CONSTANT 
    | T_TRUE
    | T_FALSE
    | T_DOUBLE_CONSTANT 
    | T_STRING_CONSTANT { delete $1; /*Change*/}

%%
