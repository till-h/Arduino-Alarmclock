/*
  Loudspeaker.h

  Library for driving a loudspeaker connected to
  one of the Arduino pins and GND.

  Created by Till Hackler, Dec 2015
*/

#ifndef Loudspeaker_h
#define Loudspeaker_h

#include "Arduino.h"

class Loudspeaker
{
    public:
        /* Default constructor leaves the object unusable
         * Pin is set to -1
         */
        Loudspeaker();
        Loudspeaker(uint8_t pin);
        void setPin(uint8_t pin);
        void startTone(uint16_t freq);
        void stopTone();
    private:
        uint8_t _pin;
        bool _set;
};

#endif // Loudspeaker_h
