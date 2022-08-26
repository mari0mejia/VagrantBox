// File: Expression.cpp
#include "Expression.h"
#include "error.h"
#include "Constant.h"
#include "Scope_manager.h"
#include <string>
#include <cmath>
using std::string;
//#include<numbers>
using std::numbers::pi; 
Expression::~Expression() { delete evaluated_constant; }

// This wrapper ensures that Constant objects created by evaluate() are
// properly released when a subsequent call to evaluate() will otherwise
// overwrite the pointer.
const Constant *Expression::ret(const Constant *new_evald_constant) const
{
  delete evaluated_constant;
  return evaluated_constant = new_evald_constant;
}
/**********************************************/
/************* Other operators ****************/
/**********************************************/

const Constant *Plus::evaluate() const
{
  GPL::Type lht = lhs->type();
  GPL::Type rht = rhs->type();
  const Constant *lhs_constant = lhs->evaluate();
  const Constant *rhs_constant = rhs->evaluate();
  if (lht == GPL::STRING || rht == GPL::STRING)
    return ret(new String_constant(lhs_constant->as_string() + //<--addition
                                   rhs_constant->as_string()));
  if (lht == GPL::DOUBLE || rht == GPL::DOUBLE)
    return ret(new Double_constant(lhs_constant->as_double() + //<--addition
                                   rhs_constant->as_double()));
  return ret(new Integer_constant(lhs_constant->as_int() + //<--addition
                                  rhs_constant->as_int()));

}

GPL::Type Plus::type() const
{
  GPL::Type lht = lhs->type();
  GPL::Type rht = rhs->type();
  if (lht == GPL::STRING || rht == GPL::STRING)
    return GPL::STRING;
  if (lht == GPL::DOUBLE || rht == GPL::DOUBLE)
    return GPL::DOUBLE;
  return GPL::INT;
  // return GPL::NO_TYPE; //REPLACE THIS. Here to suppress errors.
}
/*###########################*/
/*###########################*/
/*    MULTIPLY RULE          */
/*###########################*/
/*###########################*/


const Constant *Multiply::evaluate() const
{
  const Constant* lhs_constant=lhs->evaluate();
  const Constant* rhs_constant=rhs->evaluate();
  if(lhs->type()==GPL::DOUBLE || rhs->type()==GPL::DOUBLE)
    return ret(new Double_constant(lhs_constant->as_double() * //<--multiply
                                   rhs_constant->as_double()));
  return ret(new Integer_constant(lhs_constant->as_int() *     //<--multiply
                                  rhs_constant->as_int()));
  //return nullptr; // REPLACE THIS. Here to suppress errors.
}

GPL::Type Multiply::type() const
{
  GPL::Type lht=lhs->type();
  GPL::Type rht=rhs->type();
  if(lht==GPL::DOUBLE || rht==GPL::DOUBLE)
    return GPL::DOUBLE;
  return GPL::INT;
  //return GPL::NO_TYPE; // REPLACE THIS. Here to suppress errors.
}
/*###########################*/
/*###########################*/
/*       MINUS RULE          */
/*###########################*/
/*###########################*/


const Constant *Minus::evaluate() const
{
  const Constant* lhs_constant=lhs->evaluate();
  const Constant* rhs_constant=rhs->evaluate();
  if(lhs->type()==GPL::DOUBLE || rhs->type()==GPL::DOUBLE)
    return ret(new Double_constant(lhs_constant->as_double() - //<--Subtract
                                   rhs_constant->as_double()));
  return ret(new Integer_constant(lhs_constant->as_int() -     //<--Subtract
                                  rhs_constant->as_int()));
  //return nullptr; // REPLACE THIS. Here to suppress errors.
}

GPL::Type Minus::type() const
{
  GPL::Type lht=lhs->type();
  GPL::Type rht=rhs->type();
  if(lht==GPL::DOUBLE || rht==GPL::DOUBLE)
    return GPL::DOUBLE;
  return GPL::INT;
  //return GPL::NO_TYPE; // REPLACE THIS. Here to suppress errors.
}
/*###########################*/
/*###########################*/
/*       DIVIDE RULE         */
/*###########################*/
/*###########################*/


