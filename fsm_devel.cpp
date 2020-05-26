#include <stdio.h>

enum eventType
{
    none,
    buttonPress,
    longButtonPress,
    timeout,
    rotation
};

struct anEvent
{
    eventType type;
    uint32_t eventTime;
    uint32_t rotation_ticks;
};

struct transitionTuple
{
    eventType event;
    char newStateName[50];
    void (*transitionFunc)(void);
};

struct FSMState
{
    char name[50];
    void (*churnFunc)(void);
    transitionTuple tran[5];
};

struct FSM
{
    FSMState state[4];
};

void showCurrentTimeSteady()
{}

void showCurrentTimeFlash()
{}

void setCurrentTime()
{}

void toggleAlarm()
{}

void showAlarmStatus()
{}

void showAlarmTime()
{}

void setAlarmTime()
{}

void emptyTransition()
{}

int main(int argc, char const *argv[])
{
    FSM fsm = 
        { // initialise struct
            { // array
                { // zeroth array element
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
                    }
                },
                {
                    "showAlarmTime", showAlarmTime,
                    {
                        {timeout, "showCurrentTime", emptyTransition},
                        {rotation, "showAlarmTime", setAlarmTime}
                    }
                },
                {
                    "setCurrentTime", showCurrentTimeFlash,
                    {
                        {timeout, "showCurrentTime", emptyTransition},
                        {rotation, "setCurrentTime", setCurrentTime}
                    }
                }
            }
        };
    
    printf("%s", fsm.state[0].name);
    printf("");
    
    return 0;
}
