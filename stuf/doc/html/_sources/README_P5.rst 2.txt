#############
Fifth Project
#############

Ver. 19 Apr 2021

=======
Summary
=======

Event blocks (e.g., `on leftarrow {...}`) and statements (loops, if/else, etc.) are implemented

========
Manifest
========

        :``Locator.cpp``:                     Class that maps a symbol's data to an lvalue
        :``Locator.h``:

        |

        :``./src/p5/tests``:                  Testing suite directory, 100 tests.

===================
Project description
===================

1.  Copy the files over from ``p4`` into ``p5``:

.. topic::  Debugging projects 5 and 6

    This project introduces event blocks: statements that are executed when a key is pressed. For the automated testing suite to send keystrokes to the ``./gpl`` executable,
    a ``-stdin`` option can is used. The tests that require keystrokes have an additional file for this purpose (e.g., ``t007.keypresses``). To run individual tests manually:

      .. code-block:: sh
         
         # without valgrind
         cat tests/t007.keypresses | ./gpl -stdin tests/t007.gpl

         # with valgrind
         cat tests/t007.keypresses | valgrind ./gpl -stdin tests/t007.gpl 

|

2.  In order to get your working p4 project to compile in the p5 directory, add the following include statement, function declaration, and function definition to your Symbol class::

        //Symbol.h
        #include "Locator.h"
        ...
        std::shared_ptr<Locator> as_lvalue() const;

        //Symbol.cpp
        std::shared_ptr<Locator> Symbol::as_lvalue() const
        {
          return nullptr; /*CHANGE*/
        }

    This function stub will be fleshed out in a later step. Confirm your project still compiles.

|

.. topic::  The hierarchy of Statement classes

    Statments are designed to be similar to expressions. 

    Class hierarchy:

    * Expression classes are derived from an abstract base class, `Expression`. 
    * Statement classes (print, the for-loop, assignment, etc.) are derived from an abstract base class, `Statement`.

    Object organization:

    * Expressions group into trees, linked by each operator's operands. Each tree is *evaluated*.
    * Statements group into linked lists, linked by a "next" pointer each object inherits. Each list is *executed*.

    The abstract base class ``Statement``::

        class Statement {
          public:
            void append_statement(Statement* atend);
            Statement():next(nullptr) {}
            virtual void execute() const=0;
            virtual ~Statement() {delete next;}
          protected:
            Statement* next;
        };

    ``Statement* next``
      Enables statements to be chained together in a linked list. For example, this GPL code:

      .. code-block:: cpp
         :linenos:

         turret.visible = false;
         plane.x += 3;
         for(i=0; i<5; i+=1)
         {
           pyramid[i].user_int1=random(10);
           print("Got 'em!")
         }
         if(angle > 30)
           turret.visible = true; 

      This code sample generates a linked list of 4 statements (lines 1, 2, 3, and 8). Each statement may itself link to additional statements. The for-loop on line 3 has a pointer to another list of statements linking to lines 5 & 6.

    ``append_statement(Statement*)``
      Adds a statement to the end of the existing list. In the GPL grammar, a pointer to the head of the list is passed around the various productions. When an additional statement needs to be added, simply invoke this function on the head statement and the new statement will be appended at the end of the list.

    ``virtual void execute() const``
      Every class derived from ``Statement`` overrides this function with the behavior unique to the derived class.

|

3.  As a first foray into statements, add support for GPL's ``print`` statement. Example usage::

         print("value=" + 42 + ", fl=" + fl);

    Placing this example on line 17 of a GPL program produces output similar to::

        print[17]: value=42, fl=37.5

    Derive a ``Print`` class from ``Statement``::

        class Print : public Statement {
          public:
            Print(int line_number, const Expression* argument) : line_number(line_number), argument(argument){}
            virtual void execute() const;
            ~Print();
          private:
            int line_number;
            const Expression* argument;
        };


    ``Print(int line_number, const Expression* argument)``
      Is initialized with the global ``line_count`` variable and an expression to be printed when ``execute()`` is called.

    ``virtual void execute() const``
      Prints a line in the format shown in the example above. The expression is evaluated as a string for printing to stdout (cout).
       .. note::

          Statements are organized into linked lists, the last line of every ``execute()`` function must be: 

          ``if(next!=nullptr) next->execute();``

    ``~Print()``
      Release the object's data: ``delete argument;``

