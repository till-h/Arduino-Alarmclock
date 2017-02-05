/*
  DotMatrix.h

  Library for controlling the 3 by 8x8 dot matrix display.

  Created by Till Hackler, December 2015

*/

#ifndef DOTMATRIX_H
#define DOTMATRIX_H

#include "Arduino.h"
#include "LedControl.h"

// display font for numbers
// allow to set via preprocessor to save program memory
// #define CALLIGRAPHY
#define SANS

class DotMatrix
{
public:
    DotMatrix();
    void setup(uint8_t dta, uint8_t clk, uint8_t cs, uint8_t num, uint8_t brightness);
    void displayTime(uint8_t hour, uint8_t minute);
    void displayAlarm(bool status);
private:
    void setColumn(uint8_t col, byte value);
    LedControl lc;
    int8_t            last_hour = 100;
    int8_t            last_minute = 100;
    static byte       const n[10][5];
    static byte       const clock[9];
    static byte       const onoff[2][15];
};

#endif // DOTMATRIX
