#############
Third Project
#############

Ver. 2 May 2021

=======
Summary
=======

For this third project, support is added for GPL expressions and variables.

========
Manifest
========

:``./src/p3/Constant.h``:             Class for storing integer/double/string constants
:``./src/p3/Constant.cpp``:           

|

:``./src/p3/Expression.h``:           Class for managing expressions
:``./src/p3/Expression.cpp``:

|

:``./src/p3/tests``:                  Testing suite directory, 200 tests.

===================
Project description
===================

1.  Expand the tarball. Copy the files over from ``p2`` into ``p3``:

|

2.  Assume a .gpl file with::

        int x=682;

    In the previous project, integer initialization was hard-coded to ``42``. Begin this project by coaxing GPL to correctly initialize an integer with its ``optional_initializer`` from the ``variable_declaration:`` production (in this example, 682). A quick look at the scanner (``gpl.l``) reveals that integers generate the ``T_INT_CONSTANT`` token for the grammar.  In the parser (``gpl.y``) this constant is found in the ``primary_expression:`` production. Wrap the value passed from the scanner into an ``Integer_constant`` object:

    .. raw:: html

        <pre>primary_expression:
            ...
            | T_INT_CONSTANT <span style="font-weight:bold;color:DarkRed;">{ $$=new Integer_constant($1);}</span></pre>

    The type for ``primary_expression`` token must also be specified::

        %type <union_expression_ptr> primary_expression

    As well as adding adding the new type to the union::

        %union {
          ...
          const Expression* union_expression_ptr;
        };


    To the set of includes near the top of ``gpl.y`` add ``#include "Constant.h"``

    Compiling reveals *type clash* warnings in those rules of the ``primary_expression:`` production that have no action. To silence each, add an action to assign a ``nullptr`` to ``$$`` (be sure to add a comment to indicate the code is temporary).

    .. raw:: html

        <pre>primary_expression:
                T_LPAREN expression T_RPAREN <span style="font-weight:bold;color:DarkRed;">{$$=nullptr; /\*CHANGE\*/}</span>
                | variable <span style="font-weight:bold;color:DarkRed;">{$$=nullptr; /\*CHANGE\*/}</span>
                <span style="font-style:italic;">etc.</span>
        </pre>

|

3.  Compile. Though we haven't finished the task of integer initialization, the project should successfully compile current progress without warnings.

|

4.  The ``primary_expression`` token is a rule in the ``expression:`` production. Add an action to this rule:

    .. raw:: html

        <pre>
            expression:
                primary_expression <span style="font-weight:bold;color:DarkRed;">{$$=$1;}</span>
                ...</pre>

    As was done with ``primary_expression``, add a ``%type`` directive for ``expression``.
    
    Add ``{$$=nullptr; /*CHANGE*/}`` to the remaining rules for the ``expression:`` production to eliminate compiler warnings.

|

5.  Compile and confirm no warnings or errors.

|

