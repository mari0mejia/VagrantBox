#############
Sixth Project
#############

Ver. 7 May 2021

=======
Summary
=======
The remaining GPL functionality is implemented:

    * Initialization and Termination blocks
    * Near and Touches operators.
    * Animation blocks

========
Manifest
========

        :``./src/p6/tests``:                  Testing suite directory, 31 tests.

===========
Description
===========

1.  Copy all files from ``p5`` into ``p6``.

.. sidebar:: Initialization and Termination blocks

   In addition to event blocks (code that is executed when a key is pressed) GPL has initialization blocks and termination blocks. As their names suggest, these blocks are executed prior to the start of, and after the end of the GPL program run. They are trivial to implement.

|

2.  The actions for the ``initialization_block:`` and ``termination_block:`` productions are nearly identical to the ``on_block:`` production. The only adjustment to the action of each of these blocks is the keystroke provided to the ``add_handler`` function. Use ``Window::INITIALIZE`` and ``Window::TERMINATE`` when calling ``add_handler`` for these two blocks.

|

3.  Enhance the GPL ``exit()`` call to support termination blocks. Revise ``Exit::execute()`` by executing termination block(s) prior to exiting the program:

    .. code-block:: cpp

      Event_manager::instance().execute_handlers(Window::TERMINATE);

|

4.  Pass the relevant tests

    * Initialization blocks, tests ``t001.gpl`` and ``t002.gpl``
    * Termination blocks, tests ``t003.gpl``, ``t004.gpl``, and ``t005.gpl``

.. sidebar:: ``near`` and ``touches`` operators.

   The ``near`` and ``touches`` operators are the only operators in GPL that operate on entire game objects. All other operators operate on integers, doubles, strings, and attributes of game objects (which are either int, double, or string). 
       
|

5.  To accommodate the ``near`` and ``touches`` operators, add a ``Game_object_constant`` class. The constant's ``Gpl_type`` is ``GAME_OBJECT``. Add a new virtual function to both the base class, ``Constant`` and the new ``Game_object_constant`` class::

      virtual const Game_object* as_game_object() const

|

6.  Once ``Game_object_constant`` has been written, Enhance the ``Symbol`` class to create and return ``Game_object_constant`` objects::

      Constant* Symbol::as_constant() const
      Constant* Symbol::as_constant(int index) const

    These two member functions create ``(Integer|Double|String)_constant`` objects and return them to the caller (typically, ``Variable::evaluate()`` calls these functions). Enhance these two functions to create and return ``Game_object_constant`` objects. Tip: In the switch statement of these two functions, use the ``Gpl_type`` of the actual object for the cases (``RECTANGLE``, ``CIRCLE``, etc.). Do *not* make a case for ``GAME_OBJECT``.

|

7.  Derive two additional Expression classes for the ``near`` and ``touches`` operators. Implement the ``evaluate()`` functions of these new expression classes by calling existing game object member functions provided to check for near & touches::

      int Game_object::near(const Game_object* obj) const;
      int Game_object::touches(const Game_object* obj) const;

    The functions return one and zero for true/false.

|

8.  Add the actions for the ``near`` and ``touches`` binary operators in the grammar. A properly-formatted call to the ``Bin_op_check`` template is sufficient.

|

9.  Pass tests ``t006.gpl``, ``t007.gpl``, and ``t008.gpl``.

|

.. topic:: Animation blocks

   Animation blocks are GPL blocks that have a structure modeled after C/C++ functions. For example:

   .. code-block::
      :linenos:

      //The "tri" identifier is unnecessary in this forward
      //declaration. If provided, the parser ignores it.
      forward animation dance(triangle tri);
      ...
      triangle dancer(skew=0.5, animation_block=dance);
      ...
      animation dance(triangle tri){
        //GPL statements, typically manipulating tri
      }
       
   * A forward declaration is like a C/C++ function prototype. It is always allowed, but is only required if the animation block is referenced prior to the block's definition. The above code is an example of when it is required.

   * In the forward declaration, the identifier following the game object parameter type is not required. It is ignored if provided.

   * The forward declaration's argument type must match the type specified in the block definition (a triangle in lines 3 and 7 above). The block must also only be assigned to the ``animation_block`` parameter of a triangle object (line 5).

   * A forward declaration does not require the declared animation block to be defined unless the declared block is assigned to a game object's ``animation_block`` parameter (assignment shown in line 5 above).
   
