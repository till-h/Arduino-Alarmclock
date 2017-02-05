#include "DS3231.h"

#define SERIAL_BAUD_RATE 9600

void get2Digits(char * arrayFrom, char * arrayTo, uint8_t startIndex);

// Clock
DS3231 clk;

void setup() {
    Serial.begin(SERIAL_BAUD_RATE);
    Serial.println("Setting clock to system time at preprocessing...");
  
    // Set clock with system time at compilation
    char t[9];
    char h[3];
    char m[3];
    char s[3];
    strcpy(t, __TIME__);
    get2Digits(t, h, 0);
    get2Digits(t, m, 3);
    get2Digits(t, s, 6);
    int hour = atoi(h);
    int minute = atoi(m);
    int second = atoi(s);
    clk.setTime(second, minute, hour, 1, 1, 1, 0);
    clk.printTime();
}

void loop() {
    delayMicroseconds(1000000);
}

void get2Digits(char * arrayFrom, char * arrayTo, uint8_t startIndex)
{
    *arrayTo       = arrayFrom[startIndex];
    *(arrayTo + 1) = arrayFrom[startIndex + 1];
    *(arrayTo + 2) = '\0';
}
