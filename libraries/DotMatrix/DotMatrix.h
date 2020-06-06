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
// set via preprocessor to save program memory
#define CALLIGRAPHY
// #define SANS

// TODO use setRow instead of setColumn. Faster. Will require assembling dot raster before setting.
// TODO Any display-changing functions should compare what they are asked to show with what is already being shown.
// Across all matrices, only rows which have changed should be updated (I seem to remember setRow is as fast as setLed).
class DotMatrix
{
public:
    DotMatrix();
    void setup(uint8_t dta, uint8_t clk, uint8_t cs, uint8_t num,
               uint8_t brightness = 0, uint32_t interval = 1000000);
    void displayTime(uint8_t hour, uint8_t minute, bool force_update = false);
    void blinkTime(uint8_t hour, uint8_t minute);
    void displayAlarm(bool status);
private:
    void setColumn(uint8_t col, byte value);
    void clear();
    LedControl lc;
    int8_t            last_hour = 100;
    int8_t            last_minute = 100;
    static byte       const n[10][5];
    static byte       const clock[9];
    static byte       const onoff[2][15];
    uint32_t _interval;
    uint32_t _lastFlank;
    bool _showing;
};

#endif // DOTMATRIX
