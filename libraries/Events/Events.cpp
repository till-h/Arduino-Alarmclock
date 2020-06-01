#include "Events.h"

// Event sources

ButtonSource::ButtonSource(uint8_t pin, uint32_t longPressThreshold): 
    pin(pin),
    longPressThreshold(longPressThreshold),
    buttonChange(none)
{
    pinMode(pin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(pin), buttonCallback, CHANGE); // when button is released. TODO allow CHANGE
}

void ButtonSource::buttonCallback()
{
    lastButtonChange = micros();
    if (digitalRead(pin) == HIGH)
    {
        buttonChange = pressed;
    }
    else
    {
        buttonChange = released;
    }
}

anEvent ButtonSource::poll()
{
    uint32_t now = micros();
    if (buttonChange != none)
    {
        if ((now - lastButtonChange < longPressThreshold) && (buttonChange == pressed))
        {
            unservedButtonChange = false;
            return (anEvent){buttonPress, lastButtonChange, 0};
        }
        else if (unservedButtonChange)
        {
            unservedButtonChange = false;
            return (anEvent){longButtonPress, lastButtonChange, 0};
        }
        else
        {
            return (anEvent){none, micros(), 0};
        }
    }
}


RotationSource::RotationSource(uint8_t pin1, uint8_t pin2)
{
    
}

void RotationSource::poll()
{}


TimerSource::TimerSource(uint32_t delay):
    delay(delay),
    isActive(false),
    startTime(0)
{}

void TimerSource::start(uint32_t delay)
{
    isActive = true;
    startTime = micros();
    this->delay = delay;
}

void TimerSource::start()
{
    isActive = true;
    startTime = micros();
}

void TimerSource::cancel()
{
    isActive = false;
}

anEvent TimerSource::poll()
{
    if (isActive)
    {
        uint32_t now = micros();
        if (now - startTime > delay)
        {
            return (anEvent){timeout, now, 0};
        }
        isActive = false;
    }
    return (anEvent){noEvent, now, 0};
}


// Action and churn functions
void churnShowCurrentTimeSteady()
{}

void churnShowCurrentTimeFlash()
{}

void tranSetCurrentTime()
{}

void tranToggleAlarm()
{}

void churnShowAlarmStatus()
{}

void churnShowAlarmTime()
{}

void tranSetAlarmTime()
{}

void tranEmptyTransition()
{}


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