/*  AlarmClock.ino
 *   
 *  A simple alarm clock using a DS3231 clock chip and three 8x8
 *  dot matrix displays to show the current time, and set an alarm.
 *  
 *  Created by Till Hackler, December 2015
 */

/*  Includes
 *  
 *  Including all libraries that are anywhere used inside the
 *  project is necessary here if using the Arduino IDE for
 *  making the executable, because its linker links only
 *  files that are included somewhere within this file.
 */
#include "Wire.h"
#include "Encoder.h"
#include "RotaryDial.h"
#include "LedControl.h"
#include "Loudspeaker.h"
#include "DotMatrix.h"
#include "DS3231.h"
#include "string.h"

//////////
// Pins //
//////////
#define BEEP  9
#define ENC1  3
#define ENC2  6
#define PUSH  2
#define CLK   13
#define CS    A0
#define DIN   A1

#define SERIAL_BAUD_RATE 9600

// default alarm time
#define DEFAULT_ALARM_H 7
#define DEFAULT_ALARM_M 0

void get2Digits(char * arrayFrom, char * arrayTo, uint8_t startIndex);

//////////////////////
// Global variables //
//////////////////////

// State machine
enum aStatus
{
    displayCurrentTime,
    displayAlarmTime,
    displayAlarmStatus
};

aStatus status = displayCurrentTime;
bool alarmIsActive = false;

// Clock
DS3231 clk;

struct aTime
{
    uint8_t h = 0; // hours
    uint8_t m = 0; // minutes
};

aTime actualTime, alarmTime;

// Dot matrix display
DotMatrix matrix;

// For the control button (rotary encoder)
RotaryDial dial(ENC1, ENC2);
uint32_t lastRotation = 0;
uint32_t lastPress = 0;

// Encoder button callback function
void buttonDepressed()
{
    lastPress = micros();
    alarmIsActive = !alarmIsActive;
}

Loudspeaker ls;

void setup() {
    Serial.begin(SERIAL_BAUD_RATE);
    Serial.println("Alarm Clock v0.01");

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
    // Default alarm time 7am
    alarmTime.h = DEFAULT_ALARM_H;
    alarmTime.m = DEFAULT_ALARM_M;

    matrix.setup(DIN, CLK, CS, 3, 0);
    
    pinMode(PUSH, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(PUSH), buttonDepressed, FALLING);
    ls.initialise(BEEP);
}

void loop() {
    status = displayCurrentTime;
    int rotation = dial.getRotation();
    uint32_t now = micros();
    if (rotation != 0) {
        if (alarmTime.m + rotation > 59) { alarmTime.h = (alarmTime.h + 1) % 24; }
        if (alarmTime.m + rotation <  0) { alarmTime.h = (alarmTime.h - 1 + 24) % 24; }
        alarmTime.m = (alarmTime.m + rotation + 60) % 60;
        status = displayAlarmTime;
        lastRotation = now;
    }

    // Continue to display alarm time for a while after the last rotation
    else if (now - lastRotation < 1000000)
    {
        status = displayAlarmTime;
    }
    else if (now - lastPress < 1000000)
    {
        status = displayAlarmStatus;
    }

    switch(status)
    {
        case displayCurrentTime:        
            Serial.println("Case: displayCurrentTime"); 
            clk.readHourMinute(&actualTime.m, &actualTime.h);
            matrix.displayTime(actualTime.h, actualTime.m);
            break;
        case displayAlarmTime:
            Serial.println("Case: displayAlarmTime");
            matrix.displayTime(alarmTime.h, alarmTime.m);
            break;
        case displayAlarmStatus:
            Serial.println("Case: displayAlarmStatus");
            matrix.displayAlarm(alarmIsActive);
        
    }

    if ((alarmTime.h == actualTime.h) && (alarmTime.m == actualTime.m) && alarmIsActive)
    {
      Serial.println("Ring!");
        ls.ring();
    }
    else
    {
        ls.beQuiet();
    }
}

void get2Digits(char * arrayFrom, char * arrayTo, uint8_t startIndex)
{
    *arrayTo       = arrayFrom[startIndex];
    *(arrayTo + 1) = arrayFrom[startIndex + 1];
    *(arrayTo + 2) = '\0';
}
