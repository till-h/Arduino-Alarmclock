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

aTime currentTime, alarmTime;

// Dot matrix display
DotMatrix matrix;

// Control button (rotary encoder)
uint32_t lastRotation = 0;
uint32_t lastPress = 0;
uint32_t lastDepress = 0;
uint32_t lastSetTime = 0;

// Encoder button callback
void buttonChanged()
{
    int32_t state = digitalRead(PUSH);
    uint32_t now = micros();
    if (state == HIGH)
    {
        Serial.println("Pressed!");
        
        lastPress = now;
    }
    else if (state == LOW)
    {
        Serial.println("Depressed!");
        if (now - lastPress < 1E6)
        {
            alarmIsActive = !alarmIsActive;
        }
        else
        {
            lastSetTime = now;
        }
        lastDepress = now;
    }
}

RotaryDial dial(ENC1, ENC2, PUSH, &buttonChanged);

// Change a given time depending on rotary button
void changeTime(aTime time, int32_t rotation)
{
    aTime newTime;
    if ((time.m + rotation) > 59) { newTime.h = (time.h + 1) % 24; }
    if ((time.m + rotation) <  0) { newTime.h = (time.h - 1 + 24) % 24; }
    newTime.m = (time.m + rotation + 60) % 60;
    return newTime;
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
    // Detect rotation
    if (rotation != 0 && now - lastSetTime >= 5E6) {
        alarmTime = changeTime(alarmTime, rotation);
        status = displayAlarmTime;
        lastRotation = now;
    }
    else if (rotation != 0 && now - lastSetTime < 5E6)
    {
        currentTime = changeTime(currentTime, rotation);
        status = displaySetTime;
        lastRotation = now;
    }
    // Alarm time display timeout
    else if (now - lastRotation < 1E6)
    {
        status = displayAlarmTime;
    }
    // Alarm status display timeout
    else if (now - lastDepress < 1E6)
    {
        status = displayAlarmStatus;
    }
    



    switch(status)
    {
        case displayCurrentTime:        
            //Serial.println("Case: displayCurrentTime"); 
            clk.readHourMinute(&currentTime.m, &currentTime.h);
            matrix.displayTime(currentTime.h, currentTime.m);
            break;
        case displayAlarmTime:
            Serial.println("Case: displayAlarmTime");
            matrix.displayTime(alarmTime.h, alarmTime.m);
            break;
        case displayAlarmStatus:
            Serial.println("Case: displayAlarmStatus");
            matrix.displayAlarm(alarmIsActive);
            break;
        case displaySetTime:
            Serial.println("Case: displaySetTime");
            //matrix.displaySetTime();
    }

    if ((alarmTime.h == currentTime.h) && (alarmTime.m == currentTime.m) && alarmIsActive)
    {
      Serial.println("Ring!");
        ls.ring();
    }
    else
    {
        ls.beQuiet();
    }
}
