#############
First Project
#############

Ver. 18 Oct 2021

=======
Summary
=======

For this first project, the tokens and grammar that comprise the Game Programming Language (GPL) will be integrated into files used by "flex" the scanner, (``gpl.l``) and "bison" the parser (``gpl.y``). 

========
Manifest
========

:``./doc/html/index.html``:           documentation for all projects in the course

|

:``./src/p1/gpl_scanner_skel.txt``:   Stub scanner file. Copy to gpl.l to use.
:``./src/p1/gpl_parser_skel.txt``:    Stub parser file. Copy to gpl.y to use.

|

:``./src/p1/tokens_for_bison.txt``:   Premade tokens to be added to gpl.y
:``./src/p1/grammar_for_parser.txt``:  Premade GPL grammar to be added to gpl.y
:``./src/p1/reg_exp_for_scanner.txt``:    List of tokens to be integrated into gpl.l

|

:``./src/p1/Makefile``:               Self-explanatory (I hope)
:``./src/p1/tests``:                  Testing suite directory, 18 tests.

|

:``./src/p1/gpl.cpp``:                main() function. Don't change.
:``./src/p1/error.h``:                Header for error reporting functions. Don't change.
:``./src/p1/error.cpp``:              Source for error reporting functions. Don't change.

|

:``./bin/gt``:                        run project tests
:``./bin/vde``:                       examine test differences in expected and actual stderr
:``./bin/vd``:                        examine test differences in expected and actual stdout
:``./bin/memvalidate.bash``:          Test for memory leaks using valgrind.



===============
Format of files
===============

.. code-block:: none
  
  Grammar file (.y extension)     |       Scanner file (.l extension)
  ---------------------------     |       ---------------------------
  %{                              |       %{
  Prologue (code)                 |       Prologue (code)
  %}                              |       %}
                                  |       
  Bison Declarations              |       Flex definitions
                                  |       
  %%                              |       %%
                                  |       
  Grammar Rules                   |       Scanner rules
                                  |       
  %%                              |       %%
                                  |       
  Epilogue                        |       Epilogue

===================
Project description
===================

1.  Copy the skeleton files over to ``gpl.y`` and ``gpl.l`` as a starting point

    .. code-block:: shell
        
       cp gpl_scanner_skel.txt gpl.l
       cp gpl_parser_skel.txt gpl.y

|

2.  Because the parser (``gpl.y``) generates the tokens that are included by the scanner (``gpl.l``) the parser should be built first. Begin by copying ``tokens_for_bison.txt`` into ``gpl.y`` just below the comment ``//tokens declared here``.

.. sidebar:: Semantic values

   The scanner passes semantic values to the parser. For example, when the scanner encounters ``3.14`` in a gpl program, it returns the token ``T_DOUBLE_CONSTANT`` to the parser and sets the token's semantic value to ``3.14``. There are three types of semantic value returned: ``int``, ``double``, and ``string*``. 
    
|

3.  Complete the ``%union`` declaration in ``gpl.y`` with the three types of semantic value, ``int``, ``double``, and ``string*``. Two of these three types have already been added for you. Add the third.
    
|

4.  Several of the parser's tokens have empty angle-braces ``< >``. These are the tokens that have semantic values found by the scanner. Inside the angle braces add the appropriate union member (e.g., ``union_int``) to specify the type of that token's semantic value.

.. sidebar:: Grammar actions

   We will spend much of the semester developing actions to accompany the grammar in ``gpl.y``. To assist testing this first project, three actions (``cout`` statements) have been added to the rules of the `primary_expression:` production. Those will be replaced in the next project.

|

5.  Copy ``grammar_for_parser.txt`` into the "Grammar Rules" section of ``gpl.y`` (see **Format of files** above). 

|

6.  Run ``bison -d gpl.y``. There should be 256 shift/reduce conflict warnings, but no errors.

|

7.  Copy the ``reg_exp_for_scanner.txt`` file into the "Scanner rules" section of ``gpl.l`` (see **Format of files** above).

|

8.  Each of the matched literal/keyword expressions in ``gpl.l`` should return the corresponding token that is defined in ``gpl.y``. For example::

      "int" {return T_INT;}
      ...
      "!=" {return T_NOT_EQUAL;}

    
|

9.  Append to the list of regular expressions an expression that matches a newline. The action should  increment ``line_count`` whenever a newline is encountered.  ``line_count`` is a global variable that keeps track of the current line number being analyzed by the scanner. The global declaration of this variable has been provided in the prologue of ``gpl.l``. 

