/*
  RotaryDial.h

  Library for reading off rotation from a rotary encoder,
  including a fast forward / reverse mode.

  Created by Till Hackler, May 2016
*/

#ifndef RotaryDial_h
#define RotaryDial_h

#include "Arduino.h"
#include "Encoder.h"

class RotaryDial
{
public:
    RotaryDial(uint8_t rot1, uint8_t rot2, uint8_t push, void (*pushCallback)() );
    RotaryDial(uint8_t rot1, uint8_t rot2);
    int16_t       getRotation();

private:
    Encoder       *enc;
    uint8_t       pin1 = 0;
    uint8_t       pin2 = 0;
    uint8_t       push = 0;
    int32_t       currentPosition = 0;
    int32_t       lastPosition = 0;
    uint32_t      now = 0;
    uint32_t      then = 0;
    uint32_t      timeDelta = 0;
    uint32_t      fastRotThres = 200000; // in us
    int           buttonTimeout = 50000;
};

#endif
