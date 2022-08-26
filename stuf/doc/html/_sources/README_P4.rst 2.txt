##############
Fourth Project
##############

Ver. 15 May 2021

=======
Summary
=======

Add support for game objects (Rectangle, Textbox, etc.)

========
Manifest
========

:``./src/p4/Game_object.cpp``:        The base class for the game objects
:``./src/p4/Game_object.h``:

|

:``./src/p4/Rectangle.cpp``:          Rectangle game object class
:``./src/p4/Rectangle.h``:
:``./src/p4/Circle.cpp``:             Circle game object class      
:``./src/p4/Circle.h``:
:``./src/p4/Triangle.cpp``:           Triangle game object class
:``./src/p4/Triangle.h``:
:``./src/p4/Pixmap.cpp``:             Pixmap game object class
:``./src/p4/Pixmap.h``:
:``./src/p4/Textbox.cpp``:            Textbox game object class
:``./src/p4/Textbox.h``:

|

:``./src/p4/Window.cpp``:             Class that encapsulates graphics-rendering code.
:``./src/p4/Window.h``:

|

:``./src/p4/Animation_code.cpp``:     Animation code class. Extended in p6
:``./src/p4/Animation_code.h``:
:``./src/p4/Event_manager.cpp``:      Event manager class. Extended in p5
:``./src/p4/Event_manager.h``:
:``./src/p4/Statement.cpp``:          GPL programming statement base class.
:``./src/p4/Statement.h``:            Extended in p5

|

:``./src/p4/default_pixmap.h``:       A default image used by GPL

|

:``./src/p4/tests``:                  Testing suite directory, 40 tests.

===================
Project description
===================

1.  Copy the files from ``p3`` over to ``p4``:

|

.. topic:: Game objects

    There are five game objects: Rectangle, Circle, Triangle, Pixmap, and Textbox. Each is represented by a C++ class derived from a common ``Game_object`` class. 

    In GPL code, after a game object is created, it is always used with one of its attributes::

      Circle pie;
      int x    = pie.visible; //"visible" is attribute of pie
      double c = abs(pie.red - 0.5); //"red" is attribute of pie

    

    The game object classes have three member functions for managing attributes: write an attribute, read an attribute, and retrieve the type of an attribute:
    
    |

    .. cpp:namespace-push:: Game_object

    .. cpp:function:: Gpl_type attribute_type(const std::string& name)

    * Throws an ``out_of_range`` exception if ``name`` is not a game object attribute.
    
    |

    .. cpp:function:: template<typename T> T& write_attribute(const std::string& name, const T& var)
    .. cpp:function:: template<typename T> T& read_attribute (const std::string& name, T& var)

    * Throws an ``out_of_range`` exception if ``name`` is not a game object attribute.

    * Throws the ``Gpl_type`` of the attribute as an exception if the type of ``var`` does not match the type of the named ``Game_object`` attribute.

    * ``var`` may be an ``int``, ``double``, or ``std::string`` (or ``Animation_code*`` used in future project).

    .. cpp:namespace-pop::

    |

    Example function invocations:

    .. code-block:: cpp
       :emphasize-lines: 1,4,7

        Gpl_type param_type=game_object_ptr->attribute_type("red");
        Game_object* gop=new Circle();
        int nearby=8;
        gop->write_attribute("proximity", nearby); //"proximity" attrib assigned value of nearby
        Game_object* tri_tip=new Triangle;
        int seeit;
        gop->read_attribute("visible", seeit); //seeit assigned value of "visible" attrib.

|

2.  Derive a ``Game_attribute_constant`` class from ``Constant``. Because a game object is always used with one of its attributes, ``Game_attribute_constant`` has two pieces of private data:

    1. a ``const Game_object*`` pointing to the actual game object, and 
    2. a ``string`` holding the name of the attribute.

    Class declaration::

          class Game_attribute_constant : public Constant {
            public:
              Game_attribute_constant(const Game_object* gop, const std::string& attribute_name)
                : Constant(gop->attribute_type(attribute_name)), 
                  gop(gop), 
                  attribute_name(attribute_name) {}

              virtual int           as_int()         const;
              virtual double        as_double()      const;
              virtual std::string   as_string()      const;

            private:
              const Game_object* gop;
              std::string attribute_name;
          };

    .. note::

        The type of a ``Game_attribute_constant`` is the type its attribute (an integer, double, or string). 

        The attribute's type is initialized via the base class constructor.

        Because the constructor invokes ``Gpl_type attribute_type(const std::string& name)``, an ``out_of_range`` exception may be thrown.

    Write the definitions for ``as_int()``, ``as_double()``, and ``as_string()`` in the source file for the ``Constant`` class:

    * ``as_int()`` returns the value of the attribute if it is an integer. Throw the constant's type (``Constant::type()``) as an exception if the attribute is a different type.
    * ``as_double()`` returns the value of the attribute if it is an integer or a double.  Throw the constant's type (``Constant::type()``) as an exception if the attribute is a different type.
    * ``as_string()`` returns the value of the attribute (converted to string) if it is an integer, double, or string. Throw the constant's type (``Constant::type()``) as an exception if the attribute is a different type.

