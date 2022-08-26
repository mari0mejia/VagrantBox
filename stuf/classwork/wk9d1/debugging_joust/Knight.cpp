/* Program Preamble
 * Name: XXXXXXXXXXX
 * Project: 2 
 * Class: CSCI 111
 * Program Name: source file for Knight Joust Game "knight.cpp"
 */

//Pre-processor Directives
#include<iostream>
#include<string>
#include<iomanip>


#include "Weapon.h"
#include "Knight.h"
using namespace std;

// Function Definitions
  Knight::Knight(string kn,int s,string wt,int sr,int hc)
: name(kn), stamina(s), on_horse(true), weapon_in_hand(wt,sr,hc)
{
}
bool Knight::are_you_exhausted() // 
{
  if(stamina > 0)
    return false;
  else
    return true;
}
bool Knight::wield() // pass in weapon type
{
  stamina = stamina - weapon_in_hand.get_stamina_required();
  return weapon_in_hand.did_you_hit();
}


void Knight::unhorse_yourself()
{
  on_horse = false;
}

bool Knight::are_you_on_your_horse()
{
  return on_horse;
}


void Knight::display()
{
  string on_horse_stat;
  string exhausted_stat;

  if(stamina <= 0)
    exhausted_stat = " is exhausted";
  else
    exhausted_stat = " is not exhausted";

      if(on_horse == true)
        on_horse_stat = " is on the horse";
      else
        on_horse_stat = " has been knocked off the horse";

  cout << string(8,' ') << name << exhausted_stat
    << "( " << stamina << " )" << endl
    << string(8,' ') << name << on_horse_stat << endl;
    weapon_in_hand.display();
  cout << endl;
}


