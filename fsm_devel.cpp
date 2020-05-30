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
                    "showCurrentTime", showCurrentTimeSteady, // TODO TODO make state names enums, starting with 0 = invalidState
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
    
    // Print out contents of FSM
    printf("%p\n%p\n%p\n%p\n%p\n%p\n%p\n%p\n",
        showCurrentTimeSteady,
        showCurrentTimeFlash,
        setCurrentTime,
        toggleAlarm,
        showAlarmStatus,
        showAlarmTime,
        setAlarmTime,
        emptyTransition);

    for (uint8_t i = 0; i < 4; i++)
    {
        printf("%s\t%p\n", fsm.state[i].name, fsm.state[i].churnFunc);
        for (uint8_t j = 0; j < 5; j++)
        {
            printf("\t(%d\t%s\t%p)\n", fsm.state[i].tran[j].event, fsm.state[i].tran[j].newStateName, fsm.state[i].tran[j].transitionFunc);
        }
    }
    return 0;
}
