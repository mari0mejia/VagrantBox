#include "Expression.h"
#include "Symbol.h"
#include "Scope_manager.h"
#include "Variable.h"
#include "Constant.h"
#include "error.h"
const Constant *Variable::evaluate() const
{
  
    const Constant *array_size = array_index_expression->evaluate();
    if(symbol_name == "")
        return ret(new Integer_constant(0));
    if(array_size->as_int() > 0)
        return ret(symbol()->as_constant(array_size->as_int()));
    if(array_index_expression == nullptr)
        Error::error(Error::ARRAY_INDEX_OUT_OF_BOUNDS,to_string(array_size->as_int()),symbol_name); 
    return ret(symbol()->as_constant());

}

GPL::Type Variable::type() const
{
    if(symbol_name == "")
        return GPL::INT;

    return symbol()->get_type();
}
Variable::~Variable() {delete array_index_expression; }
//Expression::~Expression() { delete evaluated_constant; }


