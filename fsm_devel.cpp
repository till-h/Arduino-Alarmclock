#include <stdio.h>

enum eventType
{
    noEvent,
    buttonPress,
    longButtonPress,
    timeout,
    rotation
};

enum softwareState
{
    invalidState,
    showCurrentTime,
    setCurrentTime,
    showAlarmTime,
    toggleAlarm
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

int main(int argc, char const *argv[])
{
    FSM fsm = 
        { // initialise struct
            { // array
                { // zeroth array element
                    showCurrentTime, churnShowCurrentTimeSteady,
                    {
                        {rotation, showAlarmTime, tranEmptyTransition},
                        {buttonPress, toggleAlarm, tranToggleAlarm},
                        {longButtonPress, setCurrentTime, tranEmptyTransition}
                    }
                },
                {
                    toggleAlarm, churnShowAlarmStatus,
                    {
                        {timeout, showCurrentTime, tranEmptyTransition}
                    }
                },
                {
                    showAlarmTime, churnShowAlarmTime,
                    {
                        {timeout, showCurrentTime, tranEmptyTransition},
                        {rotation, showAlarmTime, tranSetAlarmTime}
                    }
                },
                {
                    setCurrentTime, churnShowCurrentTimeFlash,
                    {
                        {timeout, showCurrentTime, tranEmptyTransition},
                        {rotation, setCurrentTime, tranSetCurrentTime}
                    }
                }
            }
        };
    
    // Print out contents of FSM
    printf("%p\n%p\n%p\n%p\n%p\n%p\n%p\n%p\n",
        churnShowCurrentTimeSteady,
        churnShowCurrentTimeFlash,
        tranSetCurrentTime,
        tranToggleAlarm,
        churnShowAlarmStatus,
        churnShowAlarmTime,
        tranSetAlarmTime,
        tranEmptyTransition);

    for (uint8_t i = 0; i < 4; i++)
    {
        printf("%d\t%p\n", fsm.state[i].state, fsm.state[i].churnFunc);
        for (uint8_t j = 0; j < 5; j++)
        {
            printf("\t(%d\t%d\t%p)\n", fsm.state[i].tran[j].event, fsm.state[i].tran[j].newState, fsm.state[i].tran[j].transitionFunc);
        }
    }
    return 0;
}
