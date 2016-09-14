/*
  DotMatrix.h

  Library for controlling the 3 by 8x8 dot matrix display.

  Created by Till Hackler, December 2015

*/

#ifndef DOTMATRIX_H
#define DOTMATRIX_H

#include "Arduino.h"
#include "LedControl.h"

class DotMatrix
{
public:
    DotMatrix();
    void setup(int dta, int clk, int cs, int num);
    void displayTime(uint8_t hour, uint8_t minute);
    void displayAlarm(bool status);
private:
    void setColumn(uint8_t col, byte value);
    LedControl lc;
    uint8_t           last_hour = 100;
    uint8_t           last_minute = 100;
    unsigned long     last_update;
    static byte       const n[10][5];
    static byte       const clock[9];
    static byte       const onoff[2][15];
};

#endif // DOTMATRIX
