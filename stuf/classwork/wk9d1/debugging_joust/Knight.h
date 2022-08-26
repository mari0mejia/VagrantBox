#ifndef KNIGHT_H
#define KNIGHT_H
#include<string>
#include "Weapon.h"


class Knight
{
  private:
    std::string name;
    int stamina;
    bool on_horse;
    Weapon weapon_in_hand;

  public:
    Knight(std::string kn,int s,std::string wt,int sr,int hc); // constructor
    bool are_you_on_your_horse();
    bool are_you_exhausted();
    bool wield();
    void unhorse_yourself();
    void display();
};
#endif
