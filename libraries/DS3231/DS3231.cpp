/*
  DS3231.cpp

  Library to encapsulate communication with the
  DS3231 module using the Wire library.

  Created by Till Hackler, December 2015

  Credits to John Boxall at tronixstuff.com,
  see http://tinyurl.com/lw8umcp
*/

#include "DS3231.h"
#include "Wire.h"

DS3231::DS3231()
{
    Wire.begin();
    /* TODO Set time using compiler IF the compiler time is newer than
    / the time stored inside the chip. Otherwise stick with chip time.
    */
}

DS3231::DS3231(aTime time)
{
    Wire.begin();
    Serial.begin(SERIAL_BAUD_RATE);
    setTime(time);
}

void DS3231::setTime(aTime time)
{
    DS3231::setTimeAtAddr(DS3231_REG_CURRENT_SECONDS,
                          time);
}

void DS3231::readTime(aTime * time)
{
    DS3231::readTimeAtAddr(DS3231_REG_CURRENT_SECONDS,
                           time);
}

void DS3231::setAlarmTime(aTime time)
{
    DS3231::setTimeAtAddr(DS3231_REG_ALARM_SECONDS,
                          time);
}

void DS3231::readAlarmTime(aTime * time)
{
    DS3231::readTimeAtAddr(DS3231_REG_ALARM_SECONDS,
                           time);
}

void DS3231::printTime()
{
    aTime time;
    // retrieve data from DS3231
    readTime(&time);
    // send it to the serial monitor
    timeToSerial(time);
}

void DS3231::printAlarmTime()
{
    aTime time;
    // retrieve data from DS3231
    readAlarmTime(&time);
    // send it to the serial monitor
    timeToSerial(time);
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
                aTime time)
{
    Wire.beginTransmission(DS3231_I2C_ADDRESS);
    Wire.write(addr); // set next input to start at the seconds register
    Wire.write(decToBcd(time.s)); // set seconds
    Wire.write(decToBcd(time.m)); // set minutes
    Wire.write(decToBcd(time.h)); // set hours
    Wire.write(decToBcd(time.dw)); // set day of week (1=Sunday, 7=Saturday)
    Wire.write(decToBcd(time.dd)); // set date (1 to 31)
    Wire.write(decToBcd(time.mm)); // set month
    Wire.write(decToBcd(time.yyyy)); // set year (0 to 99)
    Wire.endTransmission();
}

void DS3231::readTimeAtAddr(uint8_t addr,
                 aTime * time)
{
    Wire.beginTransmission(DS3231_I2C_ADDRESS);
    Wire.write(addr); // set DS3231 register pointer to 00h
    Wire.endTransmission();
    // request seven bytes of data from DS3231 starting from register 00h
    Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
    time->s = bcdToDec(Wire.read() & 0x7f);
    time->m = bcdToDec(Wire.read());
    time->h = bcdToDec(Wire.read() & 0x3f);
    time->dw = bcdToDec(Wire.read());
    time->dd = bcdToDec(Wire.read());
    time->mm = bcdToDec(Wire.read());
    time->yyyy = bcdToDec(Wire.read());
}

void DS3231::timeToSerial(aTime time)
{
    // send it to the serial monitor
    Serial.print(time.h, DEC);
    // convert the uint8_t variable to a decimal number when displayed
    Serial.print(":");
    if (time.m<10)
    {
        Serial.print("0");
    }
    Serial.print(time.m, DEC);
    Serial.print(":");
    if (time.s<10)
    {
        Serial.print("0");
    }
    Serial.print(time.s, DEC);
    Serial.print(" ");
    Serial.print(time.dd, DEC);
    Serial.print("/");
    Serial.print(time.mm, DEC);
    Serial.print("/");
    Serial.print(time.yyyy, DEC);
    Serial.print(" Day of week: ");
    switch(time.dw)
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