const Constant *Divide::evaluate() const
{
  const Constant* lhs_constant=lhs->evaluate();
  const Constant* rhs_constant=rhs->evaluate();
  if(lhs->type()==GPL::DOUBLE || rhs->type()==GPL::DOUBLE)
    return ret(new Double_constant(lhs_constant->as_double() / //<--Divide
                                   rhs_constant->as_double()));
  return ret(new Integer_constant(lhs_constant->as_int() /     //<--Divide
                                  rhs_constant->as_int()));
  //return nullptr; // REPLACE THIS. Here to suppress errors.
}

GPL::Type Divide::type() const
{
  GPL::Type lht=lhs->type();
  GPL::Type rht=rhs->type();
  if(lht==GPL::DOUBLE || rht==GPL::DOUBLE)
    return GPL::DOUBLE;
  return GPL::INT;
}
/*###########################*/
/*###########################*/
/*       MOD RULE            */
/*###########################*/
/*###########################*/


const Constant *Mod::evaluate() const
{
  const Constant* lhs_constant=lhs->evaluate();
  const Constant* rhs_constant=rhs->evaluate();
  return ret(new Integer_constant(lhs_constant->as_int() %     //<--Or
                                  rhs_constant->as_int()));
}


GPL::Type Mod::type() const
{
  return GPL::INT;
}
/*###########################*/
/*###########################*/
/*        OR RULE            */
/*###########################*/
/*###########################*/


const Constant *Or::evaluate() const
{
  const Constant* lhs_constant=lhs->evaluate();
  const Constant* rhs_constant=rhs->evaluate();
  if(lhs->type()==GPL::DOUBLE || rhs->type()==GPL::DOUBLE)
    return ret(new Integer_constant(lhs_constant->as_double() || //<--Or
                                   rhs_constant->as_double()));
  return ret(new Integer_constant(lhs_constant->as_int() ||     //<--Or
                                  rhs_constant->as_int()));
  //return nullptr; // REPLACE THIS. Here to suppress errors.
}

GPL::Type Or::type() const
{
  GPL::Type lht=lhs->type();
  GPL::Type rht=rhs->type();
  if(lht==GPL::DOUBLE || rht==GPL::DOUBLE)
    return GPL::DOUBLE;
  return GPL::INT;//return GPL::NO_TYPE; // REPLACE THIS. Here to suppress errors.
}
/*###########################*/
/*###########################*/
/*        AND RULE            */
/*###########################*/
/*###########################*/


const Constant *And::evaluate() const
{
  const Constant* lhs_constant=lhs->evaluate();
  const Constant* rhs_constant=rhs->evaluate();
  if(lhs->type()==GPL::DOUBLE || rhs->type()==GPL::DOUBLE)
    return ret(new Integer_constant(lhs_constant->as_double() && //<--And
                                   rhs_constant->as_double()));
  return ret(new Integer_constant(lhs_constant->as_int() &&     //<--And
                                  rhs_constant->as_int()));//return nullptr; // REPLACE THIS. Here to suppress errors.
}

GPL::Type And::type() const
{
  GPL::Type lht=lhs->type();
  GPL::Type rht=rhs->type();
  if(lht==GPL::DOUBLE || rht==GPL::DOUBLE)
    return GPL::DOUBLE;
  return GPL::INT;
  //return GPL::NO_TYPE; // REPLACE THIS. Here to suppress errors.
}
/*###########################*/
/*###########################*/
/*        LESS EQUAL         */
/*###########################*/
/*###########################*/


const Constant *Less_equal::evaluate() const
{
GPL::Type lht = lhs->type();
  GPL::Type rht = rhs->type();
  const Constant *lhs_constant = lhs->evaluate();
  const Constant *rhs_constant = rhs->evaluate();
  if (lht == GPL::STRING || rht == GPL::STRING)
    return ret(new Integer_constant(lhs_constant->as_string() <= //<--less than or equal
                                   rhs_constant->as_string()));
  if (lht == GPL::DOUBLE || rht == GPL::DOUBLE)
    return ret(new Integer_constant(lhs_constant->as_double() <= //less than or equal
                                   rhs_constant->as_double()));
  return ret(new Integer_constant(lhs_constant->as_int() <= //<--Less than or equal
                                  rhs_constant->as_int()));


}