|

.. topic:: ``Animation_code`` class

   A partially implemented ``Animation_code`` class has been provided. It encapsulates the required information contained in a GPL animation block. ``Animation_code`` has four private data members:

   1. ``block_name`` A string. From line 7 (and 3) above, ``dance``.

   2. ``parameter_name`` A string. From line 7 above, ``tri``.

   3. ``parameter_type`` A ``Gpl_type``. From line 7 (and 3) above, ``TRIANGLE``.

   4. ``block`` A ``Statement*``. The head of the linked list of statements associated with the block.

   Three static sets (publicly accessible) are also provided by ``Animation_code``.

   * ``declared_blocklist`` is the set of names (strings) of animation blocks that have been declared (line 3 above).

   * ``defined_blocklist`` is the set of names (strings) of animation blocks that have been defined (lines 7-9 above).

   * ``used_blocklist`` is the set of names (strings) of animation blocks that have been assigned to game objects (line 5 above). 

   When parsing is completed, these sets will be used for error checking and to remove declared but undefined (and unused) animation blocks from the symbol table.


   The ``Animation_code`` class has its own ``Gpl_type``, ``ANIMATION_CODE``, to distinguish it from an animation block. For example::

     circle    hoop(x=100);
     circle    wheel;
     animation roll(circle cir) {
       cir.x+=1;
     }
     on akey {
       //assign ANIMATION_BLOCK = ANIMATION_CODE
       hoop.animation_block = roll;
       //assign ANIMATION_BLOCK to ANIMATION_BLOCK
       wheel.animation_block = hoop.animation_block;
     }

   Both ``Gpl_type``\ s (``ANIMATION_BLOCK`` and ``ANIMATION_CODE``) are ``Animation_code*`` in the underlying C++ code.

|

10.  The first step to supporting animation blocks is to enhance ``Symbol`` to hold an ``Animation_code*`` as a value:

    1. Add ``Animation_code*`` to ``union symbol_type``. Don't forget to add a new constructor to ``symbol_type`` as well.
    2. Add a Symbol constructor that takes an ``Animation_code*``. Initialize the Symbol's type to ``ANIMATION_CODE``. Only one constructor is required. There are never arrays of ``Animation_code`` objects.
    3. Add a case for ``ANIMATION_CODE`` to Symbol's destructor to release its memory.
    4. Depending on your implementation, Symbol's ``operator<<`` may need modification to print animation blocks correctly. Printing an animation block is trivial. For example, ``animation code roll`` where "roll" is the name of the of the block of animation code. Note that ``gpl_type_to_string(ANIMATION_CODE)`` returns the string "animation code".

|

11. Add a member function to ``Table_handler`` and ``Symbol_table`` to remove a named Symbol. This will be used to remove unused forward declarations when parsing finishes::

        // Simply call erase() on the unordered_map
        void Symbol_table::erase(const std::string& name) 

        //Use Table_handler::lookup as a guide. Call erase() on Symbol table with matching symbol.
        void Table_handler::erase(const std::string& name);


|

