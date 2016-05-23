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
 *  files that are included somewhere within the this file.
 */
#include "Wire.h"
#include "Encoder.h"
#include "LedControl.h"
#include "Loudspeaker.h"
#include "DotMatrix.h"
#include "DS3231.h"

///////////////////
// Global variables
///////////////////

// State machine
enum status {displayCurrentTime, displayAlarmTime, toggleAlarm};
status currentStatus = displayCurrentTime;
bool alarmStatus = false;

// Clock
DS3231 clk;
int lastEventTime = -1;

struct someTime
{
    uint8_t h = 0; // hours
    uint8_t m = 0; // minutes
};

someTime alarmTime;

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

// Rotary encoder TODO ENCAPSULATE STATE VARIABLES IN ENCODER CLASS
Encoder       enc(2, 5);
long          currentPosition = 0;
long          lastPosition = 0;
int           positionDelta = 0;
unsigned long timeNow = 0;
unsigned long timeLast = 0;
unsigned long fastRotThres = 2*100000; // in us
unsigned long currentPress = 0;
unsigned long lastPress = 0;
int buttonTimeout = 50000;

// Encoder button callback function
void buttonDepressed()
{
lastPress = micros();
alarmStatus = !alarmStatus;
}

Loudspeaker ls;
uint16_t lsFreq = 10000;

void setup() {
    Serial.begin(9600);
    Serial.println("Alarm Clock v0.01");
    // Set time when programming the clock
    clk.setTime(0, 1, 1, 1, 1, 1, 0);
    alarmTime.h = 20;
    alarmTime.m = 20;
    
    matrix.setup(4, A1, A2, 6);
    pinMode(3, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(3), buttonDepressed, RISING);
    ls.setPin(A3);
}

void loop() {
    currentStatus = displayCurrentTime;
    positionDelta = 0;
    timeNow = micros();
    
    currentPosition = enc.read() / 4;
    if (currentPosition != lastPosition) {
        unsigned long timeDelta = timeNow - timeLast;
        positionDelta = currentPosition - lastPosition;
        // Handle fast rotation
        if (timeDelta < fastRotThres) {
            Serial.println("Fast rotation mode.");
            positionDelta *= 5;
        }

        if (alarmTime.m + positionDelta > 59) { alarmTime.h = (alarmTime.h + 1) % 24; }
        if (alarmTime.m + positionDelta <  0) { alarmTime.h = (alarmTime.h - 1 + 24) % 24; }
        alarmTime.m = (alarmTime.m + positionDelta + 60) % 60;
        
        lastPosition = currentPosition;
        timeLast = timeNow;        
        currentStatus = displayAlarmTime;
    }
    // Continue to display alarm time for a while after the last rotation
    else if (timeNow - timeLast < 1000000)
    {
        currentStatus = displayAlarmTime;
    }

    if (micros() - lastPress < 1000000)
    {
        currentStatus = toggleAlarm;
    }
    
    switch(currentStatus)
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
            matrix.displayAlarm(alarmStatus);
        
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