6.  Write actions for the two rules of the ``optional_initializer:`` production. The ``empty`` rule should assign ``$$=nullptr;``. Note that in this case, ``$$=nullptr;`` is the proper action for ``empty`` (don't add a "CHANGE" comment). Don't forget to add a ``%type`` directive for ``optional_initializer``.

|

7.  Compile and confirm no warnings or errors.

|

8.  For the ``variable_declaration:`` production, modify the switch statement in first rule's action. For now, just focus on the case when ``simple_type`` is an integer.

    If ``optional_initializer`` is empty (e.g., ``int x;``) it will contain ``nullptr``.  

    If ``optional_initializer`` is not empty (e.g., ``int x=498;``) it will contain an ``Expression*``.  
    
    For an empty ``optional_initializer``, create a ``Symbol`` with a default value: ``0``.  Otherwise, obtain a ``Constant*`` from the ``Expression*`` by calling ``evaluate()``. Then create a ``Symbol``, initializing it with a value obtained from the ``Constant*`` by calling  ``as_int()``.

|

9.  Confirm that a simple program initializes integer values correctly::

        int abc;     //should be initialized to 0
        int def=293; //should be initialized to 293


|

10. Integrate similar changes for ``double`` and ``string`` (empty initialization defaults to ``0.0`` and the empty string ``""`` respectively). This requires changing the switch statement's other cases in the first rule of the ``variable_declaration:`` production. Also required are changes to the ``primary_expression:`` production that are comparable to that made to accommodate integer constants.

This is sufficient for the first six tests to pass. However, tests 7-9 will fail. Integer variables are being declared, but initialized with doubles and strings. Likewise, a double variable is being initialized a string. Find the expected error in ``error.cpp``. Enhance the action for ``variable_declaration:``'s first rule with a ``try/catch`` block to catch and report the error.

|

11. Ensure the first nine tests (``tests/t001.gpl`` - ``tests/t009.gpl``) pass.

|

12. Now is a good time to eliminate memory leaks with current progress.

    1. Prior to the scanner returning ``T_STRING_CONSTANT`` its semantic value was set by dynamically allocating a ``string``. The parser's action for ``T_STRING_CONSTANT`` allocates a ``String_constant``, initializing it with the semantic value. Once the scanner-allocated string's value has been used it is no longer required and should be released (``delete $1;``). This had been in-place as part of the previous project but may have been inadvertently removed when updating to create the ``String_constant``. Be sure the memory is released *after* it is used to allocate ``String_constant``.

    2. The first rule of the ``variable_declaration:`` production is ``simple_type T_ID optional_initializer``. The dynamically allocated semantic values associated with the second and third symbols in this rule must be released once they are no longer needed. ``T_ID``'s semantic value is a string allocated in the scanner, and ``optional_initializer`` has a dynamically allocated ``Expression``. Release them after they've been used in the ``switch`` statement.

    3. Test for memory leaks on the first nine tests. Writing a loop at the shell prompt is a convenient way to test a range:

       .. code-block:: bash

                for i in {1..9}; do memvalidate.bash $i; done

       The output of ``valgrind`` (which ``memvalidate.bash`` uses) is helpful identifying the origin of leaked memory. The leak-check enables ``valgrind`` to report on the location of memory leaks:

       .. code-block:: sh

                valgrind --leak-check=full ./gpl tests/t003.gpl

|

13. Write the ``evaluate()`` and ``type()`` member functions for ``class Plus``. The ``type()`` function should return the resulting ``Gpl_type`` of the expression based on the resulting type of the operator's two operands.

     .. code-block:: cpp

        Gpl_type Plus::type() const
        {
          Gpl_type lht=lhs->type();
          Gpl_type rht=rhs->type();
          if(lht==STRING || rht==STRING)
            return STRING;
          if(lht==DOUBLE || rht==DOUBLE)
            return DOUBLE;
          return INT;
        }

        const Constant* Plus::evaluate() const
        {
          Gpl_type lht=lhs->type();
          Gpl_type rht=rhs->type();
          const Constant* lhs_constant=lhs->evaluate();
          const Constant* rhs_constant=rhs->evaluate();
          if(lht==STRING || rht==STRING)
            return ret(new String_constant(lhs_constant->as_string() + //<--addition
                                           rhs_constant->as_string()));
          if(lht==DOUBLE || rht==DOUBLE)
            return ret(new Double_constant(lhs_constant->as_double() + //<--addition
                                           rhs_constant->as_double()));
          return ret(new Integer_constant(lhs_constant->as_int() +     //<--addition
                                          rhs_constant->as_int()));
        }

    The ``ret()`` function is a wrapper function that prevents memory leaks. In all ``evaluate()`` functions, wrap dynamically allocated objects that are returned as shown above.

|

14. Modify the rule for addition in the expression production:

    .. code-block:: cpp
       :force:

        { $$=new Plus($1, $3); }

    This is sufficient for tests 10-14 to pass. 

|

15. Write the ``evaluate()`` and ``type()`` member functions for ``class Multiply``: 

     .. code-block:: cpp

        // DOUBLE and INT are the only valid types for multiplication
        Gpl_type Multiply::type() const
        {
          Gpl_type lht=lhs->type();
          Gpl_type rht=rhs->type();
          if(lht==DOUBLE || rht==DOUBLE)
            return DOUBLE;
          return INT;
        }

        const Constant* Multiply::evaluate() const
        {
          const Constant* lhs_constant=lhs->evaluate();
          const Constant* rhs_constant=rhs->evaluate();
          if(lhs->type()==DOUBLE || rhs->type()==DOUBLE)
            return ret(new Double_constant(lhs_constant->as_double() * //<--multiply
                                           rhs_constant->as_double()));
          return ret(new Integer_constant(lhs_constant->as_int() *     //<--multiply
                                          rhs_constant->as_int()));
        }

    .. note::
        
        For the binary and unary operators, the member functions ``type()`` and ``evaluate()`` assume the operand types are valid. The code above assumes that both ``rhs`` and ``lhs`` are either ``INT`` or ``DOUBLE`` (A ``STRING`` is an illegal type with multiplication). The valid-type assumption is safe because type-checking is done while the GPL code is being parsed. If either operand is an invalid type, the error is reported and ``new Integer_constant(0)`` is created instead of ``new Multiply($1, $3)``.

|

.. sidebar:: Operator type-checking

    As noted above, actions coded for each operator require validating types. A technique for using templates will be presented in class for dramatically reducing the copy/paste/modify that is otherwise required for type-checking the operators.

16. The first 94 tests facilitate developing the binary and unary operators. Run each test individually. 
    
   
    Use the contents of the test's GPL file ``tests/tXXX.gpl`` to determine the operator being tested. Write and debug the class associated with tested operator before proceeding to the next test. Don't move on until your code passes the first 94 tests.

    For several operations (e.g., ``random``, ``floor``, boolean operations) consult the GPL manual for acceptable usage and error conditions. Some operations behave differently than in C/C++.

    At the end of this writeup are two brief sections on division/modulus and squashing memory leaks. Pause now and read through those sections. It is good information to have while completing the remaining work.

|

17. Add actions to the remaining rules for the ``primary_expression:`` production except for ``variable`` (that will be done later).

|

18. Undo the change made in the previous project to the array rule in the ``variable_declaration:`` production.

    Original:

    .. raw:: html

        <pre>variable_declaration:
           ...
           | simple_type  T_ID T_LBRACKET <span style="font-weight:bold;color:green;">T_INT_CONSTANT</span> T_RBRACKET</pre>

    Revised:

    .. raw:: html

       <pre>variable_declaration:
          ...
          | simple_type  T_ID T_LBRACKET <span style="font-weight:bold;color:DarkRed;">expression</span> T_RBRACKET</pre>



    Change ``$4`` in the action for that rule to get the code to compile again. The $4 parameter is now an expression that needs something like

    .. code-block:: cpp
       :force:

       $4->evaluate()->as_int()

    Additional code will be required; an error is reported if the expression isn't an integer. Tests 95 and 96 are helpful here.

|

19. In preparation for the next step, write two member functions for ``Symbol``::

                const Constant* Symbol::as_constant() const          //Symbol value is not an array
                const Constant* Symbol::as_constant(int index) const //Symbol value is an array

    These functions create and return an ``(Integer|Double|String)_constant`` object matching the symbol's type, initialized to the symbol's value.  For the array version, return ``nullptr`` if ``index`` is out-of-bounds. 

    If ``type`` contains something other than ``INT``, ``DOUBLE``, or ``STRING``, then ``throw type;``. This exception becomes relevant in the next project when game objects (e.g. ``RECTANGLE``) are introduced as additional types.


|

20. Up until now only the declaration of variables has been handled. The ``variable:`` production handles variables used in expressions:

    .. raw:: html

      <pre>5 + <span style="font-weight:bold;color:green;">myvar</span> * 8 / <span style="font-weight:bold;color:green;">anothervar[3]</span></pre>
    
    Add a ``Variable`` class to manage variables in expressions. The ``Variable`` class stores the symbol's name and retrieves the symbol as needed via a ``lookup`` call to ``Table_handler``::

        class Variable : public Expression {
          public:
            // initialize unused array_index_expression to nullptr
            Variable(const std::string& symbol_name);

            // initialize array_index_expression to index_expr
            Variable(const std::string& symbol_name, const Expression* index_expr);

            virtual const Constant* evaluate() const;
            virtual Gpl_type type() const;
            virtual ~Variable();

            Variable(const Variable&) = delete;
            Variable& operator=(const Variable&) = delete;
          protected: //allow derived class access

            std::string symbol_name;
            const Expression* array_index_expression;
                
            //The symbol() function eases access to the symbol
            //use it within Variable's member functions
            std::shared_ptr<Symbol> symbol()
            { return Table_handler::instance().lookup(symbol_name); }
            std::shared_ptr<Symbol> symbol() const
            { return Table_handler::instance().lookup(symbol_name); }
        };


    1. ``Variable::evaluate()`` returns ``ret(new Integer_constant(0))`` if ``symbol_name`` is the empty string. Otherwise call ``Symbol::as_constant``.  

       Call the appropriate ``Symbol::as_constant`` function written in the previous step. If the variable is not an array, call and return the ``as_constant()`` function that takes no arguments. Be sure to wrap the return value in the ``ret()`` function before returning it to the caller of ``Variable::evaluate()``. 

       If the variable is an array, evaluate the ``array_index_expression`` as an integer and pass the integer to ``Symbol::as_constant(int)```.  If it returns ``nullptr`` due to an out-of-bounds index, ``Variable::evaluate()`` should report the error and return the symbol's first array element instead.

    |

    2. ``Variable::type()`` returns ``INT`` if ``symbol_name`` is empty. Otherwise return the type of the symbol.

    |

    3. ``Variable::~Variable()`` releases ``array_index_expression``.


|

.. topic:: Parsing and the ``evaluate()`` function

   For this project and the next project the GPL programs used as tests are parsed but not executed. File ``gpl.y`` contains the grammar and actions taken `while the GPL program is being parsed, not executed`. 

   The ``evaluate()`` functions are intended to be called `while the GPL program is running` not while it is being parsed. Statements are executed and expressions are evaluated while the program runs. 

   The big exception to this intended use of ``evaluate()`` is when variables are declared (and when game objects are declared in the next project). Variable declarations have optional initializers and arrays have sizes. These must be determined while the program is being parsed. The tasks in this project have relied heavily on `evaluate()` because of its role in evaluating optional initializers and array sizes. 

   For grammar rules not part of the `variable_declaration:` production (and not part of `object_declaration:` in the next project) avoid the use of ``evaluate()``. Find a different way. Calling ``evaluate()`` while parsing is wrong.
   
   
|

21. Add actions to the first two rules of the ``variable:`` production. The actions for the first two rules ``lookup`` symbol from ``Table_handler`` to validate it. If valid, create a ``Variable`` object. Recover from parse errors in the ``variable:`` production by creating ``new Variable("")``. The third and fourth rules of the ``variable:`` production are implemented in a future project.

    Configure the type of the ``variable:`` production as ``Variable*``, not ``Expression*``::

      %type <union_variable_ptr> variable

    Enhance ``%union`` as well.


|

22. Ensure all rules for the ``primary_expression:`` production have valid actions. The ``variable`` rule simply assigns the previously-created ``Variable`` object to the lefthand side of the production.

|

Note on parsing division and modulus
------------------------------------

The division and modulus operators do not check for divide-by-zero errors while the GPL program is running. However, a check is done while the program is being parsed. The function ``Error::runtime()`` returns false if the program is parsing (not yet running). Use this function to add divide-by-zero error-checking in your division and modulus ``evaluate()`` functions. For example::

    double rhs_val=rhs_constant->as_double();
    if(!Error::runtime() && rhs_val==0.0)
    {
      Error::error(Error::DIVIDE_BY_ZERO_AT_PARSE_TIME);
      return ret(new Integer_constant(0));
    }

Tips for eliminating memory leaks
---------------------------------

Any token assigned a ``%type`` directive of ``union_string`` has a semantic value that is dynamically allocated and needs to be released after it is used. For example, in the ``variable:`` production, ``T_ID`` should be released in the first rule after:

1. generating an error because the variable is undeclared

2. generating an error because the variable is erroneously used as an array (``int a; a[3];//error!``)
   
3. creating a ``Variable`` object

If a binary operator object (Plus, Divide, etc.) is successfully created, the right-hand-side and left-hand-side expression pointers will be managed and released by the created operator object. However, if there is an error (e.g. ``3/"apple"``), then the expression pointers must be released when the error is detected. Similarly for the unary operators.

The ``optional_initializer`` in the first rule of the ``variable_declaration:`` production must be released after it is used. Similarly, in the second rule of the ``variable_declaration:`` production, release/delete ``expression`` (``$4``).

===============
Version History
===============

Ver. 2 May 2021

* Elaborated on ``Variable::evaluate()``, describing that both array variables and non-array variables must be handled.

Ver. 8 Dec 2020

* Added an explanation of the relationship between parsing, executing, and the ``evaluate()`` function.

Ver. 4 Nov 2020

* Added ``const`` to the ``Expression*`` in Variable

Ver. 22 Oct 2020

* More test number corrections in writeup and modest text corrections

Ver. 16 Oct 2020

* Added five tests early on to exercize error-checking and compiler recovery for bad types provided to the optional initializer.
  Adjusted some test numbers mentioned in directions.

Ver. 3 May 2020

* Restructured document for use with Sphinx markup

Ver. 12 March 2020

* Added verbiage for ``ret()`` and ``Error::runtime()``

Ver. 2 March 2020

* Removed need to change directory of valgrind.
* Added const to evaluate functions, ``Symbol::as_constant`` return type, and Constant pointers.

Ver. 31 October 2019

* Changed ``Variable`` to use symbol's name.

Ver. 22 October 2019 - B

* Added direction on Variable's destructor.

Ver. 22 October 2019 - A

* Reworded a coding comment

Ver. 22 October 2019

* Reorganized the material around the ``Variable`` class

Ver. 21 October 2019

* Clarified text in the section that discusses the ``Variable`` class.

Ver. 20 October 2019

* Clarified and revised some items based on redesigned architecture in downstream projects.

Ver. 15 October 2019

* Typo: 99 changed to 682

Ver. 1 October 2019

* Grammar errata removed (no longer needed)
* Reviewed and revised entire document.

Ver. 9 May 2019

* Added text for the variable production to create a variable object on parser errors.
* Added direction to ``%type`` the variable production as a variable pointer, not an expression pointer.

Ver. 7 April 2019

* Revised the tips for managing memory.

Ver. 5 April 2019

* Fixed verbiage on item #13

Ver. 2 April 2019

* Reordered steps--moved first errata item to #11

Ver. 1 April 2019

* Corrected Multiply::type()
* Added steps 12 and beyond

Ver. 25 March 2019

* Initial version
