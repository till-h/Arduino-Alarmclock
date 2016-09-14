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

void get2Digits(char * arrayFrom, char * arrayTo, int startIndex);

///////////////////
// Global variables
///////////////////

// State machine
enum aStatus {displayCurrentTime, displayAlarmTime, toggleAlarm};
aStatus status = displayCurrentTime;
bool alarmActive = false;

// Clock
DS3231 clk;
int lastEventTime = -1;

struct aTime
{
    uint8_t h = 0; // hours
    uint8_t m = 0; // minutes
};

aTime alarmTime;

// To hold the actual date
byte currentSecond,
     currentMinute,
     currentHour,
     currentDayOfWeek,
     currentDayOfMonth,
     currentMonth,
     currentYear;
byte lastMinute;

// Dot matrix display
DotMatrix matrix;

// Rotary encoder
RotaryDial dial(2, 5);
unsigned int lastRotation = 0;
unsigned int lastPress = 0;

// Encoder button callback function
void buttonDepressed()
{
lastPress = micros();
alarmActive = !alarmActive;
}

Loudspeaker ls;
uint16_t lsFreq = 10000;

void setup() {
    Serial.begin(9600);
    Serial.println("Alarm Clock v0.01");
    // Program clock with time of compilation
    char h[3];
    char m[3];
    get2Digits(__TIME__, h, 0);
    get2Digits(__TIME__, m, 3);
    int hour = atoi(h);;
    int minute = atoi(m);;

    clk.setTime(0, minute, hour, 1, 1, 1, 0);
    alarmTime.h = 20;
    alarmTime.m = 20;
    
    matrix.setup(4, A1, A2, 6);
    pinMode(3, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(3), buttonDepressed, RISING);
    ls.setPin(A3);
}

void loop() {
    status = displayCurrentTime;
    int rotation = dial.getRotation();
    unsigned long now = micros();
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
        status = toggleAlarm;
    }
    switch(status)
    {
        case displayCurrentTime:        
            Serial.println("Case: displayCurrentTime"); 
            clk.readTime(&currentSecond,
                         &currentMinute,
                         &currentHour,
                         &currentDayOfWeek,
                         &currentDayOfMonth,
                         &currentMonth,
                         &currentYear);
            matrix.displayTime(currentHour, currentMinute);
            break;
        case displayAlarmTime:
            Serial.println("Case: displayAlarmTime");
            matrix.displayTime(alarmTime.h, alarmTime.m);
            break;        
        case toggleAlarm:
            Serial.println("Case: toggleAlarm");
            matrix.displayAlarm(alarmActive);
        
    }

    if ((alarmTime.h == currentHour) && (alarmTime.m == currentMinute))
    {
        ls.startTone(lsFreq);
    }
    else
    {
        ls.stopTone();
    }
}

void get2Digits(char * arrayFrom, char * arrayTo, int startIndex)
{
    *arrayTo       = arrayFrom[startIndex];
    *(arrayTo + 1) = arrayFrom[startIndex + 1];
    *(arrayTo + 2) = '\0';
}

