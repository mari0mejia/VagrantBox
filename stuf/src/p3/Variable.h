#ifndef _VARIABLE_H
#define _VARIABLE_H
#include<iostream>
#include<memory>
using namespace std;
class Constant;
class Scope_manager;
class Expression;
class Symbol;
class Variable : public Expression {
  public:
    // initialize unused array_index_expression to nullptr
    Variable(const std::string& symbol_name) :symbol_name(symbol_name) ,array_index_expression(nullptr) {}
// Binary_operator(const Expression* lhs, const Expression* rhs) :lhs(lhs), rhs(rhs) {}

    // initialize array_index_expression to index_expr
    Variable(const std::string& symbol_name, const Expression* index_expr) :symbol_name(symbol_name), array_index_expression(index_expr) {}

    virtual const Constant* evaluate() const;
    virtual GPL::Type type() const;
    virtual ~Variable();

    Variable(const Variable&) = delete;
    Variable& operator=(const Variable&) = delete;
  protected: //allow derived class access

    std::string symbol_name;
    const Expression* array_index_expression;

    //The symbol() function eases access to the symbol
    //use it within Variable's member functions
    std::shared_ptr<Symbol> symbol()
    { return Scope_manager::instance().lookup(symbol_name); }
    std::shared_ptr<Symbol> symbol() const
    { return Scope_manager::instance().lookup(symbol_name); }
};
#endif