|

4.  Integrate the ``Print`` class into the grammar actions.  A ``Print`` object is created in the action of the ``print_statement:`` production:

    .. code-block:: cpp
       :force:

        { $$=new Print($1, $3); }

    Ensure the address of this object is passed through the ``statement``: production, to the ``statement_list:`` production. The action for the first rule of ``statement_list:`` appends the pointer to an already existing list of statements:

    .. code-block:: cpp
       :force:

        { $1->append_statement($2); $$=$1;}

    If a statement list does not yet exist, the second rule of ``statement_list:`` assigns the address of the first statement to ``$$``.

    Ensure the project compiles.  Add the appropriate ``%type`` directives and modify ``%union``. As with previous projects, "type clash" warnings can be silenced with ``{ $$=nullptr; /*CHANGE*/ }``.

|

5.  Add the functionality required of event handler blocks for the print statement (and statements generally). An event handler is a block of GPL statements that executes on a keystroke::

        on space
        {
          print("You hit the spacebar.");
          print("That's proof that you're learning computers!");
        }

        on space
        {
          print("Each keystroke may have more than one event handler.");
        }

    The head ``Statement`` pointer propagates:

      from the ``statement_list:`` production,

      through the ``statement_block:``  production,

      through the ``statement_or_block_of_statements:``  production,

      to the ``on_block:`` production. 
      
      Don't provide an action to the ``on_block:`` production yet. 

    The ``statement_block:`` production has two rules. The second rule represents an empty block (no GPL code). For this empty block create a ``NullStatement`` object:

    .. code-block:: cpp
       :force:

        statement_block:
            T_LBRACE statement_list T_RBRACE {$$=$2;}
            | T_LBRACE T_RBRACE {$$=new NullStatement;}

|

.. topic:: The Event_manager class

    In C++ code, each handler block is nothing more than a linked list of ``Statement`` objects. The sequence of productions outlined above build that linked list and deliver it to the ``on_block:`` production. The ``on_block:`` production registers the "block" of statements with an ``Event_manager`` class. The ``Event_manager`` class is a singleton class like the ``Table_handler`` class. An incomplete version of the class has been provided::

            class Event_manager
            {
              public:
                static Event_manager& instance();
                ~Event_manager();

                void execute_handlers(Window::Keystroke keystroke) const;
                void add_handler(Window::Keystroke, const Statement*);

                Event_manager(const Event_manager&) = delete; 
                const Event_manager &operator=(const Event_manager&) = delete;

              private:
                //add private data to contain the linked lists of statements.
                Event_manager(){};
            };


    `private data`
      Contains the head pointers of each keystroke. Each keystroke may have multiple event handlers. That is, each keystroke may be associated with more than one linked list. ``Event_manager`` manages these lists.

    ``void add_handler(Window::Keystroke,  const Statement*)``
      Called in the ``on_block:`` production rule's action. Adds the head ``Statement*`` to the private data.
    
    ``void execute_handlers(Window::Keystroke keystroke) const``
      Loop through the private data, calling ``Statement::execute()`` on each head ``Statement*`` associated with the input ``keystroke``. This function is called automatically by the provided GPL classes.

    ``~Event_manager()``
      Release the linked lists. Deleting the head of each list is sufficient.

|

6.  Define a data structure to manage the head pointers of each keystroke. A vector of vectors, a map of vectors, or any structure of your choosing is acceptable.  The ``Window::Keystroke`` enumeration can be found in the header file for ``Window``. It lists the enumerations for the possible keystroke events. The enumerated values are sequential, and ``Window::NUMBER_OF_KEYS`` provides a total count, making these enumerations usable as indices in a vector. They can also be used as keys into a map.

    Implement the ``add_handler()``, ``execute_handlers()`` and destructor for ``Event_manager``.

|

7.  The ``keystroke:`` production has a rule for each event key. Map the event enumerations (``Window::SPACE``, ``Window::UPARROW``, etc.) to these rules.

|

8.  Add an action to the ``on_block:`` production that calls ``Event_manager::add_handler`` to register the linked list (the ``statement_block``) associated with the keystroke.

|

