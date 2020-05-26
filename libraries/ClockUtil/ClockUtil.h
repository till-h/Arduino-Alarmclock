#ifndef ClockUtil_h
#define ClockUtil_h

#include <stdint.h>

// Utility data structures

struct aTime
{
    uint8_t s    = 0; // seconds
    uint8_t m    = 0; // minutes
    uint8_t h    = 0; // hours
    uint8_t dw   = 1; // day of week, Sun == 1, Mon == 2
    uint8_t dd   = 1; // day of month
    uint8_t mm   = 1; // month
    uint8_t yyyy = 1; // year
};

// System hardware state
// This is manipulated by the system FSM
struct hardwareState
{
    aTime currentTime, alarmTime;
    bool alarmIsActive;
}

// State machine
enum aStatus
{
    displayCurrentTime,
    displayAlarmTime,
    displayAlarmStatus,
    displaySet,
    displaySetTime
};

// Utility functions

void changeTime(aTime * time, int32_t rotation);

#endif /* ClockUtil_h */