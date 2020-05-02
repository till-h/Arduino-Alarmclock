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

#define SERIAL_BAUD_RATE 9600
#define DS3231_I2C_ADDRESS 0x68
#define DS3231_REG_SECONDS 0x00
#define DS3231_REG_ALARM_SECONDS 0x07

class DS3231
{
    public:
        DS3231();
        DS3231(uint8_t second,
               uint8_t minute,
               uint8_t hour,
               uint8_t dayOfWeek,
               uint8_t dayOfMonth,
               uint8_t month,
               uint8_t year);
        void setTime(uint8_t second,
                     uint8_t minute,
                     uint8_t hour,
                     uint8_t dayOfWeek,
                     uint8_t dayOfMonth,
                     uint8_t month,
                     uint8_t year);
        void readTime(uint8_t *second,
                      uint8_t *minute,
                      uint8_t *hour,
                      uint8_t *dayOfWeek,
                      uint8_t *dayOfMonth,
                      uint8_t *month,
                      uint8_t *year);
        void setAlarmTime(uint8_t second,
                          uint8_t minute,
                          uint8_t hour,
                          uint8_t dayOfWeek,
                          uint8_t dayOfMonth,
                          uint8_t month,
                          uint8_t year);
        void readAlarmTime(uint8_t *second,
                           uint8_t *minute,
                           uint8_t *hour,
                           uint8_t *dayOfWeek,
                           uint8_t *dayOfMonth,
                           uint8_t *month,
                           uint8_t *year);
        void readHourMinute(uint8_t *minute,
                            uint8_t *hour);
        // TODO return char and length instead of printing inside function
        void printTime();
        void printAlarmTime();
    private:
        uint8_t decToBcd(uint8_t val);
        uint8_t bcdToDec(uint8_t val);
        void setTimeAtAddr(uint8_t addr,
                           uint8_t second,
                           uint8_t minute,
                           uint8_t hour,
                           uint8_t dayOfWeek,
                           uint8_t dayOfMonth,
                           uint8_t month,
                           uint8_t year);
        void readTimeAtAddr(uint8_t addr,
                            uint8_t *second,
                            uint8_t *minute,
                            uint8_t *hour,
                            uint8_t *dayOfWeek,
                            uint8_t *dayOfMonth,
                            uint8_t *month,
                            uint8_t *year);
        void timeToSerial(uint8_t second,
                          uint8_t minute,
                          uint8_t hour,
                          uint8_t dayOfWeek,
                          uint8_t dayOfMonth,
                          uint8_t month,
                          uint8_t year);
};

#endif // DS3231_h