GPL::Type Less_equal::type() const
{
  GPL::Type lht = lhs->type();
  GPL::Type rht = rhs->type();
  if (lht == GPL::STRING || rht == GPL::STRING)
    return GPL::STRING;
  if (lht == GPL::DOUBLE || rht == GPL::DOUBLE)
    return GPL::DOUBLE;
  return GPL::INT;
  //return GPL::NO_TYPE; // REPLACE THIS. Here to suppress errors.
}
/*###########################*/
/*###########################*/
/*        GREATER EQUAL      */
/*###########################*/
/*###########################*/

const Constant *Greater_equal::evaluate() const
{
GPL::Type lht = lhs->type();
  GPL::Type rht = rhs->type();
  const Constant *lhs_constant = lhs->evaluate();
  const Constant *rhs_constant = rhs->evaluate();
  if (lht == GPL::STRING || rht == GPL::STRING)
    return ret(new Integer_constant(lhs_constant->as_string() >= //<--Greater than or equal
                                   rhs_constant->as_string()));
  if (lht == GPL::DOUBLE || rht == GPL::DOUBLE)
    return ret(new Integer_constant(lhs_constant->as_double() >= //greater than or equal
                                   rhs_constant->as_double()));
  return ret(new Integer_constant(lhs_constant->as_int() >= //<--Greater than or equal
                                  rhs_constant->as_int()));


}

GPL::Type Greater_equal::type() const
{
  GPL::Type lht = lhs->type();
  GPL::Type rht = rhs->type();
  if (lht == GPL::STRING || rht == GPL::STRING)
    return GPL::STRING;
  if (lht == GPL::DOUBLE || rht == GPL::DOUBLE)
    return GPL::DOUBLE;
  return GPL::INT;
  //return GPL::NO_TYPE; // REPLACE THIS. Here to suppress errors.
}
/*###########################*/
/*###########################*/
/*        LESS THAN          */
/*###########################*/
/*###########################*/

const Constant *Less_than::evaluate() const
{
GPL::Type lht = lhs->type();
  GPL::Type rht = rhs->type();
  const Constant *lhs_constant = lhs->evaluate();
  const Constant *rhs_constant = rhs->evaluate();
  if (lht == GPL::STRING || rht == GPL::STRING)
    return ret(new Integer_constant(lhs_constant->as_string() < //<--less than 
                                   rhs_constant->as_string()));
  if (lht == GPL::DOUBLE || rht == GPL::DOUBLE)
    return ret(new Integer_constant(lhs_constant->as_double() < //less than 
                                   rhs_constant->as_double()));
  return ret(new Integer_constant(lhs_constant->as_int() < //<--Less than 
                                  rhs_constant->as_int()));


}

GPL::Type Less_than::type() const
{
  GPL::Type lht = lhs->type();
  GPL::Type rht = rhs->type();
  if (lht == GPL::STRING || rht == GPL::STRING)
    return GPL::STRING;
  if (lht == GPL::DOUBLE || rht == GPL::DOUBLE)
    return GPL::DOUBLE;
  return GPL::INT;
  //return GPL::NO_TYPE; // REPLACE THIS. Here to suppress errors.
}
/*###########################*/
/*###########################*/
/*     GREATER THAN          */
/*###########################*/
/*###########################*/

const Constant *Greater_than::evaluate() const
{
GPL::Type lht = lhs->type();
  GPL::Type rht = rhs->type();
  const Constant *lhs_constant = lhs->evaluate();
  const Constant *rhs_constant = rhs->evaluate();
  if (lht == GPL::STRING || rht == GPL::STRING)
    return ret(new Integer_constant(lhs_constant->as_string() > //<--Greater than 
                                   rhs_constant->as_string()));
  if (lht == GPL::DOUBLE || rht == GPL::DOUBLE)
    return ret(new Integer_constant(lhs_constant->as_double() > //Greater than 
                                   rhs_constant->as_double()));
  return ret(new Integer_constant(lhs_constant->as_int() > //<--Greater than 
                                  rhs_constant->as_int()));


}