9.  Compile the project and confirm that it passes the first six tests (``t001.gpl`` through ``t006.gpl``). The statement object should still be created even if there is a parse-error. (Optionally, a ``NullStatement`` object could be created on error but it makes no difference.) In the presence of errors, the GPL interpreter continues building the ``Statement`` linked list as it parses subsequent statements. However ``Statement::execute()`` is never invoked if there have been parse errors so any statement, valid or invalid, may be created on error.

|

.. topic:: Assignment statements. 

    The GPL grammar supports several types of assignment::

        assign_statement:
              variable T_ASSIGN expression
            | variable T_PLUS_ASSIGN expression
            | variable T_MINUS_ASSIGN expression
            | variable T_PLUS_PLUS
            | variable T_MINUS_MINUS

    Examples of GPL code matching these grammar rules::

        my_rect.x += 30;
        var = 9.2;
        my_triangle.height++;
        my_circles[3].radius -= 2;

    Each of these rules modifies the ``Symbol`` found in private data of the ``Variable`` object on the lefthand side of the operator.  For example: ``my_rectangle[2].radius+=12;`` adds twelve to the radius attribute of the third rectangle object in a rectangle array. The array is managed by the "my_rectangle" symbol. The ability to modify `Symbol` values is provided by the `Locator` classes.

|

.. topic:: Locator class hierarchy

    The ``Locator`` classes provide functionality that is complementary to the ``Constant`` classes:

    Purpose:

    * The ``Symbol`` class creates ``Constant`` objects to allow read-only access to ``Symbol`` data. 
    * The ``Symbol`` class creates ``Locator`` objects to allow lvalue (modifiable) access to ``Symbol`` data. 

    Hierarchy:

    * The ``Constant`` class hierarchy consists of four derived classes: ``Integer_constant``, ``Double_constant``, ``String_constant``, and ``Game_attribute_constant``.
    *  The ``Locator`` class hierarchy consists of four derived classes: ``Integer_locator``, ``Double_locator``, ``String_locator``, and ``Game_attribute_locator``. 

    Interface::

          // Constant classes
          int         Constant::as_int()    const; // return Symbol's value as int
          double      Constant::as_double() const; // return Symbol's value as double
          std::string Constant::as_string() const; // return Symbol's value as string

          // Locator classes
          void        Locator::mutate(int);                 // Change Symbol's int value
          void        Locator::mutate(double);              // Change Symbol's double value
          void        Locator::mutate(const std::string& ); // Change Symbol's string value

    The ``Symbol`` class has four ``as_constant`` member functions that generate and return ``Constant`` objects. Write four member functions for the ``Symbol`` class that generate and return ``Locator`` objects::

        //non-array: int, double, or string
        std::shared_ptr<Locator> as_lvalue();                                             

        //array element: int, double, or string
        std::shared_ptr<Locator> as_lvalue(int index);                                    

        //non-array: game object
        std::shared_ptr<Locator> as_lvalue(           const std::string& attribute_name); 

        //array element: game object
        std::shared_ptr<locator> as_lvalue(int index, const std::string& attribute_name); 

    The ``Symbol::as_(int|double|string)`` functions create ``Constant`` objects using ``new`` and return the raw pointers. This eases the incorporation of ``Constant`` objects into bison's framework. ``Locator`` objects are not passed around by bison's grammar. Therefore use ``std::shared_ptr`` and ``std::make_shared`` in the ``Symbol::as_lvalue`` functions to generate and return ``Locator`` objects. The body of each, ``as_lvalue`` function is nearly identical to its ``as_constant`` equivalent. 

|

10. The ``Variable`` and ``Member_variable`` classes each have an ``evaluate()`` member function that creates and returns a ``Constant`` object for use in expression evaluation. Write the complementary function, ``modify()`` for both ``Variable`` and ``Member_variable`` that returns a shared pointer to a ``Locator`` object. The body of ``modify()`` is almost identical to the body of ``evaluate()``. The ``modify()`` function should be virtual, just as the ``evaluate()`` function is.

|

