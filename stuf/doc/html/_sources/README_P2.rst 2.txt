.. .. highlight:: cpp
..    :linenothreshold: 5


##############
Second Project
##############

Ver. 29 September 2020

=======
Summary
=======

Symbols and symbol tables are introduced to manage the declaration and access of variables that are created in GPL.

========
Manifest
========

        :``./src/p2/gpl_type.h``:             Enumerations & prototypes for managing GPL language types.
        :``./src/p2/gpl_type.cpp``:           Source to utility functions declared in header file.
        :``./src/p2/tests``:                  Testing suite directory, 25 tests.

|

        :``./src/p2/sample_singleton.cpp``:   Sample code--implementing Singleton class

===================
Project description
===================

1.  Expand the tarball. Copy the following files over from ``p1`` into ``p2``:

   ::

        gpl.cpp
        gpl.l
        gpl.y
        error.h
        error.cpp
        Makefile

|

2.  To assist in debugging the first project, actions were added to three of the rules in the ``primary_expression:`` production. Remove them now:

    Original:

    .. raw:: html

      <pre>primary_expression:
          T_LPAREN  expression T_RPAREN
          | variable
          | T_INT_CONSTANT <span style="font-weight:bold;color:green;">{ std::cout << "Integer constant -->" << $1 << "<--\n";}</span>
          | T_TRUE
          | T_FALSE
          | T_DOUBLE_CONSTANT <span style="font-weight:bold;color:green;">{ std::cout << "Double constant -->" << $1 << "<--\n";}</span>
          | T_STRING_CONSTANT <span style="font-weight:bold;color:green;">{ std::cout << "String constant -->" << &#42;$1 << "<--\n";}</span>
          ;</pre>

    With actions removed:

    .. raw:: html

      <pre>primary_expression:
          T_LPAREN  expression T_RPAREN
          | variable
          | T_INT_CONSTANT
          | T_TRUE
          | T_FALSE
          | T_DOUBLE_CONSTANT
          | T_STRING_CONSTANT
          ;</pre>

|

3.  For this second project, three kinds of symbols are managed: strings, integers, and doubles. To facilitate this, a union of the three possible types will be declared as a private data member of a ``Symbol`` class. Each member of the union is a pointer to the type it manages. Creating a constructor for each union member eases its use in ``Symbol``'s initialization list. Example of union declaration:

   .. code-block::

        union symbol_type {
          double* double_pointer;
          //etc. for int and string

          //constructors to ease initialization of union variables
          symbol_type(double* val) : double_pointer(val){}
          //etc. for int and string
        };

|

4.  Write a ``Symbol`` class.  

    Private data:

    .. code-block::

        std::string name; //the name of the variable
        symbol_type value; //the contents of the variable
        Gpl_type type;  //the type of the variable. See gpl_type.h
        int count; //used to store the size if the variable is an array

    Public member functions:

    .. code-block::

        Symbol(const std::string& name, double* value); //for double variable
        Symbol(const std::string& name, double* value, int count); //for double array
        //etc. for int and string

        Gpl_type get_type() const;
        std::string get_name() const;
        virtual ~Symbol();
        friend std::ostream& operator<<(std::ostream& os, const Symbol&);

        //prevent compiler from generating these functions
        Symbol(const Symbol&) = delete;
        Symbol& operator=(const Symbol&) = delete;

    The data member ``value`` points to the actual value (int, double, or string) or array of same. The value is created using ``new`` outside of the constructor. That is, the allocation using ``new`` occurs prior to creating/declaring the Symbol object. The constructor takes a pointer to the newly-allocated value(s). The destructor (``Symbol::~Symbol``) is responsible for releasing the memory (``delete``, or ``delete []``, depending on whether value points to a single variable or array of variables). A fair amount of branching (if/elses, switches, and/or ternary operators) will be required to properly release the memory based on its type and whether it is an array.

    The insertion operator (operator<<) should print out the symbol in the following format::

        type varname = value

    Examples::

        int rocket_number = 99
        double gpa = 3.5
        string name = "Icabod Crane"

    Note that the double-quotes surrounding the string are added by the printing routine. They are not part of the string's value. The flex pattern action from the previous project strips the quotes off.

    A symbol of an array of values should list each element. For example::

        int machine[0] = 235
        int machine[1] = 42
        int machine[2] = 6024

    The ``gpl_type_to_string()`` function can be useful here (see ``gpl_type.(h|cpp)``)

|

5.  Test the ``Symbol`` class. Write a test program that creates various Symbols.::

        int main()
        {
          double* oneofem=new double(3.141592);
          double* bunchofem=new double[4] {1.123, 2.234, 3.456, 7.899};
          Symbol one("pi", oneofem);
          Symbol two("racetimes", bunchofem, 4);
          cout << two << endl;
          cout << one << endl;
        }
        //BE SURE TO TEST INT AND STRING AS WELL.

    Compile your code with the ``-g`` option. Run the test code through ``valgrind`` to confirm that there are no memory leaks::

        valgrind ./a.out

        ...
        ==9006== All heap blocks were freed -- no leaks are possible
        ...

|

6.  Write the ``Symbol_table`` class.  The ``Symbol_table`` class manages an ``unordered_map`` with the symbol's name as the key and a pointer to the ``Symbol`` object as the value. Smart pointers are used to avoid memory leaks::

        class Symbol_table {
          public:
            Symbol_table(){}
            std::shared_ptr<Symbol> lookup(const std::string& name);
            bool insert(std::shared_ptr<Symbol> sym);
            friend std::ostream& operator<<(std::ostream& os, const Symbol_table&);

            //prevent compiler from generating these functions
            Symbol_table(const Symbol_table&) = delete;
            Symbol_table& operator=(const Symbol_table&) = delete;
          private:
            std::unordered_map<std::string, std::shared_ptr<Symbol>> symbols;
        };

    The key of the ``unordered_map`` is the symbol's name.

    ``Symbol_table::lookup`` 
        Return the named ``Symbol`` object if it is found in the ``unordered_map``, and ``nullptr`` otherwise. Note that ``unordered_map::find()`` should be used to search the keys for the symbol's name. Using ``[ ]`` will erroneously create an entry if it doesn't already exist.

    ``Symbol_table::insert`` 
        If the ``Symbol`` input parameter already exists in the ``unordered_map`` return ``false``. Otherwise add the ``Symbol`` to the map and return ``true``.

    ``Symbol_table::operator<<`` 
        Print out all of the symbols in the table sorted by name. The format of the output is: **type name = value**. For example::

          int a = 42
          double b = 3.14159
          int c = 42
          string d = "Hello world"
          int nums[0] = 42
          int nums[1] = 42
          int nums[2] = 42

|

7. Here's a test program to ensure the ``Symbol_table`` class is bug-free. Run the test code through ``valgrind`` to confirm that there are no memory leaks::

        #include<iostream>
        #include<memory>
        #include "Symbol.h"
        #include "Symbol_table.h"
        using namespace std;

        int main()
        {
          Symbol_table symtab;

          double* double_oneofem=new double(3.141592);
          double* double_bunchofem=new double[4] {1.123, 2.234, 3.456, 7.899};
          string* string_oneofem=new string("hi there");
          string* string_bunchofem=new string[4] 
            {"one", "two two", "three three three", "fo fo fo fo"};
          int* int_oneofem=new int(321);
          int* int_bunchofem=new int[4] {12, 34, 56, 78};

          symtab.insert(std::make_shared<Symbol>("alpha", string_oneofem));
          symtab.insert(std::make_shared<Symbol>("gamma", string_bunchofem, 4));
          symtab.insert(std::make_shared<Symbol>("beta", double_oneofem));
          symtab.insert(std::make_shared<Symbol>("delta", double_bunchofem, 4));
          symtab.insert(std::make_shared<Symbol>("zeta", int_oneofem));
          symtab.insert(std::make_shared<Symbol>("eta", int_bunchofem, 4));

          cout << symtab << endl;
          return 0;
        }

|

8.  Write the ``Table_handler`` class. This class is a *singleton* class that ensures no more than one ``Table_handler`` object exists in the application. See ``sample_singleton.cpp`` for an example of how to structure a class as a singleton. Be sure to move ``sample_singleton.cpp`` out of the project directory or the Makefile will try to add it to the project build. Use a ``std::vector`` as the private data to manage multiple symbol tables::

        std::vector<std::shared_ptr<Symbol_table>> tables;

    The public member functions include:

    ``static Table_handler& instance()`` 
        Return the singleton instance.

    ``void push_table()`` 
        Create a new ``Symbol_table`` and ``push_back`` onto the vector.

    ``void pop_table()`` 
        If more than one table is on the vector ``pop_back()`` to remove it. If there is only one table on the vector, throw an exception::

          #include<stdexcept>
          throw std::underflow_error("Can't pop global symbol table");

    ``bool insert(std::shared_ptr<Symbol> sym)`` 
        Add a symbol to the top/last/back table in the vector via a call to ``Symbol_table::insert``. Return the value that ``Symbol_table::insert`` returns.

    ``std::shared_ptr<Symbol> lookup(const std::string& name)`` 
        Iterate through the symbol table vector *from back to front*, calling ``Symbol_table::lookup`` on each. Return the first match found. Return ``nullptr`` if the symbol is undefined.

    ``bool defined_in_current_scope(const std::string& name)`` 
        Search the top/last/back table for the named symbol. Return true if it is defined, false otherwise. Used to check for variable redefinition errors.

    ``friend std::ostream& operator<<(std::ostream& os, const Table_handler& sh)`` 
        Iterate through the symbol tables *from back to front*, printing each.

    The private member function:

    * The ``Table_handler`` default constructor should simply call ``push_table()``. This pushes an initial ``Symbol_table`` object onto the vector, representing the global namespace (to hold global variables).

|

9.  Write a test program to ensure ``Table_handler`` is bug-free. Run the test code through ``valgrind`` to ensure it is free of memory leaks. A small modification to the previous test program should be sufficient::

        #include "Table_handler.h"
        ...
        //comment out:
        //Symbol_table symtab;
        //and replace with:
        Table_handler& symtab = Table_handler::instance();


|

10.  In the ``simple_type:`` production in ``gpl.y``, each rule should set the production's semantic value of to the corresponding ``Gpl_type``:

    .. raw:: html
        
        <pre>simple_type:
            T_INT      <span style="font-weight:bold;color:DarkRed;">{$$=INT;}</span>
            | T_DOUBLE <span style="font-weight:bold;color:DarkRed;">{$$=DOUBLE;}</span>
            | T_STRING  <span style="font-weight:bold;color:DarkRed;">{$$=STRING;}</span></pre>

|

11. Add ``gpl_type.h`` to the header files included near the top of ``gpl.y``.

|

12. Assigning a ``Gpl_type`` to ``simple_type`` requires two additional configurations:

    1. Add ``Gpl_type union_gpl_type;`` to the ``%union`` declaration.

    2. Declare the type that ``simple_type`` holds::

            %type <union_gpl_type> simple_type

       This can be appended just after the ``%token`` declarations.

|

13. For this project, simplify the rule in the ``variable_declaration`` production for declaring arrays. 

    Original:

    .. raw:: html

        <pre>| simple_type  T_ID T_LBRACKET <span style="font-weight:bold;color:green;">expression</span> T_RBRACKET</pre>

    Simplified:

    .. raw:: html

        <pre>| simple_type  T_ID T_LBRACKET <span style="font-weight:bold;color:DarkRed;">T_INT_CONSTANT</span> T_RBRACKET</pre>


|

14. At this point the project can be built (``make``) and tested (``../../bin/gt``). Several tests will fail.  
    
    The build may generate warnings similar to:

    .. code-block:: make

       Symbol.cpp: In destructor ‘virtual Symbol::~Symbol()’:
       Symbol.cpp:36:11: warning: enumeration value ‘NO_TYPE’ not handled in switch [-Wswitch]
          36 |     switch(type)
             |           ^
       Symbol.cpp:36:11: warning: enumeration value ‘ANIMATION_BLOCK’ not handled in switch [-Wswitch]
       Symbol.cpp:36:11: warning: enumeration value ‘GAME_OBJECT’ not handled in switch [-Wswitch]
       Symbol.cpp:36:11: warning: enumeration value ‘CIRCLE’ not handled in switch [-Wswitch]
       Symbol.cpp:36:11: warning: enumeration value ‘RECTANGLE’ not handled in switch [-Wswitch]
       Symbol.cpp:36:11: warning: enumeration value ‘TRIANGLE’ not handled in switch [-Wswitch]
       Symbol.cpp:36:11: warning: enumeration value ‘TEXTBOX’ not handled in switch [-Wswitch]
       Symbol.cpp:36:11: warning: enumeration value ‘PIXMAP’ not handled in switch [-Wswitch]
       Symbol.cpp:36:11: warning: enumeration value ‘ANIMATION_CODE’ not handled in switch [-Wswitch]
       Symbol.cpp:51:11: warning: enumeration value ‘NO_TYPE’ not handled in switch [-Wswitch]

    These warnings indicate that the enumeration type ``Gpl_type`` has many possible values not handled by the cases in switch statements. These warnings can be silenced by adding a default case::

      default:
        assert(false); //be sure to #include<cassert>

    If the default case is entered GPL will immediately abort. The code should not enter the default case. An abort allows for prompt debugging of the issue.

|

15. For the ``variable_declaration`` production, write an action for each of the two rules. The action for the first rule will allocate and initialize individual variables. The action for the second rule will allocate and initialize arrays. For simplicity at this point, each individual variable and each element of an array should be initialized using hard-code values::

        int     42
        double  3.14159
        string  "Hello world"

    The code used to test ``Symbol_table`` and ``Table_handler`` can provide clues on how to do this. Note that array elements will have to be initialized using a loop.

|

16. Running the project tests reveals that several are failing because error-checking that is required. Use ``vde`` to check error output, inspect the content of the ``tests/t###.gpl`` files directly to see why the test's GPL code generates an error, and consult ``error.cpp`` for the enumeration that corresponds to the expected error. Example of error-handling code:


    .. code-block:: cpp
       :force:

       Table_handler& tabhand=Table_handler::instance();
       if(tabhand.defined_in_current_scope(*$2))
       {
         Error::error(Error::PREVIOUSLY_DECLARED_VARIABLE,*$2);
         // break after error because bison-generated code
         // embeds each rule in a case of a switch statement
         break; 
       }

    Depending on the error type, the ``Error::error`` function may take up to three string arguments (the above example shows one). Non-string arguments should be converted to strings, for example: ``std::to_string($4)``

|

17. After these actions are written including error handling, all of the tests will pass. The next step is to eliminate memory leaks. Use ``valgrind`` to manually check individual tests for leaks. Use ``memvalidate.bash`` to check all tests (akin to the ``gt`` utility) for memory leaks.

|

18. The scanner (``gpl.l``) dynamically allocates strings for the semantic values of ``T_ID``. The parser needs to release this memory to avoid memory leaks. At the end of each action for ``variable_declaration``, release the memory allocated for ``T_ID``:

    .. code-block:: cpp
       :force:

       delete $2;

    Errors that ``break`` out of the action (Shown in the example on step 16), should release the memory before the break.

|

19. Syntax errors are sequences of tokens that do not match any rule in the grammar. In this case, the parser itself needs to release memory allocated in the scanner. This is done with the following directive (just add it below the ``%union`` directive):

    .. code-block:: cpp
       :force:

       %destructor { delete $$; } <union_string>

    Adding this directive will generate several bison warnings on rules using ``T_ID`` but for which no action has been written:

    .. code-block:: shell

      bison -vd gpl.y
      gpl.y:297.17-20: warning: unused value: $2 [-Wother]
        297 |     object_type T_ID parameter_list_or_empty
            |                 ^~~~
      gpl.y:298.19-22: warning: unused value: $2 [-Wother]
        298 |     | object_type T_ID T_LBRACKET expression T_RBRACKET
            |                   ^~~~
      gpl.y:354.27-30: warning: unused value: $3 [-Wother]
        354 |     T_FORWARD T_ANIMATION T_ID T_LPAREN animation_parameter T_RPAREN
            |                           ^~~~
      gpl.y:359.17-20: warning: unused value: $2 [-Wother]
        359 |     object_type T_ID
            |                 ^~~~
      gpl.y:370.17-20: warning: unused value: $2 [-Wother]
        370 |     T_ANIMATION T_ID T_LPAREN object_type T_ID T_RPAREN
            |                 ^~~~
      gpl.y:370.43-46: warning: unused value: $5 [-Wother]
        370 |     T_ANIMATION T_ID T_LPAREN object_type T_ID T_RPAREN
            |                                           ^~~~
      gpl.y:474.21-24: warning: unused value: $3 [-Wother]
        474 |     | T_ID T_PERIOD T_ID
            |                     ^~~~
      gpl.y:475.54-57: warning: unused value: $6 [-Wother]
        475 |     | T_ID T_LBRACKET expression T_RBRACKET T_PERIOD T_ID
            |                                                      ^~~~

    To silence these warnings, add a *temporary* action that "uses" ``T_ID``. For example, the first two warnings complain about ``$2`` on lines 297 and 298 of ``gpl.y``. Here are the temporary actions to add:

    Original:

    .. raw:: html

        <pre>object_declaration:
            object_type T_ID parameter_list_or_empty
            | object_type T_ID T_LBRACKET expression T_RBRACKET
            ;</pre>

    Temporary action added:

    .. raw:: html

        <pre>object_declaration:
            object_type T_ID parameter_list_or_empty <span style="font-weight:bold;color:DarkRed">{$2; /*CHANGE*/}</span>
            | object_type T_ID T_LBRACKET expression T_RBRACKET <span style="font-weight:bold;color:DarkRed">{$2; /*CHANGE*/}</span>
            ;</pre>

|

20. Finally, dynamic memory is allocated for ``T_STRING_CONSTANT``. Since this constant isn't used in this project, add a temporary action to release (``delete``) it in the ``primary_expression`` production:

    .. code-block:: cpp
       :force:

       primary_expression:
         ...
         | T_STRING_CONSTANT { delete $1; /*CHANGE*/}

|

21. Again, use ``memvalidate.bash`` to ensure all tests are free of memory leaks. Use ``valgrind`` directly on individual tests (e.g., ``valgrind ./gpl tests/t015.gpl``) to gain information on the location of leaked memory.

22. The code should compile in its entirety without any warnings. If there are warnings, seek help to eliminate them.

===============
Version History
===============

Ver. 29 September 2020

* Typo corrrection, memtest -> memvalidate
* Added paragraph for suppressing switch statement warnings.
* Added final item to encourage removal of switch warnings. 
* Updated T_ID warnings & grammar in example.

Ver. 26 September 2020

* Typo corrrection, STING -> STRING

Ver. 12 September 2020

* Minor changes to the prose.

Ver. 16 August 2020

* Corrected incorrect reference to previous step on #18.

Ver. 2 May 2020

* Restructured document for use with Sphinx markup

Ver. 2 March 2020

* Added ``virtual`` to ``Symbol::~Symbol``

Ver. 22 October 2019

* Corrected typo in item #14

Ver. 7 October 2019

* Removed unnecessary sentence from ``Table_handler::instance()`` description about checking for empty vector.

Ver. 3 October 2019

* In code testing directions, corrected method of accessing ``Table_handler``.

Ver. 29 September 2019

* Made some Symbol member functions constant

Ver. 24 September 2019

* Added mention of ``-g`` option.

Ver. 23 September 2019

* Minor change to Manifest

Ver. 22 September 2019

* Enhancements throughout

Ver. 07 April 2019

* Expanded the writeup on item #14

Ver. 01 April 2019

* Enhanced item 15, added memtest.bash verbiage.

Ver. 25 March 2019

* Corrected typo: p1 to p2

Ver. 11 March 2019

* Removed gpl.cpp from Manifest

Ver. 10 March 2019

* Added copy constructor and assignment operator ``=delete`` declarations to ``Symbol`` and ``Symbol_table`` class declarations to prevent unnecessary local copies of these objects from being created.
* Added missing ``const`` to ``operator<<()`` for ``Table_handler``.
* Corrected ``Table_handler::lookup`` description to call each ``Symbol_table::lookup`` (instead of each ``Table_handler::lookup``).


Ver. 7 March 2019

* Corrected ``T_CONSTANT_STRING`` to ``T_STRING_CONSTANT``

Ver. 26 February 2019

* Added ctor to Symbol, added blank line for formatting before union ``symbol_type``

Ver. 20 February 2019

* Initial version
