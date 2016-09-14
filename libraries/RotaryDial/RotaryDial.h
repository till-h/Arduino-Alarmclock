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
    RotaryDial(uint8_t pin1, uint8_t pin2);
    int16_t getRotation();

private:
    Encoder       *enc;
    uint8_t       pin1 = 0;
    uint8_t       pin2 = 0;
    long          currentPosition = 0;
    long          lastPosition = 0;
    unsigned long now = 0;
    unsigned long then = 0;
    unsigned long timeDelta = 0;
    unsigned long fastRotThres = 2*100000; // in us
    int buttonTimeout = 50000;
};

#endif