|

10.  In GPL, the ``print`` and ``exit`` functions print the line number they appear on in their output. When their regular expressions are matched, set their semantic values by assigning to the appropriate union member from ``yylval``. For example::

      "print" {yylval.union_int = line_count; return T_PRINT; }

|

11.  Create the regular expressions and associated actions for the more complex tokens:

    * **integer constant** - A sequence of one or more digits (0 - 9). Place the value of the integer in the ``union_int`` field ``yylval``.
    * **double constant** - A sequence of one or more digits (0 - 9) that contains a period. May start with a period or end with a period (``.1``, ``1.``, ``1.1``, ``123.123`` are all legal). Place the value of the double in the ``union_double`` field of ``yylval``.  Ensure a period ``"."`` is not matched as a double.
    * **string constant** - Any sequence of characters enclosed in double quotes (``"one"``, ``"123"``, ``"one two three"`` are all legal). Dynamically allocate a new string for the found expression and assign it to the ``union_string`` field of ``yylval``. For example:  ``yylval.union_string=new string(yytext);`` Note that this example is incomplete. ``yytext`` contains the string *with* double quotes. Be sure to write C++ code in the action that strips the double quotes from the string.
    * **identifier** - Any letter (``a-z`` or ``A-Z``) or underscore followed by zero or more letters, underscores, and digits (``0-9``). Place the value of the string in the ``union_string`` field of ``yylval``. Dynamically allocate a new string for each token.
    * **comments** - Like the C++ ``//`` comment. Ignore everything to the end of the line
   
|

12. In ``gpl.y`` just below the ``%token`` declarations, add precedence declarations (e.g., ``%left, %right, %nonassoc``) for the following operators::

      *   /    &&   ||
      +   -   <   >   <=   
      %   >=   ==   !=


    Place operators with the same precedence on the same line. For example::

      %left T_PLUS T_MINUS
      %left T_MULTIPLY T_DIVIDE T_MOD

    Entries that appear earlier in the file have lower precedence. Shown above, addition and subtraction have lower precedence than multiplication, division, and modulus. Refer to an operator precedence table to ensure the correct order for the operators listed above. 
    Only set precedence for these specific operators. Precedence tables for C/C++ include many operators that are not part of GPL. Do not set precedence rules for operators found in a precedence table but absent from the above list.

|

13. Add precedence for the ``near`` (``T_NEAR``) and ``touches`` (``T_TOUCHES``) operators. They should be non-associative (directive written as: ``%nonassoc``) and have the highest precedence (appear last in the current list of precedence rules).

|

14. The unary operator ``!`` (``T_NOT``) is also non-associative. Add it at the correct level of precedence. Another non-associative operator to add is the unary minus sign for negation (e.g., ``-var``). The problem is the the token ``T_MINUS`` is already being used to specify precedence for the binary subtraction operator. To get around this, create/name a precedence for negation (at its appropriate level), like this::

      %nonassoc UNARY_OPS

    Apply this new precedence token to the existing grammar rule for negation by appending a ``%prec`` directive at the end of the rule:

    .. raw:: html
        
        <pre>expression:
        ...
        | T_MINUS expression <span style="font-weight:bold;color:DarkRed;">%prec UNARY_OPS</span>

    .. only:: latex

       ::
        
        expression:
            ...
            | T_MINUS expression %prec UNARY_OPS

|

15.  Run ``bison -d gpl.y`` again. Adding the precedence directives above eliminates nearly all shift/reduce conflicts. There will now only be one remaining shift/reduce conflict warning, and no errors.

|

16. The if-statement production has the last remaining shift/reduce conflict. The conflict is between if statements with else clauses and if statements without else clauses. To remove the conflict, the two types of if-statements must be given different precedence levels. The if-else statement should have a higher precedence than the if-statement without an "else". Add two ``%nonassoc`` precedence directives: one for the existing ``T_ELSE`` token and a new token for the if without an else, ``IF_NO_ELSE``:

    Ensure that the precedence of ``T_ELSE`` is  higher than ``IF_NO_ELSE``

    As was done with the ``UNARY_OPS``, add a ``%prec`` directive at the end of the production rule for an if-statement that does not have an else clause.

|

17. Run ``make`` to build the project. Once built successfully (and with no shift/reduce conflicts), use ``gt`` to run the tests. Information on using ``gt`` can be found below. Be sure to read `Tips for debugging errors reported by gt` below as well.


