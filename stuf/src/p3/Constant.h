//File: Constant.h
#ifndef CONSTANT_H
#define CONSTANT_H
#include "Expression.h"
#include "types_and_ops.h"
class Constant : public Expression {
  public:
    Constant(GPL::Type enum_name) : intrinsic_type(enum_name) {}
    virtual const Constant*   evaluate() const final;
    virtual int         as_int()    const {throw intrinsic_type;}
    virtual double      as_double() const {throw intrinsic_type;}
    virtual std::string as_string() const {throw intrinsic_type;}
    virtual GPL::Type   type()      const final;
    virtual ~Constant(){}//virtual dtor to ensure derived data is released properly, i.e., string
  private:
    GPL::Type intrinsic_type;
};

class Integer_constant : public Constant {
  public:
    Integer_constant(int d) : Constant(GPL::INT), data(d) {}
    virtual int         as_int()    const { return data; }
    virtual double      as_double() const { return data; }
    virtual std::string as_string() const { return std::to_string(data);}
  private:
    int data;
};

class Double_constant : public Constant {
  public:
    Double_constant(double d) : Constant(GPL::DOUBLE), data(d) {}
    virtual double      as_double() const { return data; }
    virtual std::string as_string() const { return std::to_string(data);}
  private:
    double data;
};

class String_constant : public Constant {
  public:
    String_constant(const std::string& d) : Constant(GPL::STRING), data(d) {}
    virtual std::string as_string() const { return data;}
  private:
    std::string data;
};

#endif
