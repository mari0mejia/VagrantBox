#ifndef WEAPON_H
#define WEAPON_H

#include<string>

class Weapon
{
  private:
    std::string type;
    int stamina_required;
    int hit_chance;
  public:
    Weapon(std::string wt,int sr,int hc); // constructor
    int get_stamina_required();
    bool did_you_hit();
    void display();
};
#endif