GPL::Type Greater_than::type() const
{
  GPL::Type lht = lhs->type();
  GPL::Type rht = rhs->type();
  if (lht == GPL::STRING || rht == GPL::STRING)
    return GPL::STRING;
  if (lht == GPL::DOUBLE || rht == GPL::DOUBLE)
    return GPL::DOUBLE;
  return GPL::INT;
  //return GPL::NO_TYPE; // REPLACE THIS. Here to suppress errors.
}
/*###########################*/
/*###########################*/
/*            Equal          */
/*###########################*/
/*###########################*/

const Constant *Equal::evaluate() const
{
GPL::Type lht = lhs->type();
  GPL::Type rht = rhs->type();
  const Constant *lhs_constant = lhs->evaluate();
  const Constant *rhs_constant = rhs->evaluate();
  if (lht == GPL::STRING || rht == GPL::STRING)
    return ret(new Integer_constant(lhs_constant->as_string() == //<--Not equal
                                   rhs_constant->as_string()));
  if (lht == GPL::DOUBLE || rht == GPL::DOUBLE)
    return ret(new Integer_constant(lhs_constant->as_double() == //Not equal
                                   rhs_constant->as_double()));
  return ret(new Integer_constant(lhs_constant->as_int() == //<--Not equal 
                                  rhs_constant->as_int()));


}

GPL::Type Equal::type() const
{
  GPL::Type lht = lhs->type();
  GPL::Type rht = rhs->type();
  if (lht == GPL::STRING || rht == GPL::STRING)
    return GPL::STRING;
  if (lht == GPL::DOUBLE || rht == GPL::DOUBLE)
    return GPL::DOUBLE;
  return GPL::INT;
  //return GPL::NO_TYPE; // REPLACE THIS. Here to suppress errors.
}
/*###########################*/
/*###########################*/
/*       NOT EQUAL           */
/*###########################*/
/*###########################*/

const Constant *Not_equal::evaluate() const
{
GPL::Type lht = lhs->type();
  GPL::Type rht = rhs->type();
  const Constant *lhs_constant = lhs->evaluate();
  const Constant *rhs_constant = rhs->evaluate();
  if (lht == GPL::STRING || rht == GPL::STRING)
    return ret(new Integer_constant(lhs_constant->as_string() != //<--Not equal 
                                   rhs_constant->as_string()));
  if (lht == GPL::DOUBLE || rht == GPL::DOUBLE)
    return ret(new Integer_constant(lhs_constant->as_double() != //Not Equal 
                                   rhs_constant->as_double()));
  return ret(new Integer_constant(lhs_constant->as_int() != //<--Not equal 
                                  rhs_constant->as_int()));


}

GPL::Type Not_equal::type() const
{
  GPL::Type lht = lhs->type();
  GPL::Type rht = rhs->type();
  if (lht == GPL::STRING || rht == GPL::STRING)
    return GPL::STRING;
  if (lht == GPL::DOUBLE || rht == GPL::DOUBLE)
    return GPL::DOUBLE;
  return GPL::INT;
  //return GPL::NO_TYPE; // REPLACE THIS. Here to suppress errors.
}
/*###########################*/
/*###########################*/
/*       Unary Minus         */
/*###########################*/
/*###########################*/

const Constant *Unary_minus::evaluate() const
{
  GPL::Type rht = rhs->type();
  const Constant *rhs_constant = rhs->evaluate();
  if (rht == GPL::DOUBLE)
    return ret(new Double_constant((-1)*rhs_constant->as_double()));
  return ret(new Integer_constant((-1)*rhs_constant->as_int()));


}

GPL::Type Unary_minus::type() const
{
  GPL::Type rht = rhs->type();
  if (rht == GPL::DOUBLE)
    return GPL::DOUBLE;
  return GPL::INT;
  //return GPL::NO_TYPE; // REPLACE THIS. Here to suppress errors.
}
/*###########################*/
/*###########################*/
/*       Unary Not           */
/*###########################*/
/*###########################*/

