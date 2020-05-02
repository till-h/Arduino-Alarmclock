/*
  DS3231.cpp

  Library to encapsulate communication with the
  DS3231 module using the Wire library.

  Created by Till Hackler, December 2015

  Credits to John Boxall at tronixstuff.com,
  see http://tinyurl.com/lw8umcp
*/

#include "Arduino.h"
#include "DS3231.h"
#include "Wire.h"

DS3231::DS3231()
{
    Wire.begin();
    // Some default datum, not needed when
    // setting datum through different program.
    //setTime(0,1,1,1,1,1,0);
}

DS3231::DS3231(uint8_t second,
               uint8_t minute,
               uint8_t hour,
               uint8_t dayOfWeek,
               uint8_t dayOfMonth,
               uint8_t month,
               uint8_t year)
{
    Wire.begin();
    Serial.begin(SERIAL_BAUD_RATE);
    setTime(second,
            minute,
            hour,
            dayOfWeek,
            dayOfMonth,
            month,
            year);
}

void DS3231::setTime(uint8_t second,
                uint8_t minute,
                uint8_t hour,
                uint8_t dayOfWeek,
                uint8_t dayOfMonth,
                uint8_t month,
                uint8_t year)
{
    DS3231::setTimeAtAddr(DS3231_REG_SECONDS,
                          second,
                          minute,
                          hour,
                          dayOfWeek,
                          dayOfMonth,
                          month,
                          year);
}

void DS3231::readTime(uint8_t *second,
                 uint8_t *minute,
                 uint8_t *hour,
                 uint8_t *dayOfWeek,
                 uint8_t *dayOfMonth,
                 uint8_t *month,
                 uint8_t *year)
{
    DS3231::readTimeAtAddr(DS3231_REG_SECONDS,
                           second,
                           minute,
                           hour,
                           dayOfWeek,
                           dayOfMonth,
                           month,
                           year);
}

void DS3231::setAlarmTime(uint8_t second,
                uint8_t minute,
                uint8_t hour,
                uint8_t dayOfWeek,
                uint8_t dayOfMonth,
                uint8_t month,
                uint8_t year)
{
    DS3231::setTimeAtAddr(DS3231_REG_ALARM_SECONDS,
                          second,
                          minute,
                          hour,
                          dayOfWeek,
                          dayOfMonth,
                          month,
                          year);
}

void DS3231::readAlarmTime(uint8_t *second,
                 uint8_t *minute,
                 uint8_t *hour,
                 uint8_t *dayOfWeek,
                 uint8_t *dayOfMonth,
                 uint8_t *month,
                 uint8_t *year)
{
    DS3231::readTimeAtAddr(DS3231_REG_ALARM_SECONDS,
                           second,
                           minute,
                           hour,
                           dayOfWeek,
                           dayOfMonth,
                           month,
                           year);
}

void DS3231::readHourMinute(uint8_t *minute,
                       uint8_t *hour)
{
  uint8_t second, dayOfWeek, dayOfMonth, month, year;
  readTime(&second,
           minute,
           hour,
           &dayOfWeek,
           &dayOfMonth,
           &month,
           &year);
}

void DS3231::printTime()
{
    uint8_t second, minute, hour, dayOfWeek, dayOfMonth, month, year;
    // retrieve data from DS3231
    readTime(&second,
             &minute,
             &hour,
             &dayOfWeek,
             &dayOfMonth,
             &month,
             &year);
    // send it to the serial monitor
    timeToSerial(second,
                 minute,
                 hour,
                 dayOfWeek,
                 dayOfMonth,
                 month,
                 year);
}

void DS3231::printAlarmTime()
{
    uint8_t second, minute, hour, dayOfWeek, dayOfMonth, month, year;
    // retrieve data from DS3231
    readAlarmTime(&second,
                  &minute,
                  &hour,
                  &dayOfWeek,
                  &dayOfMonth,
                  &month,
                  &year);
    // send it to the serial monitor
    timeToSerial(second,
                 minute,
                 hour,
                 dayOfWeek,
                 dayOfMonth,
                 month,
                 year);
}

uint8_t DS3231::decToBcd(uint8_t val)
{
  return( (val/10*16) + (val%10) );
}

uint8_t DS3231::bcdToDec(uint8_t val)
{
  return( (val/16*10) + (val%16) );
}

void DS3231::setTimeAtAddr(uint8_t addr,
                uint8_t second,
                uint8_t minute,
                uint8_t hour,
                uint8_t dayOfWeek,
                uint8_t dayOfMonth,
                uint8_t month,
                uint8_t year)
{
    Wire.beginTransmission(DS3231_I2C_ADDRESS);
    Wire.write(addr); // set next input to start at the seconds register
    Wire.write(decToBcd(second)); // set seconds
    Wire.write(decToBcd(minute)); // set minutes
    Wire.write(decToBcd(hour)); // set hours
    Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
    Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
    Wire.write(decToBcd(month)); // set month
    Wire.write(decToBcd(year)); // set year (0 to 99)
    Wire.endTransmission();
}

void DS3231::readTimeAtAddr(uint8_t addr,
                 uint8_t *second,
                 uint8_t *minute,
                 uint8_t *hour,
                 uint8_t *dayOfWeek,
                 uint8_t *dayOfMonth,
                 uint8_t *month,
                 uint8_t *year)
{
    Wire.beginTransmission(DS3231_I2C_ADDRESS);
    Wire.write(addr); // set DS3231 register pointer to 00h
    Wire.endTransmission();
    // request seven bytes of data from DS3231 starting from register 00h
    Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
    *second = bcdToDec(Wire.read() & 0x7f);
    *minute = bcdToDec(Wire.read());
    *hour = bcdToDec(Wire.read() & 0x3f);
    *dayOfWeek = bcdToDec(Wire.read());
    *dayOfMonth = bcdToDec(Wire.read());
    *month = bcdToDec(Wire.read());
    *year = bcdToDec(Wire.read());
}

void DS3231::timeToSerial(uint8_t second,
                  uint8_t minute,
                  uint8_t hour,
                  uint8_t dayOfWeek,
                  uint8_t dayOfMonth,
                  uint8_t month,
                  uint8_t year)
{
    // send it to the serial monitor
    Serial.print(hour, DEC);
    // convert the uint8_t variable to a decimal number when displayed
    Serial.print(":");
    if (minute<10)
    {
        Serial.print("0");
    }
    Serial.print(minute, DEC);
    Serial.print(":");
    if (second<10)
    {
        Serial.print("0");
    }
    Serial.print(second, DEC);
    Serial.print(" ");
    Serial.print(dayOfMonth, DEC);
    Serial.print("/");
    Serial.print(month, DEC);
    Serial.print("/");
    Serial.print(year, DEC);
    Serial.print(" Day of week: ");
    switch(dayOfWeek)
    {
    case 1:
        Serial.println("Sunday");
        break;
    case 2:
        Serial.println("Monday");
        break;
    case 3:
        Serial.println("Tuesday");
        break;
    case 4:
        Serial.println("Wednesday");
        break;
    case 5:
        Serial.println("Thursday");
        break;
    case 6:
        Serial.println("Friday");
        break;
    case 7:
        Serial.println("Saturday");
        break;
    default:
        Serial.println("N/A");
    }
}