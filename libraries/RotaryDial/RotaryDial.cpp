#include "RotaryDial.h"
// Constructor: rot1, rot2 are rotation pins, push is pin for push button, pushCallback is called when push is FALLING
RotaryDial::RotaryDial( uint8_t pin1, uint8_t pin2, uint8_t push, void (*pushCallback)() )
{
    // Rotation
    enc = new Encoder(pin1, pin2);
    // Push button
    pinMode(push, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(push), pushCallback, FALLING);
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
