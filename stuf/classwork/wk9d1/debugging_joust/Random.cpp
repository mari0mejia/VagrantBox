// File:  Random.cpp
// Version: 1.4

#include <cstdlib>
#include <ctime>


#include "Random.h"
// class definition

int Random::seedcount=0;

void Random::seed(void)
{
  if(seedcount) return; //we only want to seed once
  seedcount=1;
  srand((int)time(0)); //seeds the random number generator
}
//# 13 "project_joust.cpp" 3
 // string
 //
Random::Random(int l, int h) : low(l), diff(abs(h-l)+1)
{
  seed();
}

int Random::get(void) const
{
  return int((float)rand()/RAND_MAX*diff+low);
}