const Constant *Not::evaluate() const
{
  GPL::Type rht = rhs->type();
  const Constant *rhs_constant = rhs->evaluate();
  if (rht == GPL::DOUBLE)
    return ret(new Integer_constant(!(rhs_constant->as_double())));
  if (rht == GPL::STRING)
    return ret(new Integer_constant(!(rhs_constant->as_int())));
  return ret(new Integer_constant(!(rhs_constant->as_int())));


}

GPL::Type Not::type() const
{
  GPL::Type rht = rhs->type();
  if (rht == GPL::DOUBLE)
    return GPL::DOUBLE;
  if (rht == GPL::STRING)
    return GPL::STRING;
  return GPL::INT;
  //return GPL::NO_TYPE; // REPLACE THIS. Here to suppress errors.
}
/*###########################*/
/*###########################*/
/*         SIN               */
/*###########################*/
/*###########################*/

const Constant *Sin::evaluate() const
{
  GPL::Type rht = rhs->type();
  const Constant *rhs_constant = rhs->evaluate();
  if (rht == GPL::DOUBLE)
    return ret(new Double_constant(sin(rhs_constant->as_double()*pi/180)));
  return ret(new Double_constant(sin(rhs_constant->as_int()*pi/180)));


}

GPL::Type Sin::type() const
{
  GPL::Type rht = rhs->type();
  if (rht == GPL::DOUBLE)
    return GPL::DOUBLE;
  return GPL::INT;
  //return GPL::NO_TYPE; // REPLACE THIS. Here to suppress errors.
}
/*###########################*/
/*###########################*/
/*         COS               */
/*###########################*/
/*###########################*/

const Constant *Cos::evaluate() const
{
  GPL::Type rht = rhs->type();
  const Constant *rhs_constant = rhs->evaluate();
  if (rht == GPL::DOUBLE)
    return ret(new Double_constant(cos(rhs_constant->as_double()*pi/180)));
  return ret(new Double_constant(cos(rhs_constant->as_int()*pi/180)));


}

GPL::Type Cos::type() const
{
  GPL::Type rht = rhs->type();
  if (rht == GPL::DOUBLE)
    return GPL::DOUBLE;
  return GPL::INT;
  //return GPL::NO_TYPE; // REPLACE THIS. Here to suppress errors.
}
/*###########################*/
/*###########################*/
/*         TAN               */
/*###########################*/
/*###########################*/

const Constant *Tan::evaluate() const
{
  GPL::Type rht = rhs->type();
  const Constant *rhs_constant = rhs->evaluate();
  if (rht == GPL::DOUBLE)
    return ret(new Double_constant(tan(rhs_constant->as_double()*pi/180)));
  return ret(new Double_constant(tan(rhs_constant->as_int()*pi/180)));


}

GPL::Type Tan::type() const
{
  GPL::Type rht = rhs->type();
  if (rht == GPL::DOUBLE)
    return GPL::DOUBLE;
  return GPL::INT;
  //return GPL::NO_TYPE; // REPLACE THIS. Here to suppress errors.
}

/*###########################*/
/*###########################*/
/*         ASIN              */
/*###########################*/
/*###########################*/

const Constant *Asin::evaluate() const
{
  GPL::Type rht = rhs->type();
  const Constant *rhs_constant = rhs->evaluate();
  if (rht == GPL::DOUBLE)
    return ret(new Double_constant(asin(rhs_constant->as_double())));
  return ret(new Double_constant(asin(rhs_constant->as_int())));


}

GPL::Type Asin::type() const
{
  GPL::Type rht = rhs->type();
  if (rht == GPL::DOUBLE)
    return GPL::DOUBLE;
  return GPL::INT;
  //return GPL::NO_TYPE; // REPLACE THIS. Here to suppress errors.
}

/*###########################*/
/*###########################*/
/*         ACOS              */
/*###########################*/
/*###########################*/

const Constant *Acos::evaluate() const
{
  GPL::Type rht = rhs->type();
  const Constant *rhs_constant = rhs->evaluate();
  if (rht == GPL::DOUBLE)
    return ret(new Double_constant(acos(rhs_constant->as_double())));
  return ret(new Double_constant(acos(rhs_constant->as_int())));


}

