/*  AlarmClock.ino
 *   
 *  A simple alarm clock using a DS3231 clock chip and three 8x8
 *  dot matrix displays to show the current time, and set an alarm.
 *  
 *  Till Hackler, 2015-2020
 */

#include "ClockUtil.h"
#include "RotaryDial.h"
#include "LedControl.h"
#include "Loudspeaker.h"
#include "DotMatrix.h"
#include "DS3231.h"
#include "string.h"

#include "ClockConfig.h"

//////////////////////
// Global variables //
//////////////////////

aStatus status = displayCurrentTime;
bool alarmIsActive = false;

// Clock
DS3231 clk;

aTime actualTime, alarmTime;

// Dot matrix display
DotMatrix matrix;

// Control button (rotary encoder)
uint32_t lastRotation = 0;
uint32_t lastPress = 0;

// Encoder button callback
void buttonDepressed()
{
    lastPress = micros();
    alarmIsActive = !alarmIsActive;
}

RotaryDial dial(ENC1, ENC2, PUSH, &buttonDepressed);

Loudspeaker ls;

void setup() {
    Serial.begin(SERIAL_BAUD_RATE);
    Serial.println("Alarm Clock v0.1");
    Serial.println("Alarm time set to:");
    clk.printAlarmTime();
    
    // Read in alarm time
    uint8_t dummyVar = 0;
    clk.readAlarmTime(&alarmTime);

    matrix.setup(DIN, CLK, CS, 3);
    
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
    else
    {
        // Persist alarm time on clock chip - stored time survives power cuts
        clk.setAlarmTime(alarmTime);
    }

    switch(status)
    {
        case displayCurrentTime:        
            Serial.println("Case: displayCurrentTime"); 
            clk.readTime(&actualTime);
            matrix.blinkTime(actualTime.h, actualTime.m);
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
