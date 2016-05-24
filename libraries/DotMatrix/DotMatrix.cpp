/*
  DotMatrix.cpp

  Library for controlling the 3 by 8x8 dot matrix display.

  Created by Till Hackler, December 2015

*/

#include <DotMatrix.h>

DotMatrix::DotMatrix() {}

void DotMatrix::setup(int dta, int clk, int cs, int num)
{
    lc.setup(dta, clk, cs, num);
    for(int8_t i = 0; i < lc.getDeviceCount(); i++)
    {
        lc.clearDisplay(i);
        lc.shutdown(i, false);
        lc.setIntensity(i, 2);
    }
}

void DotMatrix::displayTime(uint8_t hour, uint8_t minute)
{
    // extract digits
    uint8_t h0 = hour / 10; // tens of hours
    uint8_t h1 = hour % 10; // hours
    uint8_t m0 = minute / 10; // tens of minutes
    uint8_t m1 = minute % 10; // minutes

    if (hour != last_hour || minute != last_minute)
    {
        // display them
        uint8_t col = 0;
        //if (h0 != 0) // skip leading zero
        for (col = 0; col < 5; col++)
            setColumn(col, n[h0][col]);
        for (col = 6; col < 11; col++)
            setColumn(col, n[h1][col - 6]);
        for (col = 13; col < 18; col++)
            setColumn(col, n[m0][col - 13]);
        for (col = 19; col < 24; col++)
            setColumn(col, n[m1][col - 19]);
    }
    last_hour = hour;
    last_minute = minute;
}

void DotMatrix::displayAlarm(bool status)
{
    if (int(status) - 2 != last_hour)
    {
        uint8_t col = 0;
        // display clock symbol
        for (; col < 9; col++)
            setColumn(col, clock[col]);
        // display on / off
        for (; col < 24; col++)
            setColumn(col, onoff[int(status)][col - 9]);
        // mark which alarm was last set
        last_hour = int(status) - 2;
    }
}

/*
 * Use column number to determine which matrix is addressed.
 */
void DotMatrix::setColumn(uint8_t col, byte value)
{
    lc.setColumn(col / 8, col % 8, value);
}

byte const DotMatrix::n[10][5] =
{
    { // 0
        B00011111,
        B00100001,
        B11000010,
        B10000100,
        B11111000
    },
    { // 1
        B00100000,
        B00100011,
        B01001100,
        B01110000,
        B11000000
    },
    { // 2
        B00100001,
        B01000111,
        B11001010,
        B01110010,
        B00000100
    },
    { // 3
        B00100000,
        B01000000,
        B10001001,
        B10110010,
        B11001100
    },
    { // 4
        B00011000,
        B01101000,
        B10010011,
        B00111100,
        B00010000
    },
    { // 5
        B00110010,
        B01001001,
        B01010001,
        B10010001,
        B00001110
    },
    { // 6
        B00001100,
        B00110010,
        B01001001,
        B10001010,
        B00000100
    },
    { // 7
        B00100000,
        B00101011,
        B01001100,
        B01111000,
        B11010000
    },
    { // 8
        B00000111,
        B01101001,
        B10010001,
        B10101110,
        B11000000
    },
    { // 9
        B00000001,
        B11000011,
        B10101100,
        B11110000,
        B11000000
    }
};

byte const DotMatrix::clock[9] =
{
    B01000000,
    B10111001,
    B01010101,
    B10010010,
    B10110010,
    B10000010,
    B01000101,
    B10111001,
    B01000000
};

byte const DotMatrix::onoff[2][15] =
{
    { // ON
        B00000000,
        B00011100,
        B00100010,
        B01000010,
        B01000100,
        B00111000,
        B00000000,
        B00000000,
        B00011110,
        B01100000,
        B00011000,
        B00000110,
        B01111000,
        B00000000,
        B00000000
    },
    { // OFF
        B00000000,
        B00011100,
        B00100010,
        B01000010,
        B01000100,
        B00111000,
        B00000000,
        B00000010,
        B00111100,
        B01010000,
        B01000000,
        B00000010,
        B00111100,
        B01010000,
        B01000000

    }
};
