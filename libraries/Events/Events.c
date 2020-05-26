#include "Events.h"

ButtonSource::ButtonSource(uint8_t pin)
{
    pinMode(pin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(pin), buttonCallback, FALLING); // when button is released. TODO allow CHANGE
    unservedButtonRelease = false;
}

void ButtonSource::buttonCallback()
{
    lastButtonRelease = micros();
    unservedButtonRelease = true;
}

anEvent ButtonSource::poll()
{
    if (unservedButtonRelease)
    {
        // TODO Differentiate between release after short press and after long press
        unservedButtonRelease = false;
        return (anEvent){buttonPress, micros()};
    }
    else
    {
        return (anEvent){none, micros()};
    }
}

RotationSource::RotationSource(uint8_t pin1, uint8_t pin2)
{
    
}

Scheduler::Scheduler()
{
    source[1] = ButtonSource;
    source[2] = RotationSource;
    source[3] = TimerSource;
}

Scheduler::run()
{
    // for source in sources
    // event = source.poll
    // if event is not None
    // 

    while true
    {

    }
}