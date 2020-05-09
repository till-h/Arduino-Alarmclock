/*
  DS3231.h

  Library to encapsulate communication with the
  DS3231 module using the Wire library.

  Created by Till Hackler, December 2015

  Credits to John Boxall at tronixstuff.com,
  see http://tinyurl.com/lw8umcp

  TODO use aTime structs to store all times instead of individual uint8_t's.

*/

#ifndef DS3231_h
#define DS3231_h

#include "Arduino.h"
#include "ClockConfig.h"
#include "ClockUtil.h"

class DS3231
{
    public:
        DS3231();
        DS3231(aTime time);
        void setTime(aTime time);
        void readTime(aTime * time);
        void setAlarmTime(aTime time);
        void readAlarmTime(aTime * time);
        // TODO return char and length instead of printing inside function
        void printTime();
        void printAlarmTime();
    private:
        uint8_t decToBcd(uint8_t val);
        uint8_t bcdToDec(uint8_t val);
        void setTimeAtAddr(uint8_t addr,
                           aTime time);
        void readTimeAtAddr(uint8_t addr,
                            aTime * time);
        void timeToSerial(aTime time);
};

#endif /* DS3231_h */
