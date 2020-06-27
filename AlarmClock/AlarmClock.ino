/*  AlarmClock.ino
 *   
 *  A simple alarm clock using a DS3231 clock chip and three 8x8
 *  dot matrix displays to show the current time, and set an alarm.
 *  
 *  Till Hackler, 2015-2020
 */

#include "ClockUtil.h"
// #include "RotaryDial.h"
// #include "Loudspeaker.h"
#include "DotMatrix.h"
#include "DS3231.h"
// #include "string.h"

#include "ClockConfig.h"

//////////////////////
// Global variables //
//////////////////////

// State machine
// typedef enum aStatus {
//     displayCurrentTime,
//     displayAlarmTime,
//     displayAlarmStatus,
//     displaySet,
//     displaySetTime
// } aStatus; // make aStatus available inside common namespace

// aStatus status = displayCurrentTime;
// bool alarmIsActive = false;

// Clock
DS3231 clk;

aTime actualTime, alarmTime;

// Dot matrix display
DotMatrix matrix;

// // Control button (rotary encoder)
// uint32_t lastRotation = 0;
// uint32_t lastPress = 0;

// // Encoder button callback
// void buttonDepressed()
// {
//     lastPress = micros();
//     alarmIsActive = !alarmIsActive;
// }

// RotaryDial dial(ENC1, ENC2, PUSH, &buttonDepressed);

// Loudspeaker ls;

void setup() {
    // Serial.begin(SERIAL_BAUD_RATE);
    // Serial.println("Alarm Clock v0.1");
    // Serial.println("Alarm time set to:");
    // clk.printAlarmTime();
    
    // Read in alarm time
    clk.readAlarmTime(&alarmTime);

    matrix.setup(DIN, CLK, CS);
    
    //ls.initialise(BEEP);

    // matrix.clearCache();
    // matrix.clearDisplay();

    // size_t count = 24;
    // for (size_t i = 0; i < count; i++)
    // {
    //     matrix.display[i] = 0U;
    // }
    // for (size_t i = 0; i < 24; i++)
    // {
    //     matrix.lc.setRow(1, 4, 1U << (i % 8));
    //     matrix.lc.setRow(2, 4, 1U << (i % 8));
    //     delay(300);
    // }
    
    // matrix.copyBit(B00001000, 3, matrix.display_cache,     0);  //1
    // matrix.copyBit(B00001111, 2, matrix.display_cache,     2);  //1
    // matrix.copyBit(B00001000, 3, matrix.display_cache + 1, 0);  //1
    // matrix.copyBit(B00001000, 3, matrix.display_cache + 2, 0);  //1
    // matrix.copyBit(B00000001, 0, matrix.display_cache + 7, 7);  //1
    // matrix.copyBit(B00000100, 2, matrix.display_cache + 8, 6);  //1
    // matrix.copyBit(B00000000, 7, matrix.display_cache+15,  5); //0
    // matrix.copyBit(B11111111, 5, matrix.display_cache+16,  4); //1
    // matrix.copyBit(B11101111, 4, matrix.display_cache+23,  3); //0

    // matrix.setCacheColumn(0, B11111111);
    // matrix.setCacheColumn(1, B01010101);
    // matrix.setCacheColumn(7, B11111111);
    // matrix.setCacheColumn(8, B10101010);
    // matrix.setCacheColumn(23, B11111111);

    matrix.displayAlarm(true);
    delay(3E3);
    matrix.displayAlarm(false);
    delay(3E3);
    aTime t;
    clk.readTime(&t);
    matrix.displayTime(t);

    
    // matrix.updateDisplayFromCache();  
}

void loop() {
//     status = displayCurrentTime;
//     int32_t rotation = dial.getRotation();

//     uint32_t now = micros();
//     if (rotation != 0) {
//         changeTime(&alarmTime, rotation);
//         status = displayAlarmTime;
//         lastRotation = now;
//     }

//     // Continue to display alarm time for a while after the last rotation
//     else if (now - lastRotation < 1000000)
//     {
//         status = displayAlarmTime;
//     }
//     else if (now - lastPress < 1000000)
//     {
//         status = displayAlarmStatus;
//     }
//     else
//     {
//         // Persist alarm time on clock chip - stored time survives power cuts
//         clk.setAlarmTime(alarmTime);
//     }

//     switch(status)
//     {
//         case displayCurrentTime:        
//             Serial.println("Case: displayCurrentTime"); 
//             clk.readTime(&actualTime);
//             matrix.blinkTime(actualTime);
//             break;
//         case displayAlarmTime:
//             Serial.println("Case: displayAlarmTime");
//             matrix.displayTime(alarmTime);
//             break;
//         case displayAlarmStatus:
//             Serial.println("Case: displayAlarmStatus");
//             matrix.displayAlarm(alarmIsActive);
//             break;
//         case displaySet:
//             Serial.println("Case: displaySet");
//             //matrix.displaySet();
//             break;
//         case displaySetTime:
//             Serial.println("Case: displaySetTime");
//             //matrix.displaySetTime();
//     }

//     if ((alarmTime.h == actualTime.h) && (alarmTime.m == actualTime.m) && alarmIsActive)
//     {
//       Serial.println("Ring!");
//         ls.ring();
//     }
//     else
//     {
//         ls.beQuiet();
//     }
}
