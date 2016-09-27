/*
  Loudspeaker.cpp

  Library for driving a loudspeaker connected to
  one of the Arduino pins and GND.

  Created by Till Hackler, December 2015

  TODO: implement on-off beeping using clock inside the Loudspeaker class
*/


#include "Loudspeaker.h"
#include "Arduino.h"

Loudspeaker::Loudspeaker() : _pin(-1), _set(false) {}

Loudspeaker::Loudspeaker(uint8_t pin) : _pin(pin), _set(false) {}

void Loudspeaker::setPin(uint8_t pin)
{
    _pin = pin;
}

void Loudspeaker::ring(uint16_t freq)
{
    if (!_set)
    {
        tone(_pin, freq);
    }
    _set = true;
}

void Loudspeaker::beQuiet()
{
    if (_set)
    {
        noTone(_pin);
    }
    _set = false;
}
