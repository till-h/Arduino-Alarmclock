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

#include "AlarmClockConfig.h"

//////////////////////
// Global variables //
//////////////////////

// State machine
typedef enum aStatus {
    displayCurrentTime,
    displayAlarmTime,
    displayAlarmStatus,
    displaySet,
    displaySetTime
} aStatus; // make aStatus available inside common namespace

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

// Control button (rotary encoder)
uint32_t lastRotation = 0;
uint32_t lastPress = 0;

// Encoder button callback
void buttonDepressed()
{
    int32_t state = digitalRead(PUSH);
    uint32_t now = micros();
    if (state == HIGH)
    {
        alarmIsActive = !alarmIsActive;
        lastPress = now;
    }
    else if (state == LOW)
    {

    }
}

RotaryDial dial(ENC1, ENC2, PUSH, &buttonDepressed);

// Change a given time depending on rotary button
void changeTime(aTime * time, int32_t rotation)
{
    if ((time->m + rotation) > 59) { time->h = (time->h + 1) % 24; }
    if ((time->m + rotation) <  0) { time->h = (time->h - 1 + 24) % 24; }
    time->m = (time->m + rotation + 60) % 60;
}

Loudspeaker ls;

void setup() {
    Serial.begin(SERIAL_BAUD_RATE);
    Serial.println("Alarm Clock v0.01");
    
    // Default alarm time 7am
    alarmTime.h = DEFAULT_ALARM_H;
    alarmTime.m = DEFAULT_ALARM_M;

    matrix.setup(DIN, CLK, CS, 3, 0);
    
    ls.initialise(BEEP);
}

void loop() {
    status = displayCurrentTime;
    int32_t rotation = dial.getRotation();

    uint32_t now = micros();
    if (rotation != 0) {
        changeTime(&alarmTime, rotation);
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
    //else if (now - lastPress )

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
            break;
        case displaySet:
            Serial.println("Case: displaySet");
            //matrix.displaySet();
            break;
        case displaySetTime:
            Serial.println("Case: displaySetTime");
            //matrix.displaySetTime();
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
