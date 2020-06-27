/*
  Loudspeaker.cpp

  Library for driving a loudspeaker connected to
  one of the Arduino pins and GND.

  Created by Till Hackler, December 2015

  TODO: implement on-off beeping using clock inside the Loudspeaker class
*/


#include "Loudspeaker.h"

Loudspeaker::Loudspeaker() : _pin(-1), _freq(-1), _set(false) {}

void Loudspeaker::initialise(int8_t pin, int16_t freq, uint32_t interval)
{
    _pin = pin;
    _freq = freq;
    _interval = interval;
}

void Loudspeaker::ring()
{
    uint32_t now = micros();
    // change state if _interval microseconds have passed
    if (now - _lastFlank > _interval)
    {
        if (!_set)
        {
            tone(_pin, _freq);
            _set = true;
        }
        else
        {
            noTone(_pin);
            _set = false;
        }
        _lastFlank = now;
    }
}

void Loudspeaker::beQuiet()
{
    noTone(_pin);
    _set = false;
}
