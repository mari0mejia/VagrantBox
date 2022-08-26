#include <iostream>
#include <string>
#include <iomanip>
#include "Knight.h"
using namespace std;

int main()  
{

  string kn;
  int s;
  string wt;
  int sr;
  int hc;

  cout << "Enter the first Knight's name. " << endl;
  getline(cin,kn);

  cout << "Enter the Knight's stamina. " << endl;
  cin >> s;

  cout << "Enter the Knight's weapon type " << endl;
  cin.ignore();
  getline(cin, wt);

  cout << "Enter the hit chance of the weapon (must be between 0-100). " << endl;
  cin >> hc;

  cout << "Enter the stamina required to wield that weapon " << endl;
  cin >> sr;

  Knight Knight_One(kn,s,wt,sr,hc);
  cout << "Enter the second Knight's name. " << endl;
  cin.ignore();
  getline(cin, kn);

  cout << "Enter the Knight's stamina. " << endl;
  cin >> s;

  cout << "Enter the Knight's weapon type " << endl;
  cin.ignore();
  getline(cin, wt);

  cout << "Enter the hit chance of the weapon (must be between 0-100). " << endl;
  cin >> hc;

  cout << "Enter the stamina required to wield that weapon " << endl;
  cin >> sr;

  Knight Knight_Two(kn,s,wt,sr,hc);

  cout << "//////////// Joust Statistics ////////////\n";

  Knight_One.display();

  Knight_Two.display();

  // begin joust
  int round=1;

  do{

    if(Knight_One.wield()==true)
      Knight_Two.unhorse_yourself();

    if(Knight_Two.wield()==true)
      Knight_One.unhorse_yourself();

    cout << "//////////// Round " << round << " ////////////\n";

    Knight_One.display();
    Knight_Two.display();

    round++;
  }while(Knight_One.are_you_exhausted() == false && 
      Knight_Two.are_you_exhausted() == false && 
      Knight_One.are_you_on_your_horse()==true && 
      Knight_Two.are_you_on_your_horse()==true);
  return 0;
//# 32 "project_joust.cpp" 3
}//main
