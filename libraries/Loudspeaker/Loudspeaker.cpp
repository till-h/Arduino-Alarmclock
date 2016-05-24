/*
  Loudspeaker.cpp

  Library for driving a loudspeaker connected to
  one of the Arduino pins and GND.

  Created by Till Hackler, December 2015
*/


#include "Loudspeaker.h"
#include "Arduino.h"

Loudspeaker::Loudspeaker() : _pin(-1) {}

Loudspeaker::Loudspeaker(uint8_t pin) : _pin(pin) {}

void Loudspeaker::setPin(uint8_t pin)
{
    _pin = pin;
}

void Loudspeaker::startTone(uint16_t freq)
{
    if (!_set)
    {
        tone(_pin, freq);
    }
}

void Loudspeaker::stopTone()
{
    if (_set)
    {
        noTone(_pin);
    }
}