12. The forward animation declaration adds an entry into the symbol table (so that it is defined when assigned to a game object's ``animation_block`` attribute).

    There are two grammar productions specific to forward animation declarations: ``forward_declaration:`` and ``animation_parameter:``. Both actions of ``animation_parameter:`` should assign the ``object_type`` (whose semantic value is a ``Gpl_type``) to ``$$``. The ``forward_declaration:`` production should insert a dynamically allocated ``Animation_code`` object into the table handler. After inserting the ``Animation_code`` object, insert the animation block name into ``Animation_code::declared_blocklist``.

|

13. Take a few minutes to write a test program to ensure you can implement ``std::set_difference`` correctly. For example, subtracting set ("two", "four", "six") from ("one", "two", "three", "four") should produce the result set ("one", "three").

|

14. The the 'program:' production rule is matched after the GPL program has completed parsing. Write post-parsing actions for in this production. Use ``std::set_difference`` to subtract the ``Animation_code`` sets::

        declared_blocklist - defined_blocklist //use std::set_difference of course

    Iterate through the result set, calling ``Table_handler::erase`` on each animation block that was declared but not defined. At this early point in the project implementation ``Animation_code::defined_blocklist`` will be empty (all symbols inserted from a forward declaration will be removed).

|

15. Pass tests ``t009.gpl`` (proper forward declarations) and ``t010.gpl`` (redeclaration error).

|

16. ``Animation_code`` objects in the symbol table are accessed as ``Constant`` objects when assigned to a game object's ``animation_block`` attribute::

        hoop.animation_block = roll; //The rhs variable is evalated, returning a constant object

    Derive a new class from ``Constant`` named ``Animation_block_constant``. Its private data type is ``Animation_code*``.  Add a new member function to the base class, ``Constant``::
        
        virtual const Animation_code* as_animation_block() const { throw intrinsic_type; }

    Override this function in ``Animation_block_constant`` to ``return data;``.  

    Because all game objects have an animation block attribute, override ``as_animation_block()`` in the ``Game_attribute_constant`` class as well. This function is modeled after ``Game_attribute_constant::as_int()``. That is, if the attribute's type is ``ANIMATION_BLOCK`` obtain the ``Animation_code`` pointer via ``read_attribute`` and return it. Otherwise throw the object's type as an exception.

|

17. Add a ``case ANIMATION_CODE`` to ``Symbol::as_constant()`` to create and return an ``Animation_block_constant``.

|

18. The ``Game_attribute_locator`` class allows game object attributes to be modified. Add another ``mutate()`` function to this class (and the base class) to allow modification of the ``animation_block`` attribute of game objects. 

|

19. The forward declaration adds animation code objects to the symbol table. However those animation code objects are incomplete. The private data members ``std::string parameter_name`` and ``Statement* block`` are not known at the forward declaration. They are initialized by the parser when the animation code block is defined.

    Derive another ``Locator`` class: ``Animation_code_locator``. The purpose of this class is to allow easy modification/initialization of ``Animation_code``'s private data members, ``parameter_name`` and ``block``.
    
    ``Animation_code_locator`` has an ``Animation_code*`` as private data. In addition to the constructor, the class has two member functions::
        
        virtual void mutate(const std::string& val); //set parameter_name
        virtual void mutate(const Statement* val);   //set block
        
|

20. Add a ``case ANIMATION_CODE`` to ``Symbol::as_lvalue()`` to create and return an ``Animation_code_locator``.

|

.. topic:: Understanding symbol tables at parse time

   Two production rules drive the parsing of animation blocks:

   .. code-block:: cpp
      :force:

      animation_block:
          animation_declaration statement_block

      animation_declaration:
            T_ANIMATION T_ID T_LPAREN object_type T_ID T_RPAREN 

                
   The second production is parsed first. It is here that the animation code object is added to the symbol table if not already done by a forward declaration. After the ``animation_declaration`` production is parsed, the code block of statements is parsed. Take this example:

   .. code-block::
      :linenos:

      animation dance(triangle tri)
      {
        print("The skew is: " + tri.skew);
        tri.x+=5;
        tri.red=0.5;
      }


   Line 1 is parsed by the ``animation_declaration`` production. 

   Then lines 2-6 are parsed (a ``statement_block`` in the grammar).
   
   To parse the block successfully, the ``tri`` variable must be a declared symbol. The ``animation_declaration`` production will push a new symbol table onto the symbol table stack and insert the ``tri`` symbol into it. The ``animation_block`` production will pop the table off the stack.

|

21. Write the action for the ``animation_declaration`` production:

    1. If the animation block had a forward declaration, the ``Animation_code`` object already exists in the symbol table. Confirm this. If not, add it now.
    2. If the symbol's type is ``ANIMATION_CODE``, set the ``parameter_name`` of the symbol (via ``as_lvalue()->mutate(*$5)``).
    3. Use table handler to push a symbol table onto the symbol table stack
    4. Create a new game object symbol and insert it into the symbol table. Refer to the ``object_declaration`` production as a quick reminder (a simple switch statement will do the trick).
    
    The ``animation_block`` production will need to access the ``Animation_code`` symbol to attach the statement block to it, so the name of the block needs to be passed to it as a semantic value, ``$$=$2`` (``%type <union_string> animation_declaration``).

|

22. Add the action for the ``animation_block`` production:

    1. Pop the symbol table off the table handler's stack (this production is matched as soon as the statement block has finished parsing).
    2. If the animation block's name (``$1``, a ``std::string pointer``) isn't ``nullptr``:
        1. Retrieve the symbol and set its statement pointer. The statement pointer is available via ``$2``.
        2. Insert the animation block's name into ``Animation_code::defined_blocklist``.

|

23. Pass tests ``t011.gpl`` through ``t015.gpl``.

|

24. Pass tests ``t016.gpl``, ``t017.gpl``, and ``t018.gpl``. These tests should generate errors. Examine the ``.gpl`` and ``.err`` files for guidance. Note that with a redeclaration error (``t018.gpl``) The (uncreatable) ``animation_declaration``  should set ``$$=nullptr;``.

|

25. The parameter processing portion of the ``object_declaration:`` production must be able to initialize the object's animation block::

        triangle teepee(x=12, animation_block=dance, y=38);

    Add a case for the fourth parameter type, ``ANIMATION_BLOCK``. Be sure to insert the block's name into ``Animation_code::used_blocklist``.

|

26. Pass tests ``t019.gpl`` through ``t022.gpl``.


|

27. The ``program:`` production currently subtracts sets to remove from the symbol table those forward declarations that were not defined:

    .. code-block:: none

        declared - defined

    Before calculating this ``set_difference``, insert another ``set_difference`` calculation:

    .. code-block:: none

        used - defined

    The result of this difference is the set of animation blocks that have a forward declaration and are used by game objects but are never defined. This is an error. For each item in this error set report a ``NO_BODY_PROVIDED_FOR_FORWARD`` error.

|

28. Pass tests ``t023.gpl`` through ``t026.gpl``.

|

.. topic:: Executing animation blocks

   Executing a game object's animation block is similar to executing a function with an argument: 

    1. A new symbol table is pushed onto the stack
    2. A symbol representing the function argument (the game object) is inserted into the table
    3. The function is executed
    4. The new symbol table is popped off the stack.

    The game object function argument is passed by reference. That is, the symbol inserted in step 2 is just a reference to a symbol that already exists in a different symbol table. Specifically, the reference refers to the *value* of an existing symbol. 
    
|

29. Derive a subclass from ``Symbol`` named ``Reference``. 

    1. (This substep has been done for you) Make the ``Symbol`` class destructor virtual. Whenever inheritance is involved, if any member functions are virtual the destructor should *always* be virtual.
    2. Give the derived class direct access to ``Symbol``'s data member, ``value`` variable. Change the access specifier for that variable from ``private`` to ``protected``.
    3. To Symbol's ``union symbol_type`` add a ``game_object_pointer`` (and a corresponding ``symbol_type`` constructor).
    4. Write a new ``Symbol`` constructor with an access specifier of ``protected``. It will only be invoked from the initialization list of the ``Reference`` class's constructor::

        Symbol(const std::string& name, Gpl_type type, Game_object* argument);

    5. The Reference class only has a constructor and a destructor::

        class Reference : public Symbol {
          public:
            Reference(const std::string& parameter_name,  
                      Gpl_type           parameter_type, 
                      Game_object*       gop)
            : Symbol(parameter_name, parameter_type, gop) {}
            virtual ~Reference() { value.game_object_pointer=nullptr; }
        };

    .. note::

       The destructor for ``Reference`` is trivial but important to the functionality. As with all virtual destructors, after ``Reference``'s destructor runs, the base class destructor runs (``Symbol::~Symbol()``). Assigning ``nullptr`` to the reference's data pointer ensures the ``delete`` statement in ``~Symbol()`` won't release the memory the reference was referring to. That memory belongs to a Symbol in a different symbol table.

|

30. The ``Animation_code`` class has an execute function that is automatically called when game objects are animated::

        void Animation_code::execute(Game_object* argument) const

    Write the body to this function:

    1. Push a new symbol table onto the stack.
    2. Create a Reference object to the game object argument (for a guide on syntax, refer to the symbols created in ``object_declaration:``)
    3. Call the ``execute()`` function on ``Animation_code``'s private data member, ``block``.
    4. Pop the symbol table off of the stack.

|

31. Pass ``t027.gpl``.

.. sidebar:: Example: two animation block assignments in ``akey`` event block

   .. code-block:: cpp
      :emphasize-lines: 7,9,10

        circle    hoop(x=100);
        circle    wheel();
        animation roll(circle cir) {
          cir.x+=1;
        }
        on akey {
          hoop.animation_block = roll;

          wheel.animation_block = 
            hoop.animation_block;
        }

|

32. Enhance the ``assign_statement:`` production to support the assignment of animation blocks to ``animation_block`` attributes of game objects. 
    
    The lefthand side is valid if it is an ``ANIMATION_BLOCK`` being assigned either ``ANIMATION_BLOCK`` or ``ANIMATION_CODE``.  The ``ANIMATION_CODE`` type may never appear on the LHS of an assignment.

    1. If ``ANIMATION_CODE`` is on the lefthand side, ``Error::CANNOT_ASSIGN_TO_ANIMATION_CODE``.
    2. If the lefthand side parameter type does not match the righthand side parameter type, ``Error::ANIMATION_BLOCK_ASSIGNMENT_PARAMETER_TYPE_ERROR``.
    3. If the assignment is valid and the righthand side is ``ANIMATION_CODE``, insert the block's name into ``Animation_code::used_blocklist``.

|

33. In ``Assign::execute()`` add a case for ``ANIMATION_BLOCK``::

        case ANIMATION_BLOCK:
          lvalue->mutate(rhs_constant->as_animation_block());
          break;

|


34. Pass remaining tests that exercise the assignment operator.

|

35. Try your gpl interpreter both the gpl game you wrote and the example games provided at the beginning of the semester.

===============
Version History
===============

Ver. 7 May 2021

* Rephrased sentence on how the Gpl_types are represented by the underlying C++ code.
* Other minor text changes.
* Updated number of tests

Ver. 19 Dec 2020

* More test shuffling and additional test. Removed the downcasting direction in step #32.3. It is not required.

Ver. 15 Dec 2020

* Added a test so renumbered in text.
* Renamed ``Animation_block_locator`` to ``Animation_code_locator``

Ver. 12 Dec 2020

* Changed last couple of items

Ver. 08 Dec 2020

* Corrected typo.

Ver. 05 Nov 2020

* Added const to ``dynamic_cast<const Variable*>``

Ver. 02 May 2020

* Added code highlights to a sidebar

Ver. 01 May 2020

* Restructured document for use with Sphinx markup

Ver. 29 Apr 2020

* Removed the penultimate step. It is no longer relevant.

Ver. 28 Apr 2020

* Fixed typo and number of tests shown in manifest. Noted "virtual" keyword had been done already. Reworded last step.

Ver. 13 Dec 2019

* Fixed invisible angle-braces around dynamic cast

Ver. 12 Dec 2019

* Fixed private to protected.

Ver. 10 Dec 2019

* Completed writeup.

Ver. 08 Dec 2019

* Through step 26.

Ver. 05 Dec 2019

* Added description through test 17

Ver. 02 Dec 2019

* Initial version.