GPL::Type Acos::type() const
{
  GPL::Type rht = rhs->type();
  if (rht == GPL::DOUBLE)
    return GPL::DOUBLE;
  return GPL::INT;
  //return GPL::NO_TYPE; // REPLACE THIS. Here to suppress errors.
}
/*###########################*/
/*###########################*/
/*         ATAN              */
/*###########################*/
/*###########################*/

const Constant *Atan::evaluate() const
{
  GPL::Type rht = rhs->type();
  const Constant *rhs_constant = rhs->evaluate();
  if (rht == GPL::DOUBLE)
    return ret(new Double_constant(atan(rhs_constant->as_double())*180/pi));
  return ret(new Double_constant(atan(rhs_constant->as_int())*180/pi));


}

GPL::Type Atan::type() const
{
  GPL::Type rht = rhs->type();
  if (rht == GPL::DOUBLE)
    return GPL::DOUBLE;
  return GPL::INT;
  //return GPL::NO_TYPE; // REPLACE THIS. Here to suppress errors.
}
/*###########################*/
/*###########################*/
/*          SQRT             */
/*###########################*/
/*###########################*/

const Constant *Sqrt::evaluate() const
{
  GPL::Type rht = rhs->type();
  const Constant *rhs_constant = rhs->evaluate();
  if (rht == GPL::DOUBLE)
    return ret(new Double_constant(sqrt(rhs_constant->as_double())));
  return ret(new Double_constant(sqrt(rhs_constant->as_int())));


}

GPL::Type Sqrt::type() const
{
  GPL::Type rht = rhs->type();
  if (rht == GPL::DOUBLE)
    return GPL::DOUBLE;
  return GPL::INT;
  //return GPL::NO_TYPE; // REPLACE THIS. Here to suppress errors.
}
/*###########################*/
/*###########################*/
/*          ABS              */
/*###########################*/
/*###########################*/
const Constant *Abs::evaluate() const
{
  GPL::Type rht = rhs->type();
  const Constant *rhs_constant = rhs->evaluate();
  if (rht == GPL::DOUBLE)
    return ret(new Double_constant(fabs(rhs_constant->as_double())));
  return ret(new Integer_constant(fabs(rhs_constant->as_int())));


}

GPL::Type Abs::type() const
{
  GPL::Type rht = rhs->type();
  if (rht == GPL::DOUBLE)
    return GPL::DOUBLE;
  return GPL::INT;
  //return GPL::NO_TYPE; // REPLACE THIS. Here to suppress errors.
}
/*###########################*/
/*###########################*/
/*          FLOOR            */
/*###########################*/
/*###########################*/
const Constant *Floor::evaluate() const
{
  GPL::Type rht = rhs->type();
  const Constant *rhs_constant = rhs->evaluate();
  if (rht == GPL::DOUBLE)
    return ret(new Integer_constant(floor(rhs_constant->as_double())));
  return ret(new Integer_constant(floor(rhs_constant->as_int())));


}

GPL::Type Floor::type() const
{
  GPL::Type rht = rhs->type();
  if (rht == GPL::DOUBLE)
    return GPL::DOUBLE;
  return GPL::INT;
  //return GPL::NO_TYPE; // REPLACE THIS. Here to suppress errors.
}
/*###########################*/
/*###########################*/
/*          RANDOM           */
/*###########################*/
/*###########################*/
const Constant *Random::evaluate() const
{
  GPL::Type rht = rhs->type();
  const Constant *rhs_constant = rhs->evaluate();

  if (rht == GPL::DOUBLE)
  {
    if(rhs_constant->as_double() < 2) 
    {
      
      int n = 2;
      return ret(new Integer_constant((rand()%n)));

    }
    return ret(new Integer_constant((rand()%lround(rhs_constant->as_double()))));
  }
  if(rhs_constant->as_int() < 2) 
    {
      
      int n = 2;
      return ret(new Integer_constant((rand()%n)));
    }
  return ret(new Integer_constant((rand()%rhs_constant->as_int())));

}

GPL::Type Random::type() const
{
  GPL::Type rht = rhs->type();
  if (rht == GPL::DOUBLE)
    return GPL::DOUBLE;
  return GPL::INT;
  //return GPL::NO_TYPE; // REPLACE THIS. Here to suppress errors.
}

