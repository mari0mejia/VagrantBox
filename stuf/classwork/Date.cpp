#include"Date.h"
#include<iostream>

//DATE CLASS DEFINITION
Date::Date(int m, int d, int y) //constructor
  : month(m), day(d), year(y) //initialization list
{
}

void Date::add_day()
{
  ++day;
}
std::ostream& operator<<(std::ostream& os, const Date& dt)
{
  os << dt.month << '/' << dt.day << '/' << dt.year;
  return os;
}

Date::~Date()
{
  std::cerr << "Destructing " << *this << "\n";
}
