/*
  DS3231.h

  Library to encapsulate communication with the
  DS3231 module using the Wire library.

  Created by Till Hackler, December 2015

  Credits to John Boxall at tronixstuff.com,
  see http://tinyurl.com/lw8umcp

*/

#ifndef DS3231_h
#define DS3231_h

#include "Arduino.h"

#define DS3231_I2C_ADDRESS 0x68
#define SERIAL_BAUD_RATE 9600

class DS3231
{
    public:
        DS3231();
        DS3231(byte second,
               byte minute,
               byte hour,
               byte dayOfWeek,
               byte dayOfMonth,
               byte month,
               byte year);
        void setTime(byte second,
                     byte minute,
                     byte hour,
                     byte dayOfWeek,
                     byte dayOfMonth,
                     byte month,
                     byte year);
        void readTime(byte *second,
                      byte *minute,
                      byte *hour,
                      byte *dayOfWeek,
                      byte *dayOfMonth,
                      byte *month,
                      byte *year);
        void printTime();
    private:
        byte decToBcd(byte val);
        byte bcdToDec(byte val);

};

#endif // DS3231_h
