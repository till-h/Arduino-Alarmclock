#include "ClockUtil.h"

// Utility functions

// Change a given time depending on rotary button
void changeTime(aTime * time, int32_t rotation)
{
    if ((time->m + rotation) > 59) { time->h = (time->h + 1) % 24; }
    if ((time->m + rotation) <  0) { time->h = (time->h - 1 + 24) % 24; }
    time->m = (time->m + rotation + 60) % 60;
}