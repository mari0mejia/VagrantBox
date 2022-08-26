//File: Expression.h
#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "types_and_ops.h"
#include <memory>
#include "Symbol.h"
using namespace std;
class Constant;
class Expression {
  public:
    Expression():evaluated_constant(nullptr) {}
    virtual const Constant* evaluate() const=0;
    virtual GPL::Type type() const=0;
    virtual ~Expression();
    Expression(const Expression&) = delete;
    Expression& operator=(const Expression&) = delete;
  protected:
    //a wrapper around returned constants
    const Constant* ret(const Constant* new_evald_constant) const;
  private:
    //pointer to evaluate()'s created constant so it can be released
    mutable const Constant* evaluated_constant;
};

class Binary_operator : public Expression {
  public:
    Binary_operator(const Expression* lhs, const Expression* rhs) :lhs(lhs), rhs(rhs) {}
    virtual const Constant* evaluate() const=0;
    virtual ~Binary_operator() { delete lhs; delete rhs; }
  protected:
    const Expression* lhs;
    const Expression* rhs;
};
class Unary_operator : public Expression {
  public:
    Unary_operator(const Expression* rhs) :rhs(rhs) {}
    virtual const Constant* evaluate() const=0;
    virtual ~Unary_operator() { delete rhs; }
  protected:
    const Expression* lhs;
    const Expression* rhs;
};

class Plus : public Binary_operator {
  public:
    Plus(const Expression* lhs, const Expression* rhs) : Binary_operator(lhs, rhs){};
    virtual const Constant* evaluate() const;
    virtual GPL::Type type() const;
};

class Multiply : public Binary_operator {
  public:
    Multiply(const Expression* lhs, const Expression* rhs) : Binary_operator(lhs, rhs){};
    virtual const Constant* evaluate() const;
    virtual GPL::Type type() const;
};
class Minus : public Binary_operator {
  public:
    Minus(const Expression* lhs, const Expression* rhs) : Binary_operator(lhs, rhs){};
    virtual const Constant* evaluate() const;
    virtual GPL::Type type() const;
};

class Mod : public Binary_operator {
  public:
    Mod(const Expression* lhs, const Expression* rhs) : Binary_operator(lhs, rhs){};
    virtual const Constant* evaluate() const;
    virtual GPL::Type type() const;
};
class Divide : public Binary_operator {
  public:
    Divide(const Expression* lhs, const Expression* rhs) : Binary_operator(lhs, rhs){};
    virtual const Constant* evaluate() const;
    virtual GPL::Type type() const;
};
class Or : public Binary_operator {
  public:
    Or(const Expression* lhs, const Expression* rhs) : Binary_operator(lhs, rhs){};
    virtual const Constant* evaluate() const;
    virtual GPL::Type type() const;
};
class And : public Binary_operator {
  public:
    And(const Expression* lhs, const Expression* rhs) : Binary_operator(lhs, rhs){};
    virtual const Constant* evaluate() const;
    virtual GPL::Type type() const;
};
class Less_equal : public Binary_operator {
  public:
    Less_equal(const Expression* lhs, const Expression* rhs) : Binary_operator(lhs, rhs){};
    virtual const Constant* evaluate() const;
    virtual GPL::Type type() const;
};
class Greater_equal : public Binary_operator {
  public:
    Greater_equal(const Expression* lhs, const Expression* rhs) : Binary_operator(lhs, rhs){};
    virtual const Constant* evaluate() const;
    virtual GPL::Type type() const;
};
class Less_than : public Binary_operator {
  public:
    Less_than(const Expression* lhs, const Expression* rhs) : Binary_operator(lhs, rhs){};
    virtual const Constant* evaluate() const;
    virtual GPL::Type type() const;
};
class Greater_than : public Binary_operator {
  public:
    Greater_than(const Expression* lhs, const Expression* rhs) : Binary_operator(lhs, rhs){};
    virtual const Constant* evaluate() const;
    virtual GPL::Type type() const;
};

class Equal : public Binary_operator {
  public:
    Equal(const Expression* lhs, const Expression* rhs) : Binary_operator(lhs, rhs){};
    virtual const Constant* evaluate() const;
    virtual GPL::Type type() const;
};
class Not_equal : public Binary_operator {
  public:
    Not_equal(const Expression* lhs, const Expression* rhs) : Binary_operator(lhs, rhs){};
    virtual const Constant* evaluate() const;
    virtual GPL::Type type() const;
};
class Unary_minus : public Unary_operator {
  public:
    Unary_minus(const Expression* rhs) : Unary_operator(rhs){};
    virtual const Constant* evaluate() const;
    virtual GPL::Type type() const;
};
class Not : public Unary_operator {
  public:
    Not(const Expression* rhs) : Unary_operator(rhs){};
    virtual const Constant* evaluate() const;
    virtual GPL::Type type() const;
};
class Sin : public Unary_operator {
  public:
    Sin(const Expression* rhs) : Unary_operator(rhs){};
    virtual const Constant* evaluate() const;
    virtual GPL::Type type() const;
};
class Cos : public Unary_operator {
  public:
    Cos(const Expression* rhs) : Unary_operator(rhs){};
    virtual const Constant* evaluate() const;
    virtual GPL::Type type() const;
};

class Tan : public Unary_operator {
  public:
    Tan(const Expression* rhs) : Unary_operator(rhs){};
    virtual const Constant* evaluate() const;
    virtual GPL::Type type() const;
};
class Asin : public Unary_operator {
  public:
    Asin(const Expression* rhs) : Unary_operator(rhs){};
    virtual const Constant* evaluate() const;
    virtual GPL::Type type() const;
};
class Acos : public Unary_operator {
  public:
    Acos(const Expression* rhs) : Unary_operator(rhs){};
    virtual const Constant* evaluate() const;
    virtual GPL::Type type() const;
};
class Atan : public Unary_operator {
  public:
    Atan(const Expression* rhs) : Unary_operator(rhs){};
    virtual const Constant* evaluate() const;
    virtual GPL::Type type() const;
};
class Sqrt : public Unary_operator {
  public:
    Sqrt(const Expression* rhs) : Unary_operator(rhs){};
    virtual const Constant* evaluate() const;
    virtual GPL::Type type() const;
};
class Abs : public Unary_operator {
  public:
    Abs(const Expression* rhs) : Unary_operator(rhs){};
    virtual const Constant* evaluate() const;
    virtual GPL::Type type() const;
};
class Floor : public Unary_operator {
  public:
    Floor(const Expression* rhs) : Unary_operator(rhs){};
    virtual const Constant* evaluate() const;
    virtual GPL::Type type() const;
};
class Random : public Unary_operator {
  public:
    Random(const Expression* rhs) : Unary_operator(rhs){};
    virtual const Constant* evaluate() const;
    virtual GPL::Type type() const;
};
#endif
