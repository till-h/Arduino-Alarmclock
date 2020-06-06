#include "Events.h"
#include "ClockConfig.h"


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

// TODO use internal FSM to distinguish long / short button presses.
anEvent ButtonSource::poll()
{
    uint32_t now = micros();
    if (buttonChange == released)
    {
        // if ((now - lastButtonChange < longPressThreshold) && (buttonChange == pressed))
        // {
        //     unservedButtonChange = false;
        //     return (anEvent){buttonPress, lastButtonChange, 0};
        // }
        // else if (unservedButtonChange)
        // {
        //     unservedButtonChange = false;
        //     return (anEvent){longButtonPress, lastButtonChange, 0};
        // }
        // else
        // {
        //     return (anEvent){none, micros(), 0};
        // }
        buttonChange = none;
        return (anEvent){buttonPress, now, 0};
    }
}


RotationSource::RotationSource(uint8_t pin1, uint8_t pin2)
{
    dial = RotaryDial(pin1, pin2);
}

void RotationSource::poll()
{
    int32_t rot = dial.getRotation();
    uint32_t now = micros();
    if (rotation != 0)
    {
        return (anEvent){EventType::rotation, now, rot};
    }
    else
    {
        return (anEvent){noEvent, now, 0};
    }
}


TimerSource::TimerSource():
    delay(0),
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

void tranSetCurrentTime(anEvent event)
{}

void tranToggleAlarm(anEvent event)
{}

void churnShowAlarmStatus()
{}

void churnShowAlarmTime()
{}

void tranSetAlarmTime(anEvent event)
{}

void tranEmptyTransition(anEvent event)
{}


// Scheduler

Scheduler::Scheduler(FSM fsm)
{
    uint8_t numSources = 3;
    source[1] = ButtonSource(PUSH, 3000000); // TODO use 3E6 possible?
    source[2] = RotationSource(ENC1, ENC2);
    source[3] = TimerSource();

    state = showCurrentTime;
}

Scheduler::run()
{
    while true
    {
        // run churn function of current state
        runChurnFunc(state);

        // poll events
        for (uint8_t i = 0; i < numSources; i++)
        {
            anEvent event = source[i].poll();
            if (event.type != noEvent)
            {
                runTranFunc(state, event);
                state = getNewState(state, event);
                break; // break out of polling sources for the old state
            }
        }
    }
}

uint8_t Scheduler::getIndex(softwareState state)
{
    for (uint8_t i = 0; i < fsm.numStates; i++)
    {
        if (fsm.state[i].state == state)
        {
            return i;
        }   
    }
    return 0;
}

void Scheduler::runChurnFunc(softwareState state)
{
    fsm.state[getIndex(state)].churnFunc();
}

uint8_t getTranIndex(uint8_t stateIndex, eventType event)
{
    for (uint8_t i = 0; i < fsm.state[stateIndex].numTrans; i++)
    {
        if (fsm.state[stateIndex].tran[i].event == event)
        {
            return i;
        }
    }
    return 0;
}

void Scheduler::runTranFunc(softwareState state, anEvent event)
{
    uint8_t stateIndex = getIndex(state);
    uint8_t tranIndex = getTranIndex(stateIndex, event.type);
    fsm.state[stateIndex].tran[tranIndex].transitionFunc(event);
}