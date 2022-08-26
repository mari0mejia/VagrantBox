#ifndef DATE_H
#define DATE_H
#include<iostream>
//DATE CLASS DECLARATION

class Date {
  public:
    Date(int m, int d, int y); //constructor
    ~Date(); //destructor
    void add_day();

    friend std::ostream& operator<<(std::ostream& os, const Date& dt);

    //Don't allow compiler to generate these functions
    Date& operator=(const Date&) = delete;
    Date(const Date&) = delete;
  private:
    int month;
    int day;
    int year;
};
#endif
