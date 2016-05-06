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
//#include "Encoder.h"
#include "LedControl.h"
//#include "Loudspeaker.h"
#include "DotMatrix.h"
#include "DS3231.h"

///////////////////
// Sign function from http://stackoverflow.com/a/4609795
///////////////////
//template <typename T> int sgn(T val) {
//    return (T(0) < val) - (val < T(0));
//}

///////////////////
// Global variables
///////////////////
//enum action {displayCurrentTime, displayAlarmTime, toggleAlarm};
//action clockAction = displayCurrentTime;
//bool alarmIsOn = false;
//int lastEventTime = -1;
//
//struct aTime
//{
//    uint8_t h = 0; // hours
//    uint8_t m = 0; // minutes
//};
//
//aTime currentTime, alarmTime;

DotMatrix   matrix;
DS3231 clk;

// actual date
//byte dateSecond,
//     dateMinute,
//     dateHour,
//     dateDayOfWeek,
//     dateDayOfMonth,
//     dateMonth,
//     dateYear;




//Encoder       enc(2, 5);
//long          oldPosition  = 0;
//unsigned long encoderLast = 0;
//unsigned long fastRotThres = 100000; // in us

//Loudspeaker ls;

/////////////////////////////
// Encoder callback functions
/////////////////////////////
//void buttonPressed()
//{}
//
//void buttonDepressed()
//{}

/* Initialise classes
 * 
 */
void setup() {
    clk.setTime(0, 1, 1, 1, 1, 1, 0); // set time when programming the clock
    Serial.begin(9600);
    
    Serial.println("HELLO");
    
    matrix.setup(4, A1, A2, 3); 
//    ls.setPin(A3);
    matrix.displayTime(00,00);
}

void loop() {
//  int positionDelta = 0;
//   handle encoder rotation
//  long newPosition = enc.read() / 4;
//  if (newPosition != oldPosition) {
//    unsigned long timeNow = micros();
//    unsigned long timeDelta = timeNow - encoderLast;
//    positionDelta = newPosition - oldPosition;
//    if (timeDelta < fastRotThres) {
//      positionDelta *= 5;
//    }
//    oldPosition = newPosition;
//    encoderLast = timeNow;
//  }
  
  
//  switch(clockAction)
//  {
//      case displayCurrentTime:
//        if (positionDelta != 0) {
//          
//          if (currentTime.m + positionDelta > 59) { currentTime.h = (currentTime.h + 1) % 24; }
//          if (currentTime.m + positionDelta <  0) { currentTime.h = (currentTime.h - 1 + 24) % 24; }
//          
//          currentTime.m = (currentTime.m + positionDelta + 60) % 60;
//          
////          matrix.displayTime(currentTime.h, currentTime.m);
//        }
//      case displayAlarmTime:
//        if (positionDelta != 0) {
//          break;
//        }
//      case toggleAlarm:
//          break;
//  }
  delay(500);
  clk.printTime();
  Serial.println("Hello");   
}
