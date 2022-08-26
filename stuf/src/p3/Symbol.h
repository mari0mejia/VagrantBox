//Symbol.h
#ifndef _SYMBOL_H_
#define _SYMBOL_H_
#include <iostream>
#include <string>
#include "types_and_ops.h"
#include <cassert>
class Constant;
class Symbol
{

  public:
    Symbol(const std::string& name, double* value);//for double varible
    Symbol(const std::string& name, double* value,int count);//for double varible
    Symbol(const std::string& name, int* value);//for int varible
    Symbol(const std::string& name, int* value,int count);//for int varible
    Symbol(const std::string& name, std::string* value);//for string varible
    Symbol(const std::string& name, std::string* value,int count);//for string varible
    const Constant* as_constant() const;          //Symbol value is not an array
    const Constant* as_constant(int index) const; //Symbol value is an array

    GPL::Type get_type() const;
    std::string get_name() const;
    double* get_double() const;
    int* get_int() const;
    int get_count() const;
    std::string* get_string() const;
    virtual ~Symbol();
    friend std::ostream& operator<<(std::ostream& os, const Symbol& sym);

    //prevent compiler from generating these funcitons
    Symbol(const Symbol&) = delete;
    Symbol& operator=(const Symbol&) = delete;



  private:
    union symbol_type 
    {
      double* double_pointer;
      int* int_pointer;
      std::string* string_pointer;

      symbol_type(double* value) : double_pointer(value){}
      symbol_type(int* value) : int_pointer(value){}
      symbol_type(std::string* value) : string_pointer(value){}

    };
    std::string name;//the name of the varible
    symbol_type value; //the contents of the variable
    GPL::Type type =GPL::NO_TYPE;   //the type of the variable. See types_and_ops.h
    int count=0;       //used to store the size if the variable is an array



};
#endif
