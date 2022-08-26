//File: Constant.cpp
#include "Constant.h"

GPL::Type  Constant::type() const 
{ return intrinsic_type; }

const Constant* Constant::evaluate() const
{ return this; }
