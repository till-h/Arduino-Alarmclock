/*  AlarmClock.ino
 *   
 *  A simple alarm clock using a DS3231 clock chip and three 8x8
 *  dot matrix displays to show the current time, and set an alarm.
 *  
 *  Created by Till Hackler, December 2015
 */

/*  Includes
 *  
 *  Including all libraries that are ever used inside the     
 *  project is necessary here if using the Arduino IDE for
 *  making the executable, because its linker links only
 *  files that are included somewhere within the this file.
 */
#include "Wire.h"
#include "Encoder.h"
#include "LedControl.h"
//#include "Loudspeaker.h"
#include "DotMatrix.h"
#include "DS3231.h"

///////////////////
// Sign function from http://stackoverflow.com/a/4609795
///////////////////
template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

///////////////////
// Global variables
///////////////////

// State machine
enum status {displayCurrentTime, displayAlarmTime, toggleAlarm};
status clockStatus = displayCurrentTime;
status lastStatus = displayCurrentTime;
bool alarmIsOn = false;

// Clock
DS3231 clk;
int lastEventTime = -1;

struct aTime
{
    uint8_t h = 0; // hours
    uint8_t m = 0; // minutes
};

aTime currentTime, alarmTime;

// actual date
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
Encoder       enc(2, 5);
long          lastPosition  = 0;
unsigned long timeLast = 0;
unsigned long fastRotThres = 2*100000; // in us

//Loudspeaker ls;

/////////////////////////////
// Encoder callback functions
/////////////////////////////
//void buttonPressed()
//{}
//
//void buttonDepressed()
//{}

void setup() {
    Serial.begin(9600);
    Serial.println("Alarm Clock v0.01");
    // Set time when programming the clock
    clk.setTime(0, 1, 1, 1, 1, 1, 0);
    alarmTime.h = 20;
    alarmTime.m = 20;
    
    matrix.setup(4, A1, A2, 6); 
//    ls.setPin(A3);
}

void loop() {
    clockStatus = displayCurrentTime;
    int positionDelta = 0;
    
    // Handle encoder rotation, inc. fast forward (rotation) mode
    long currentPosition = enc.read() / 4;
    Serial.println(currentPosition);
    Serial.println(lastPosition);
    if (currentPosition != lastPosition) {
        unsigned long timeNow = micros();
        unsigned long timeDelta = timeNow - timeLast;
        positionDelta = currentPosition - lastPosition;
        if (timeDelta < fastRotThres) {
            Serial.println("Fast rotation mode.");
            positionDelta *= 5;
        }
        lastPosition = currentPosition;
        timeLast = timeNow;
    }
    if (positionDelta != 0)
    {
        Serial.println("positionDelta != 0"); 
        clockStatus = displayAlarmTime;
        if (alarmTime.m + positionDelta > 59) { alarmTime.h = (alarmTime.h + 1) % 24; }
        if (alarmTime.m + positionDelta <  0) { alarmTime.h = (alarmTime.h - 1 + 24) % 24; }
        alarmTime.m = (alarmTime.m + positionDelta + 60) % 60;
    }
    switch(clockStatus)
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
//            if (currentMinute != lastMinute) // reverts to current time only after a minute, fix!
//            {
                matrix.displayTime(currentHour, currentMinute);
                lastMinute = currentMinute;
//            }
            break;
        case displayAlarmTime:
            Serial.println("Case: displayAlarmTime"); 
            matrix.displayTime(alarmTime.h, alarmTime.m);
            break;        
        case toggleAlarm:
            break;
    }
}
