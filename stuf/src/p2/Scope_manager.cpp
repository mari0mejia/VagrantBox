#include "Scope_manager.h"
bool Scope_manager::add_to_current_scope(std::shared_ptr<Symbol> sym)
{
 //add sym to the last/back table in the vector via a call to Symboll_table::insert. Return the value that Symbol_table::insert returns 
  //std::cout << " inserted the value " << "\n";
  return tables.back()->Symbol_table::insert(sym);
}//add_to_current_scope function 
Scope_manager& Scope_manager::instance()
{
  //Making this variable static gives it global scope
  //with visibility local to this function
  static Scope_manager the_instance;
  return the_instance;
}//scope manager instance 

std::shared_ptr<Symbol> Scope_manager::lookup(const std::string& name)
{
  std::shared_ptr<Symbol> temp(nullptr);
  for ( long unsigned int i = 0; i < tables.size() ; ++i)
  {
    //return the first symbol match 
    temp =  tables[i]->Symbol_table::lookup(name);
    if(temp)
      return temp;
    //else return nullptr
  }
  return nullptr;
}//scope manager constructor
  /* Search the top/last/back table for the named symbol. Return true if it is defined, 
   * false otherwise. Used to check for variable redefinition errors.
   */
bool Scope_manager::defined_in_current_scope(const std::string& name)
{
  std::shared_ptr<Symbol> temp(nullptr);
  for ( int i = 0; i < (int)tables.size() ; ++i)
  {
    //return the first symbol match 
    temp =  tables[i]->Symbol_table::lookup(name);
    if(temp)
      return true;
    //else return nullptr
  }
  return false;
}//defined in current scope function

void Scope_manager::push_table()
{
  tables.push_back(std::shared_ptr<Symbol_table>(new Symbol_table()));
}
//scopemanager --> tables[0] --> symbol_table[i]
std::ostream& operator<<(std::ostream& os, const Scope_manager& sh)
{
  for(auto i=sh.tables.size(); i--;)
    os << *sh.tables[i];
  return os;
}

void Scope_manager::pop_table()
{
  if(tables.size() > 1)
    tables.pop_back();
  else throw std::underflow_error("Can't pop global symbol table");
}
// int main()
// {
//   Scope_manager& symtab=Scope_manager::instance();
//   //and replace each call to insert() with a call to add_to_current_scope()

//   double* double_oneofem=new double(3.141592);
//   double* double_bunchofem=new double[4] {1.123, 2.234, 3.456, 7.899};
//   std::string* string_oneofem=new std::string("hi there");
//   std::string* string_bunchofem=new std::string[4]{"one", "two two", "three three three", "fo fo fo fo"};
//   int* int_oneofem=new int(321);
//   int* int_bunchofem=new int[4]{12, 34, 56, 78};

//   symtab.add_to_current_scope(std::make_shared<Symbol>("alpha", string_oneofem));
//   symtab.add_to_current_scope(std::make_shared<Symbol>("gamma", string_bunchofem, 4));
//   symtab.add_to_current_scope(std::make_shared<Symbol>("beta", double_oneofem));
//   symtab.add_to_current_scope(std::make_shared<Symbol>("delta", double_bunchofem, 4));
//   symtab.add_to_current_scope(std::make_shared<Symbol>("zeta", int_oneofem));
//   symtab.add_to_current_scope(std::make_shared<Symbol>("eta", int_bunchofem, 4));

//   std::cout << symtab << "\n";

// }
