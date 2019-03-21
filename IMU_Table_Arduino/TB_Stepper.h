#ifndef TB_Stepper_h
#define TB_Stepper_h

#include "Arduino.h"
//extern bool dir = true;     // Make direction variable external

// Define stepper motor x pins
//#define PUL_X      6  //define Pulse pin for x axis
//#define DIR_X      5  //define Direction pin for x axis
//#define EN_X       4  //define Enable Pin for x axis

// Define XY_plotter class
class TB_Stepper {
public:
  TB_Stepper(int pul = 6, int dir = 5, int en = 4);
  void init();
  void step(int steps, int rpm);

private:
  int pul;
  int dir;
  int en;

};

#endif
