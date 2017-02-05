/*  AlarmClock.ino
 *   
 *  A simple alarm clock using a DS3231 clock chip and three 8x8
 *  dot matrix displays to show the current time, and set an alarm.
 *  
 *  Created by Till Hackler, December 2015
 */

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