|

3.  Ensure the project compiles without error.

|

4.  Add support for the five game object types (all as pointers) to class ``Symbol``. This requires:

    * Writing 10 constructors: two constructors for each new type, one for individual objects and one for arrays.

    * Adding support for the five game object types in ``operator<<(ostream& os, const Symbol& sym)``. The ``operator<<`` has been overloaded for the game objects so game objects can be printed as easily as floats.

    * Writing two additional ``as_constant`` member functions::

        Symbol::as_constant(           const std::string& attribute_name) const; // non-arrays
        Symbol::as_constant(int index, const std::string& attribute_name) const; // arrays

      These two functions are structurally similar to the existing ``as_constant()`` and ``as_constant(int)`` functions. Just like those functions, if the symbol type is not one of the five game object types, ``throw type;``.

|

5.  Ensure the project compiles without error.

|

.. topic:: Declaring game objects
    
    In the previous project, the actions for the ``variable_declaration:`` production rules were coded to create int, string, and double symbols and insert them into a symbol table.  The ``object_declaration:`` production fulfills the equivalent function for game objects. The actions for the ``object_declaration:`` production rules create symbols of the five game objects: ``Rectangle``, ``Circle``, ``Textbox``, ``Triangle``, and ``Pixmap``::

        object_declaration:
            object_type T_ID parameter_list_or_empty
            | object_type T_ID T_LBRACKET expression T_RBRACKET
        ;

    The first rule creates single game objects. For example::

        triangle my_triangle;
        rectangle some_rect(); //empty parentheses optional
        circle happy_face(x=100, y=100); //may have parameters

    The action of ``object_declaration:``'s first rule creates the game object symbol and then processes the object's parameters (if any). 

|

6.  Write the action for ``object_declaration:``'s first rule to create game objects and insert them into the symbol table. Ignore ``parameter_list_or_empty`` for now.

|

7.  Ensure the project passes tests 1-3.

|

8.  Write the action for the second rule of the ``object_declaration:`` production that creates arrays of game objects. Use the array rule for ``variable_declaration:`` as a guide.

|

9.  Ensure the project passes tests 4-8.

|

.. topic:: Initializing game objects with parameters

   .. code-block:: cpp

        triangle my_triangle(x = 100, y = 100);
        rectangle some_rect(x = 42, y = 2, h = 100, w = 200, green = 1.0);

   The ``object_declaration``'s, first rule ends with the non-terminal token ``parameter_list_or_empty``. The production for ``parameter_list_or_empty:`` parses the parameters before the ``object_declaration:`` production is processed.
   This presents a challenge because the error messages generated while processing parameters include the name of
   the game object being declared which isn't available until the ``object_declaration:`` production is processed. 
   
   The solution is to collect the parameters in the
   ``parameter_list_or_empty:`` production but delay processing them until
   after the game object itself is created as a symbol in the
   ``object_declaration:`` production (completed in previous step). 

   The parameters are collected into a linked list that is passed to the
   ``object_declaration:`` production. The ``object_declaration:`` production will
   validate the parameters after the game object is added to the symbol table.  

|

10. Declare a structure in ``gpl.y`` to handle the linked
    list of parameters::

        struct Parameter {
              const Expression* expression;
              std::string name;
              Parameter* next;
              ~Parameter() { delete next; }
        }; 

|

11. Write the action for the ``parameter:`` production. The action simply creates and initializes a new ``Parameter`` object (``next`` is initialized to ``nullptr``). Assign the new structure object to ``$$``.

|

12. Add ``Parameter*`` to the ``%union`` directive. Add a ``%type`` directive for the non-terminal token ``parameter``. 

|

13. Ensure the project compiles without error.
   
|

14. Write an action for the first rule in the ``parameter_list:`` production.  The first rule should append the new parameter (``$3``) to the *end* of the linked list of parameters (``$1``). The action for the production's second rule is just ``{$$=$1;}``. This action is also bison's default action, so adding it to the second rule is optional.

|

15. Add actions to the ``parameter_list_or_empty:`` production's two ``empty`` rules: ``$$=nullptr;``. The action for the production's first rule should assign the linked list to ``$$``.

|

16. Ensure the project compiles without error.
   
|

17. The action for ``object_declaration``'s first rule creates a game object and adds it to the symbol table. Append code after this to process the parameters (if any). Iterate thought the linked list of parameters: 

    *  Use :cpp:func:`Game_object::attribute_type` to determine the *Attribute*'s expected type (int, double, or string).
    * Call :cpp:func:`Game_object::write_attribute` to set the attribute's value.
    * Use try/catch to report unknown parameters and parameter type mismatches thrown by ``attribute_type`` and ``write_attribute``.

|

18. Ensure the project passes tests 9-12.

|

