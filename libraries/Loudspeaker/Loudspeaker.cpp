/*
  Loudspeaker.cpp

  Library for driving a loudspeaker connected to
  one of the Arduino pins and GND.

  Created by Till Hackler, December 2015

  TODO: implement on-off beeping using clock inside the Loudspeaker class
*/


#include "Loudspeaker.h"
#include "Arduino.h"

Loudspeaker::Loudspeaker() : _pin(-1), _freq(-1), _set(false) {}

void Loudspeaker::initialise(int8_t pin, int16_t freq)
{
    _pin = pin;
    _freq = freq;
}

void Loudspeaker::ring()
{
    if (!_set)
    {
        tone(_pin, _freq);
        _set = true;
    }
}

void Loudspeaker::beQuiet()
{
    if (_set)
    {
        noTone(_pin);
        _set = false;
    }
}
