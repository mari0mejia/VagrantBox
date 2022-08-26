#include "Symbol_table.h"
//cpp
std::shared_ptr<Symbol> Symbol_table::lookup(const std::string& name)
{


  if(symbols.find(name)==symbols.end())
    return nullptr;

  return symbols.find(name)->second;
}
bool Symbol_table::insert(std::shared_ptr<Symbol> sym)
{
  if(lookup(sym->get_name())!=nullptr)
  {
    return true;
  }
  symbols.insert({sym->get_name(),sym});
  return false;


}
std::ostream& operator<<(std::ostream& os, const Symbol_table& symtab)
{
  
  std::vector<std::string> names;
  for(auto& key_value: symtab.symbols)
    names.push_back(key_value.first);
  std::sort(names.begin(), names.end());
  for(auto name : names)
  {
    os << *symtab.symbols.at(name);
  }
  return os;
}


/*
//main 
int main()
{
  Symbol_table symtab;

  double* double_oneofem=new double(3.141592);
  double* double_bunchofem=new double[4] {1.123, 2.234, 3.456, 7.899};
  std::string* string_oneofem=new std::string("hi there");
  std::string* string_twoofem=new std::string("hi there");
  std::string* string_bunchofem=new std::string[4]{"one", "two two", "three three three", "fo fo fo fo"};
  int* int_oneofem=new int(321);
  int* int_bunchofem=new int[4] {12, 34, 56, 78};

  symtab.insert(std::make_shared<Symbol>("alpha", string_oneofem));
  std::cout<<"fine at line 64"<<"\n";
  symtab.insert(std::make_shared<Symbol>("alpha", string_twoofem));
  std::cout<<"fine at line 66"<<"\n";
  symtab.insert(std::make_shared<Symbol>("gamma", string_bunchofem, 4));
  symtab.insert(std::make_shared<Symbol>("beta", double_oneofem));
  symtab.insert(std::make_shared<Symbol>("delta", double_bunchofem, 4));
  symtab.insert(std::make_shared<Symbol>("zeta", int_oneofem));
  symtab.insert(std::make_shared<Symbol>("eta", int_bunchofem, 4));

  std::cout << symtab << "\n";

  return 0;
}
*/
  // switch(iter->second->get_type())
    // {
    //   case GPL::DOUBLE:
    //     if(!iter->second->get_count())
    //       os << to_string(iter->second->get_type()) << " " << iter->second->get_name() << " = " << *iter->second->get_double() << "\n";
    //     else
    //     {
    //       double* temp = iter->second->get_double();
    //       for( int i = 0; i < iter->second->get_count(); i++)
    //       {
    //         os << to_string(iter->second->get_type()) << " " << iter->second->get_name()<<"["<<i<<"]" << " = " << temp[i] << "\n";
    //       }
    //     }
    //     break;
    //   case GPL::STRING:
    //     if(!iter->second->get_count())
    //       os << to_string(iter->second->get_type()) << " " << iter->second->get_name() << " = " <<'"'<< *iter->second->get_string() <<'"' << "\n";
    //     else
    //     {
    //       std::string* temp = iter->second->get_string();
    //       for( int i = 0; i < iter->second->get_count(); i++)
    //       {
    //         os << to_string(iter->second->get_type()) << " " << iter->second->get_name()<<"["<<i<<"]" << " = " << '"'<< temp[i] << '"'<<"\n";
    //       }
    //     }
    //     break;
    //   case GPL::INT:
    //     if(!iter->second->get_count())
    //       os << to_string(iter->second->get_type()) << " " << iter->second->get_name() << " = " << *iter->second->get_int() << "\n";
    //     else
    //     {
    //       int* temp = iter->second->get_int();
    //       for( int i = 0; i < iter->second->get_count(); i++)
    //       {
    //         os << to_string(iter->second->get_type()) << " " << iter->second->get_name() <<"["<<i<<"]" << " = " << temp[i] << "\n";
    //       }
    //     }
    //     break;
    //   default:
    //     assert(false);
    // }