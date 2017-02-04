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
        void initialise(int8_t pin, int16_t freq = 31, uint32_t interval = 1000000);
        void ring();
        void beQuiet();
    private:
        int8_t _pin;
        int16_t _freq;
        bool _set;
        uint32_t _interval;
        uint32_t _lastFlank;
};

#endif // Loudspeaker_h
