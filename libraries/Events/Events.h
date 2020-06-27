#ifndef Events_h
#define Events_h

#include "ClockUtil.h"
#include "RotaryDial.h"


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
    uint32_t rotationTicks;
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
    void (*transitionFunc)(anEvent event);
};

struct FSMState
{
    const static uint8_t numTrans = 5;
    softwareState state;
    void (*churnFunc)(void);
    transitionTuple tran[numTrans];
};

struct FSM
{
    const static uint8_t numStates = 4;
    FSMState state[numStates];
};


// Event sources

class EventSource
{
    public:
        virtual void poll(anEvent * e) = 0;
};

// button change events (it has just been pressed etc.)
enum _buttonChange {press, release, disregardRelease, none};
// button state (it currently is down etc.)
//enum _buttonState {pressed, released, initialUndefined};


// TODO Build FSM internal to Button class to distinguish short/long button press.
// NOTE: Deciding whether the button was pressed long or short should be handled inside the ButtonSource class,
// probably inside the poll function (where the duration since the last change is checked).
// The long/short press differentiation should not seep out into the top-level FSM.
// If it did, we'd probably require more states there and that's naff.
class ButtonSource: EventSource
{
    public:
        ButtonSource(uint8_t pin, uint32_t longPressThreshold);
        void poll(anEvent * e);
    private:
        static uint8_t pin;        
        static uint32_t buttonChangeTime;
        uint32_t longPressThreshold;
        static _buttonChange buttonChange;
        static void buttonCallback();
};

class RotationSource: EventSource
{
    public:
        RotationSource(uint8_t pin1, uint8_t pin2);
        void poll(anEvent * e);
    private:
        RotaryDial dial;
};

class TimerSource: EventSource
{
    public:
        TimerSource();
        TimerSource(uint32_t delay);
        void start();
        void start(uint32_t delay);
        void cancel();
        void poll(anEvent * e);
    private:
        bool isActive;
        uint32_t delay;
        uint32_t startTime;
};


// Scheduler

class Scheduler
{
    public:
        Scheduler();
        void setup(FSM fsm, softwareState initialState);
        void run();

        ButtonSource butSrc;
        RotationSource rotSrc;
        TimerSource timSrc;

    private:
        softwareState state;
        FSM fsm;

        softwareState getNewState(softwareState state, anEvent event);
        uint8_t getIndex(softwareState state);
        void runChurnFunc(softwareState state);
        uint8_t getTranIndex(uint8_t stateIndex, eventType event);
        void runTranFunc(softwareState state, anEvent event);
};

#endif /* Events_h */