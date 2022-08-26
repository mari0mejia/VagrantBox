#ifndef _RANDOM_H_
#define _RANDOM_H_
// File:  Random.h
// Version: 1.4
// class declaration

class Random {
  public:
    Random(int low, int high); //constructor
    int get(void) const;
  private:
    int low;
    int diff;
    static void seed(void);
    static int seedcount;
};
#endif
