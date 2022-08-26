// Sun Sep 22 18:01:45 PDT 2019
#include<ostream>
#include<string>
#include <stdexcept>
#include <vector>
#include <iterator>
#include "Symbol_table.h"
class Scope_manager {
  public:
    // This function retrieves the Sample_singleton instance. 
    // It is a static member function--it is not associated with 
    // an instance (an object) of the class. 
    // Think of it as a "Traditional, C-style" function that is in the
    // Sample_singleton namespace
    static Scope_manager& instance();

    //Go ahead and write any member functions you require for your class
    //std::string get_it();

    // Create a new symbol_table and push it onto the back of the vector
    void push_table();


    /*
     * If more then one table is on the vector remove the table on the back
     * of the vector. There must always be at least one table in the vector.
     * Throw an exception if there is an attempt to pop it off. 
     */
    void pop_table();

    /* Add sym to the last/back table in the vector via a call to 
     * Symbol_table::insert. Return the value that Symbol_table::insert 
     * returns
     */
    bool add_to_current_scope(std::shared_ptr<Symbol> sym);

    /* Iterate through the symbol table vector from back to front,calling
     *  Symbol_table::lookup on each. Return the first match found. 
     *  Return nullptr if the symbol is undefined. 
     */
    std::shared_ptr<Symbol> lookup(const std::string& name);

    /* Search the top/last/back table for the named symbol. Return true if 
     * it is defined, false otherwisse. Used to check for variable 
     * redefinition errors.
     */
    bool defined_in_current_scope(const std::string& name);
    /*
     * Iterate through the symbol tables from back to front, printing each.
     */
    friend std::ostream& operator<<(std::ostream& os, const Scope_manager& sh);

    //The copy constructor and assignment operator are compiler-generated
    //functions. Here the compiler is instructed not to generate these
    //functions.  This is useful for all classes that don't explicitly 
    //require these functions (not just singletons).
    Scope_manager(const Symbol_table&) = delete;
    Scope_manager& operator=(const Symbol_table&) = delete;


  private:
    std::vector<std::shared_ptr<Symbol_table>>tables;
    //Any private data you require for your class
    //std::string string_data;
    //int count_data;


    //Make the default constructor private. Only the static
    //variable object (See Sample_singleton::instance() function) 
    //has access to this private function, allowing it to be constructed.
    Scope_manager() 
      :tables({})
      {push_table();}
};
