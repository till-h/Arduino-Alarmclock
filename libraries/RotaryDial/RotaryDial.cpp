#include "Arduino.h"
#include "RotaryDial.h"

RotaryDial::RotaryDial(uint8_t pin1, uint8_t pin2)
{
    enc = new Encoder(pin1, pin2);
}

int16_t RotaryDial::getRotation()
{
    currentPosition = enc->read() / 4;
    now = micros();

    int16_t rotation = currentPosition - lastPosition;
    if (rotation != 0)
    {
        timeDelta = now - then;
        if (timeDelta <= fastRotThres)
        {
            rotation *= 5;
        }
        lastPosition = currentPosition;
        then = now;
    }
    return rotation;
}
