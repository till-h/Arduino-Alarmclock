/*
  DotMatrix.h

  Library for controlling the 3 by 8x8 dot matrix display.

  Created by Till Hackler, December 2015

*/

#ifndef DOTMATRIX_H
#define DOTMATRIX_H

#include "Arduino.h"
#include "LedControl.h"
#include "ClockUtil.h"

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
    void setup(uint8_t dta, uint8_t clk, uint8_t cs,
               uint8_t brightness = 0, uint32_t interval = 5E5);
    void displayTime(aTime time);
    void blinkTime(aTime time);
    void displayAlarm(bool status);
//private:
    LedControl lc;
    void setCacheColumn(uint8_t col, uint8_t value); // sets cache contents
    void setMatrixRow(uint8_t row, uint8_t value);
    void copyBit(const uint8_t source, uint8_t source_index,  uint8_t * const target, const uint8_t target_index);
    void updateDisplayFromCache();
    void clearCache();
    void clearDisplay();

    // Actual display contents, 1st display is row 0-7, 2nd is 8-15, etc.
    uint8_t           display[24];
    // Cache of display contents: in rows from 1...24
    uint8_t           display_cache[24];

    static byte       const n[10][5];
    static byte       const clock[9];
    static byte       const onoff[2][15];
    uint32_t _interval;
    uint32_t _lastFlank;
    bool _showing;
};

#endif // DOTMATRIX
