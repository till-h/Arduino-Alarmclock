#include "Arduino.h"

// Arduino Nano Pins
#define BEEP  9
#define ENC1  3
#define ENC2  6
#define PUSH  2
#define CLK   13
#define CS    A0
#define DIN   A1

// USB Serial
#define SERIAL_BAUD_RATE 9600

// DS3231
#define DS3231_I2C_ADDRESS 0x68
#define DS3231_REG_SECONDS 0x00
#define DS3231_REG_ALARM_SECONDS 0x07