===============
Troubleshooting
===============

Use ``bison -d gpl.y`` to check for shift/reduce conflicts. If there are any, there is more work to be done. Adding the verbose flag: ``bison -vd gpl.y`` will produce ``gpl.output`` which can be helpful in debugging ``gpl.y``.

=======
Testing
=======

Each project directory has a ``tests`` directory. Each test is identified by a number and has three files associated with it (where NNN is a number 001, 128, etc.)

* ``tNNN.gpl`` - The Game Programming Language code comprising the test
* ``tNNN.out`` - The standard output expected by the test
* ``tNNN.err`` - The standard error expected by the test

The ``bin`` directory has tools to aid in running the tests. To make your life easier, add the ``bin`` directory to your PATH. 

------
``gt``
------

Runs the ``tNNN.gpl`` files in the ``tests`` directory using your gpl executable. A ``results`` directory is created if it doesn't already exist. When ``gt`` runs a test it redirects your executable's ``stderr`` and ``stdout`` to files in the results directory. The ``gt`` utility then compares your program's ``stderr`` and stdout in the ``results`` with the expected ``stderr`` and stdout files in the ``tests`` directory. If they are not identical ``gt`` reports that the test does not pass. 

    Examples of usage:

    .. code-block:: bash

        gt      #run all tests that have not yet passed
        gt -all #force all tests to run even if passed previously
        gt 38   #run test 38

-------
``vde``
-------

For a given test, examines the differences between the test's expected ``stderr`` and your program's ``stderr``. For folks who don't use vim, type ``:qa`` to quit all editor windows and return to a shell prompt.

    Usage example (showing differences in stderr for test 13):

    .. code-block:: bash

        vde 13

------
``vd``
------

For a given test, examines the differences between the test's expected ``stdout`` and your program's ``stdout``. For folks who don't use vim, type ``:qa`` to quit all editor windows and return to a shell prompt.

    Usage example (showing differences in stdout for test 126):

    .. code-block:: bash

        vd 126 


========================================
Tips for debugging errors reported by gt
========================================

1. When ``gt`` complains that both ``stderr`` and ``stdout`` fail, the problem can usually be uncovered by looking at ``stderr``. Therefore, Examine the output of ``vde`` before taking a look at ``vd``.
2. Run the program manually! For example, for test 12: ``./gpl tests/t012.gpl``.
3. Examine the ``tNNN.gpl`` test file. This is vital. It's difficult to troubleshoot a problem if you don't know the gpl code that is being tested.
4. Many tests contain several lines of gpl code. Create a copy of ``tNNN.gpl`` and start commenting out lines of code to isolate the specific line in ``tNNN.gpl`` that is not behaving as expected. This is very important. Troubleshooting is the process of narrowing down possible causes.


===============
Version History
===============

Ver. 18 Oct 2021

* Added red coloring for ``%prec UNARY_OPS``

Ver. 5 Oct 2021

* Broke out and added new item for ``exit`` and ``print``.

Ver. 15 Sep 2020

* Reworded the step for unary negation and logical "not".

Ver. 14 Sep 2020

* Removed ``forward`` from the list of commands requiring a semantic value. Elaborated on why ``print`` and ``exit`` require line number as semantic value.

Ver. 11 Sep 2020

* Changed filenames of text files imported into ``gpl.y`` and ``gpl.l``.

* Modest reworking of some text, including separating into additional steps.

* Added step at end to make explicit the use of ``make`` and ``gt``.

Ver. 2 May 2020

* Restructured document for use with Sphinx markup

Ver. 27 Apr 2020

* Reworked to eliminate ``parser.h``

Ver. 6 Apr 2020

* Modest verbiage clarifications.

Ver. 11 Feb 2020

* Added information on the testing tools in the bin directory.

Ver. 16 Jan 2020

* Minor verbiage improvements

Ver. 3 Jan 2020

* Modified manifest

Ver. 14 Dec 2019

* Minor typographical changes.

Ver. 19 September 2019

* Added explanation of where ``near`` and ``touches`` appear in bison's precedence declarations.
* Rewrote last section on adding precedence to ``gpl.y``
* Revised Troubleshooting section at end.

Ver. 20 February 2019

* Added additional prose around operator precedence.
* Renamed starting gpl.l and gpl.y to avoid clobbering existing work if archive is updated and extracted in-place.

Ver. 19 February 2019

* Added troubleshooting section

Ver. 16 February 2019

* Minor typos and formatting

Ver. 10 February 2019

* Initial version