11. The ``Assign`` class handles simple assignment. GPL Examples::

        var=9.2;
        my_triangle.skew = building_height / var;

    Derive an ``Assign`` class from ``Statement``::

        class Assign : public Statement {
          public:
            Assign(const Variable* lhs, const Expression* rhs) : lhs(lhs), rhs(rhs){}
            virtual void execute() const;
            virtual ~Assign();
          private:
            const Variable* lhs;
            const Expression* rhs;
        };

    ``virtual void execute() const``
      Obtain an lvalue from ``lhs`` using ``modify()`` and a constant from ``rhs`` using ``evaluate()``. Use lvalue's ``mutate()`` member function to assign it a new value. The type (``int``, ``double``, or ``string``) passed into ``mutate()`` depends on lvalue's type. That is, lvalue's type determines whether an ``int``, ``double``, or ``string`` is retrieved from rhs's constant.

    ``~Assign()``
      Release ``rhs``, and ``lhs``.

|

12. In the grammar, create an ``Assign`` object as the action in the first rule of the ``assign_statement:`` production. Review the grammar and ensure the pointer reaches the ``statement_list:`` production.

|

13. The project should now pass tests ``t007.gpl``, ``t008.gpl``, & ``t009.gpl``.

|

14. Implement the four remaining assignment statements: ``+=``, ``-=``, ``++``, and ``--``.

|

15. Ensure the project passes tests:

    * ``+=``,  ``t010.gpl`` and ``t011.gpl``
    * ``-=``, ``t012.gpl`` and ``t013.gpl``
    * ``++``,  ``t014.gpl`` and ``t015.gpl``
    * ``--``,  ``t016.gpl`` and ``t017.gpl``

|

16. Implement the ``if/else``, ``exit``, and ``for-loop`` statements. Consult the GPL manual for usage details. They have the following private data members:

    * class ``If``
        + Expression pointer
        + Statement pointer (for the if-clause)
        + Statement pointer (for the optional else-clause)
    * class ``For``
        + Statement pointer (initializer)
        + Expression pointer (condition, evaluates to int)
        + Statement pointer (incrementor)
        + Statement pointer (body of loop)
          
      The for-loop statement may have an empty initializer and/or incrementor. The ``For::execute()`` implementation will be easier if a ``NullStatement`` is used (instead of ``nullptr``) for these empty components.

    * class ``Exit``
        + line number
        + Expression pointer (evaluates to integer exit status)

    .. warning::

       Statements are organized into linked lists, the last line of every ``execute()`` function must be:

       ``if(next!=nullptr) next->execute();``


|

17. Initial statement-specific tests:

    :``if/else``: ``t020.gpl`` and ``t021.gpl``
    :``for-loop``: ``t022.gpl``, ``t023.gpl``, and ``t024.gpl``
    :``exit``: ``t025.gpl`` through ``t028.gpl``

|

18. Pass remaining tests.

===============
Version History
===============

Ver. 19 Apr 2021

* For ``print`` production action, corrected ``line_count`` to ``$1``.

Ver. 2 Dec 2020

* Added section on debugging.

Ver. 26 Nov 2020

* Corrected a p4 to p5 copy/paste error.

Ver. 14 Nov 2020

* Reword paragraph on handling errors.
* Added ``const`` on a couple member functions, data members, and input parameters

Ver. 9 Nov 2020

* Removed direction to extract tarball
* Enhanced get-to-compile direction
* Added ``statement_or_block_of_statements`` to the list of productions the pointer traverses.
* Changed `rhs` to `argument` in print statement

Ver. 5 May 2020

* Added Constant/Locator comments to code in "Locator class hierarchy" topic.

Ver. 3 May 2020

* Restructured document for use with Sphinx markup

Ver. 27 Apr 2020

* Added ``const`` to ``Assign::execute()`` function declaration

Ver. 5 Dec 2019

* Added sentence to ensure modify is made virtual

Ver. 27 Nov 2019

* Added for-loop note about using NullStatement instead of nullptr.

Ver. 18 Nov 2019

* Removed ``delete next;`` from ``Print::~Print()``

Ver. 13 Nov 2019

* Added NullStatement creation to 2nd rule of ``statement_block:`` production.

Ver. 12 Nov 2019-B

* Added stub Symbol::lvalue() so project 4 compiles in p5 directory

Ver. 12 Nov 2019

* ``Game_object_...`` to ``Game_attribute_...``

Ver. 11 Nov 2019

* Removed Statement class (now introduced at P4)

Ver. 31 Oct 2019

* Minor revision. Rewording, fixing typos.

Ver. 25 Oct 2019

* Major rewrite

Ver. 6 May 2019

* Initial version.
