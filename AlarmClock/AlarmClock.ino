/*  AlarmClock.ino
 *   
 *  A simple alarm clock using a DS3231 clock chip and three 8x8
 *  dot matrix displays to show the current time, set and (de)
 *  activate an alarm.
 *  
 *  Created by Till Hackler, December 2015
 */

/*  Includes
 *  
 *  Including all libraries that are ever used inside the     
 *  project is necessary here if using the Arduino IDE for
 *  making the executable, because its linker links only
 *  files that are included somewhere within the present file.
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
enum action {displayCurrentTime, displayAlarmTime, toggleAlarm};
action clockAction = displayCurrentTime;
bool alarmIsOn = false;
int lastEventTime = -1;

struct aTime
{
    uint8_t h = 0; // hours
    uint8_t m = 0; // minutes
};

aTime currentTime, alarmTime;


DS3231 clk;
// actual date
byte dateSecond,
     dateMinute,
     dateHour,
     dateDayOfWeek,
     dateDayOfMonth,
     dateMonth,
     dateYear;


//DotMatrix   matrix;

Encoder       enc(2, 5);
long          oldPosition  = 0;
unsigned long encoderLast = 0;
unsigned long fastRotThres = 100000; // in us

Loudspeaker ls;

/////////////////////////////
// Encoder callback functions
/////////////////////////////
void buttonPressed()
{}

void buttonDepressed()
{}


/* Initialise classes
 * 
 */
LedControl lc2 = LedControl(4, A1, A2, 3);

void setup() {

for(int8_t i = 0; i < 3; i++)
{
    lc2.clearDisplay(i);
    lc2.setIntensity(i, 1);
    lc2.shutdown(i, false);
   
}
  
    clk.setTime(0, 1, 1, 1, 1, 1, 0); // set current time for production // IDEA / TODO: Try setting current computer system time through clever use of preprocessor logic
//    matrix.setup(4, A1, A2, 3); 
    ls.setPin(A3);
//    matrix.displayTime(2,2);
    
}

void loop() {
  lc2.setLed(1,1,1,true);
  
//  switch(clockAction)
//  {
//      case displayCurrentTime:
//          matrix.displayTime(2, 20);
//      case displayAlarmTime:
//          break;
//      case toggleAlarm:
//          break;
//  }
//    clk.readTime(&dateSecond,
//                 &dateMinute,
//                 &dateHour,
//                 &dateDayOfWeek,
//                 &dateDayOfMonth,
//                 &dateMonth,
//                 &dateYear);
}