19. Add code to handle the other two rules in the ``variable:`` production. These
    two rules handle variables that are members of game objects. For example::

        my_circle.radius
        my_list_of_triangles[3].y

    Derive a class from ``Variable`` named ``Member_variable``. It has the following characteristics:

    * One private data string: the name of the game object's attribute.
      In the above example the attribute names are "radius" and "y". 

    * An ``evaluate()`` function. This is very similar to ``Variable::evaluate()``. The signature of the function in the class declaration is::

                virtual const Constant* evaluate() const override;

      .. note::

         The ``override`` keyword appears in the class declaration but is absent from the definition of the function (just like the ``virtual`` keyword).

    * A ``type()`` function that returns the type of the attribute. Roughly: ``symbol()->as_constant(...)->type()``. The signature of the function in the class declaration is::

                virtual Gpl_type type() const override;

    * Two constructors just like ``Variable``: one for the attribute of an individual object (``my_circle.radius``) and one for the attribute of an element of an array (``my_list_of_triangles[3].y``).

    If there is an error in the fourth rule of the ``variable:`` production, the proper tokens to release are ``$1``, ``$3``, and ``$6``. However, if there are *no* errors, a ``Member_variable`` is created and only ``$1`` and ``$6`` should be released. The expression ``$3`` is used by ``Member_variable`` and should not be released by the parser.

.. topic:: Handling errors when parsing member variables

   There are several errors to recover from in the final two rules of the `variable:` production. The errors
   
     *Variable...is not an object*, and

     *Object...does not contain the member variable...*
   
   can be reported by creating a (temporary, discardable) constant object from the symbol and catching the exceptions thrown. For example:

   .. code-block:: cpp
      :force:

        //Somewhere inside of the third rule of the variable: production
        try { std::shared_ptr<const Constant>(sym->as_constant(*$3)); }
        //catch blocks follow.

   Wrapping the returned constant in a ``shared_ptr`` is just a simple way to avoid leaking the allocated constant. 

   For error-recovery in the ``variable:`` production  generate an "empty" ``Variable``:  ``$$=new Variable("")``. Do not create an empty ``Member_variable``.

   It's worth understanding *why* creating a constant object can generate exceptions that are useful for error-handling.  The member function ``Symbol::as_constant(const std::string& attribute_name)`` called in the try block above switches on the symbol's type with a case for each of the five game object types (``RECTANGLE``, ``TRIANGLE``, etc.). (Take a look at the member function source.)  If the symbol's value isn't a game object, the switch's default case throws the actual type of the symbol (*Variable...is not an object*).

   If the symbol's value *is* a game object, the relevant case in the switch statement creates and returns a ``Game_attribute_constant``. The ``Game_attribute_constant`` constructor retrieves the attribute's type by calling :cpp:func:`attribute_type(attribute_name) <Game_object::attribute_type>`. As described earlier, this function throws ``out_of_range`` if the game object does not have the named attribute (*Object...does not contain the member variable...*).


|

20. Pass all remaining tests.

===============
Version History
===============
Ver. 15 May 2021

* Changed outdated ``Animation_block`` to ``Animation_code``

Ver. 2 May 2021

* Clarified that the game object attribute type/read/write functions are *member* functions.

Ver. 4 November 2020

* Removed errant comma
* Added `const` to Parameter's expression pointer.

Ver. 2 November 2020

* Removed mention of tarball
* Made string argument to as_constant() references
* Fixed grammar.

Ver. 3 May 2020

* Restructured document for use with Sphinx markup

Ver. 21 April 2020

* Added ``const`` to ``Game_object*`` in ``Game_attribute_constant``.

Ver. 12 April 2020

* Clarified *Attribute*'s type when iterating over an object's parameters.

* Fixed order of $1, $3 in ``parameter_list:`` production description.

* Added verbiage in final item about releasing memory.

Ver. 6 April 2020

* Elaboration on ``override`` keyword

Ver. 4 March 2020

* Add const to ``shared_ptr<Constant>``

Ver. 3 March 2020

* Add override directives

Ver. 17 November 2019

* Enhanced the description around handling errors by constructing a constant object.

Ver. 11 November 2019

* Added direction to create two constructors for ``Member_variable``.

Ver. 1 November 2019

* Change ``Game_object_constant`` to ``Game_attribute_constant``

Ver. 31 October 2019

* Minor updates to accommodate revisions to ``Variable`` class.

Ver. 29 October 2019

* Minor grammar change.

Ver. 24 October 2019

* Switched public/private order in class declaration. Appended to note on operator<<

Ver. 21 October 2019 - A

* Nominal updates to reflect simpler use of Symbol in ``variable:`` production

Ver. 21 October 2019

* Portions rewritten to reflect updated design.

Ver. 17 October 2019

* Minor changes

Ver. 16 October 2019

* Revisions to reflect redesigned game object classes.

Ver. 9 October 2019

* Major revision to writeup

Ver. 5 May 2019

* Reworded some text

Ver. 25 April 2019

* Removed 'const' from static member functions

Ver. 24 April 2019

* Corrected listed functions in step #10

Ver. 22 April 2019

* Reworked game object code and updated step #10. More to come.

Ver. 19 April 2019

* Some minor textual edits. More substantive edits to come.

Ver. 17 April 2019

* Initial version. Step 13 to be revised.
