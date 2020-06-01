#ifndef Events_h
#define Events_h

#include "ClockUtil.h"


// Events

enum eventType
{
    noEvent,
    buttonPress,
    longButtonPress,
    timeout,
    rotation
};

// All-purpose structure for events
// Depending on the event type, subsequent fields are meaningful or not.
// TODO Could this be handled better by polymorphic structs?
struct anEvent
{
    eventType type;
    uint32_t eventTime;
    uint32_t rotation_ticks;
};


// Event sources

class EventSource
{
    public:
        anEvent poll() = 0;
    private:
};

// TODO Build FSM internal to Button class to distinguish short/long button press.
// NOTE: Deciding whether the button was pressed long or short should be handled inside the ButtonSource class,
// probably inside the poll function (where the duration since the last change is checked).
// The long/short press differentiation should not seep out into the top-level FSM.
// If it did, we'd probably require more states there and that's naff.
class ButtonSource: EventSource
{
    public:
        ButtonSource(uint8_t pin, uint32_t longPressThreshold);
        anEvent poll();
    private:
        uint8_t pin;
        uint32_t lastButtonChange;
        uint32_t longPressThreshold;
        enum {press, release, none} buttonChange; // button change events (it has just been pressed etc.)
        enum {pressed, released, initialUndefined} buttonState; // button state (it currently is down etc.)
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
        void start(uint32_t delay);
        void start();
        anEvent poll();
    private:
        bool isActive;
        uint32_t delay;
        uint32_t startTime;
};


// States and FSM

enum softwareState
{
    invalidState,
    showCurrentTime,
    setCurrentTime,
    showAlarmTime,
    toggleAlarm
};



struct transitionTuple
{
    eventType event;
    softwareState newState;
    void (*transitionFunc)(void);
};

struct FSMState
{
    softwareState state;
    void (*churnFunc)(void);
    transitionTuple tran[5];
};

struct FSM
{
    FSMState state[4];
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