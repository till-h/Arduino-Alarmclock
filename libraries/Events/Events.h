#ifndef Events_h
#define Events_h

#include "ClockUtil.h"

enum eventType
{
    none,
    buttonPress,
    buttonLongPress,
    timeout,
    rotation
};

struct anEvent
{
    eventType type;
    uint32_t eventTime;
};

class EventSource
{
    public:
        anEvent poll() = 0;
    private:
};

class ButtonSource: EventSource
{
    public:
        ButtonSource(uint8_t pin);
        anEvent poll();
    private:
        uint8_t pin;
        uint32_t lastButtonRelease;
        bool unservedButtonRelease;
        void buttonCallback();
};

class RotationSource: EventSource
{
    public:
        RotationSource(uint8_t pin1, uint8_t pin2);
        anEvent poll();
    private:
};

class TimerSource: EventSource
{
    public:
        TimerSource(uint32_t delay);
        anEvent poll();
    private:
};

struct transitionTuple
{
    eventType event;
    FSMState newState;
    void (*transitionFunc)(void);
};

// state = [event1 -> transitionFunc1, newState1; event2 -> transitionFunc2, newState2; ...], ongoingFunc
struct FSMState
{
        char statename[];
        void (*churnFunc)(void);
        transitionTuple tran[];
};

struct FSM
{
        FSMState state[] =
        {
            {
                "showCurrentTime", showCurrentTimeSteady,
                {
                    {rotation, "showAlarmTime", emptyTransition},
                    {buttonPress, "toggleAlarm", toggleAlarm},
                    {longButtonPress, "setCurrentTime", emptyTransition}
                }
            },
            {
                "toggleAlarm", showAlarmStatus,
                {
                    {timeout, "showCurrentTime", emptyTransition}
                },
            },
            {
                "showAlarmTime", showAlarmTime,
                {
                    {timeout, "showCurrentTime", emptyTransition},
                    {rotation, "showAlarmTime", updateAlarmTime}
                }
            },
            {
                "setCurrentTime", showCurrentTimeFlash,
                {
                    {timeout, "showCurrentTime", emptyTransition},
                    {rotation, "setCurrentTime", updateCurrentTime}
                }
            }
        };
};


class Scheduler
{
    public:
        Scheduler();
        void run();
    private:
        uint8_t numSources;
        EventSource source[3];

};

#endif /* Events_h */