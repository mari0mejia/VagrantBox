#include<iostream>
#include<string>
#include<iomanip>

#include "Random.h"
#include "Weapon.h"
using namespace std;


// Function Definitions
Weapon::Weapon(string wt,int sr,int hc) // 
: type(wt), stamina_required(sr), hit_chance(hc)
{
  if(hc < 0 || hc > 100)
    hit_chance = 10;
}


int Weapon::get_stamina_required() // 
{
  return stamina_required;
}


bool Weapon::did_you_hit()
{
  int rn;
  Random Random_number(0,100);

  rn = Random_number.get(); // will return a number between 0-100

  if(rn <= hit_chance) // if the number is within range of your hit change than the hit is true
    return true;
  else
    return false;
}


void Weapon::display() // 
{
  cout << "Weapon type: " << type << endl;
  cout << string(8,' ') << "Stamina required: " << stamina_required << endl;
  cout << string(8,' ') << "Hit chance: " << hit_chance << endl;
}
