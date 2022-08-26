#include <cassert>
#include "Symbol.h"
#include "Constant.h"
//cpp
Symbol::Symbol(const std::string& name, double* value)
  : name(name), value(value), type(GPL::DOUBLE), count(0)
{
}
Symbol::Symbol(const std::string& name, double* value, int count)
  : name(name), value(value), type(GPL::DOUBLE), count(count)
{
}
//cpp
Symbol::Symbol(const std::string& name, int* value)
  : name(name), value(value), type(GPL::INT), count(0)
{
}
Symbol::Symbol(const std::string& name, int* value, int count)
  : name(name), value(value), type(GPL::INT), count(count)
{
}
Symbol::Symbol(const std::string& name, std::string* value)
  : name(name), value(value), type(GPL::STRING), count(0)
{}
Symbol::Symbol(const std::string& name, std::string* value,int count)
  : name(name), value(value), type(GPL::STRING), count(count)
{}


const Constant* Symbol::as_constant() const          //Symbol value is not an array
{

  switch (type)
  {
  case GPL::INT:
    return new Integer_constant(*value.int_pointer);
  case GPL::DOUBLE:
    return new Double_constant(*value.double_pointer);
  case GPL::STRING:
    return new String_constant(*value.string_pointer);
  default:
    throw type;
  }
}
const Constant* Symbol::as_constant(int index) const //Symbol value is an array
{
  if(index < 0 || index >= count)
    return nullptr;
  switch (type)
  {
  case GPL::INT:
    return new Integer_constant(*(value.int_pointer));
  case GPL::DOUBLE:
    return new Double_constant(*(value.double_pointer));
  case GPL::STRING:
    return new String_constant(*(value.string_pointer));
  default:
    throw type;
  }
}
std::ostream& operator<<(std::ostream& os, const Symbol& sym)
{

  switch(sym.type)
  {
    case GPL::DOUBLE:
      if(!sym.count)
        os << to_string(sym.get_type()) << " " << sym.get_name() << " = "<< *sym.get_double() << "\n";
      else
      {
        double* temp = sym.get_double();
        for( int i = 0; i < sym.count; i++)
        {
          os << to_string(sym.get_type()) << " " << sym.get_name() << "["<<i<<"]"<< " = " <<temp[i]<< "\n";
        }
      }
      break;
    case GPL::INT:
      if(!sym.count)
        os << to_string(sym.get_type()) << " " << sym.get_name() << " = "<<*sym.get_int()<< "\n";
      else
      {
        int* temp = sym.get_int();
        for( int i = 0; i < sym.count; i++)
        {
          os << to_string(sym.get_type()) << " " << sym.get_name()<< "["<<i<<"]" << " = "<<temp[i]<< "\n";
        }
      }
      break;
    case GPL::STRING:
      if(!sym.count)
        os << to_string(sym.get_type()) << " " << sym.get_name() << " = "<<'"'<<*sym.get_string()<<'"'<< "\n";
      else
      {
        std::string* temp = sym.get_string();
        for( int i = 0; i < sym.count; i++)
        {
          os << to_string(sym.get_type()) << " " << sym.get_name() << "["<<i<<"]" << " = "<<'"'<<temp[i]<<'"'<< "\n";
        }
      }
      break;

    default:
      assert(false);
  }//switch statement
  return os;
}
GPL::Type Symbol::get_type() const
{
  return this->type;
}
std::string Symbol::get_name() const
{
  return this->name;
}

int Symbol::get_count() const
{
  return count;
}
double* Symbol::get_double() const
{
  return this->value.double_pointer;

}
int* Symbol::get_int() const
{
  return this->value.int_pointer;

}
std::string* Symbol::get_string() const
{
  return this->value.string_pointer;

}
Symbol::~Symbol()
{
  switch(this->get_type())
  {
    case GPL::DOUBLE:
      if(!this->count){
        delete this->value.double_pointer;
        this->value.double_pointer = nullptr;
      }
      else
      {
        delete [] this->value.double_pointer;
        this->value.double_pointer = nullptr;
      }
      break;
    case GPL::INT:
      if(!this->count)
      {
        delete this->value.int_pointer;
        this->value.int_pointer = nullptr;
      }
      else
      {
        for(int i = 0; i < count; ++i)
          this->value.int_pointer[i] = 0;
       // std::cout <<"calling int destructor" <<"\n";
        delete [] this->value.int_pointer;
      }
      break;
    case GPL::STRING:
      if(!this->count)
      {
        delete this->value.string_pointer;
        this->value.string_pointer = nullptr; 
        }
      else
      {
        delete [] this->value.string_pointer;
        this->value.string_pointer = nullptr; 
      }
      break;
    default:
      assert(false);
  }

}



/*
int main()
{
  double* oneofem=new double(3.141592);
  int* oneintem=new int(3);
  std::string* onestringem = new std::string("Icabod Crane");
  int* bunchintem=new int[4]{1,2,3,4};
  double* bunchofem=new double[4] {1.123,2.234,3.456,7.899};
  std::string* bunchstringem=new std::string[4] {"chicken","steak","sofritas","chorizo"};

  Symbol one("pi",oneofem);
  Symbol two("count",oneintem);
  Symbol three("name",onestringem);
  Symbol onemulti("pi",bunchofem,4);
  Symbol threemulti("food",bunchstringem,4);
  Symbol twomuli("count",bunchintem);

  std::cout << three;
  std::cout << two;
  std::cout << one;
  std::cout << onemulti;
  std::cout << threemulti;
  return 0;
}
*/
