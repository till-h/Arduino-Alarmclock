/*  AlarmClock.ino
 *   
 *  A simple alarm clock using a DS3231 clock chip and three 8x8
 *  dot matrix displays to show the current time, and set an alarm.
 *  
 *  Till Hackler, 2015-2020
 */


#include "RotaryDial.h"
#include "Loudspeaker.h"
#include "DotMatrix.h"
#include "DS3231.h"
#include "Events.h"
#include "ClockConfig.h"
#include "ClockUtil.h"


// Hardware control
DS3231 clk;
DotMatrix matrix;
Loudspeaker ls;


// Software control
clockState clkState;
Scheduler scheduler;


// Action and churn functions

void churnShowCurrentTimeSteady()
{
    clk.readTime(&clkState.currentTime);
    matrix.displayTime(clkState.currentTime);
    ls.ring(clkState.alarmIsActive);
}

void churnShowCurrentTimeFlash()
{
    clk.readTime(&clkState.currentTime);
    matrix.blinkTime(clkState.currentTime);
    ls.ring(clkState.alarmIsActive);
}

void tranCacheCurrentTime(anEvent event)
{
    changeTime(&clkState.currentTime, event.rotationTicks);
}

void tranSetCurrentTime(anEvent event)
{
    clk.setTime(clkState.currentTime); // update hardware time
}

void tranToggleAlarm(anEvent event)
{
    scheduler.timSrc.start(1E6);
    clkState.alarmIsActive = !clkState.alarmIsActive;
}

void churnShowAlarmStatus()
{
    matrix.displayAlarm(clkState.alarmIsActive);
    ls.ring(clkState.alarmIsActive);
}

void churnShowAlarmTime()
{
    matrix.displayTime(clkState.alarmTime);
    ls.ring(clkState.alarmIsActive);
}

void tranCacheAlarmTime(anEvent event)
{
    changeTime(&clkState.alarmTime, event.rotationTicks); // update software time
}

void tranSetAlarmTime(anEvent event)
{
    clk.setAlarmTime(clkState.alarmTime); // update hardware time
}

void tranIntoShowAlarmTime(anEvent event)
{
    scheduler.timSrc.start(3E6);
}

void tranEmptyTransition(anEvent event)
{}


// Finite State Machine

FSM fsm = 
    { // initialise struct
        { // array
            { // zeroth array element
                showCurrentTime, churnShowCurrentTimeSteady,
                {
                    {rotation, showAlarmTime, tranIntoShowAlarmTime},
                    {buttonPress, toggleAlarm, tranToggleAlarm},
                    {longButtonPress, setCurrentTime, tranEmptyTransition}
                }
            },
            {
                toggleAlarm, churnShowAlarmStatus,
                {
                    {timeout, showCurrentTime, tranEmptyTransition},
                    {buttonPress, toggleAlarm, tranToggleAlarm}
                }
            },
            {
                showAlarmTime, churnShowAlarmTime,
                {
                    {timeout, showCurrentTime, tranSetAlarmTime},
                    {rotation, showAlarmTime, tranCacheAlarmTime}
                }
            },
            {
                setCurrentTime, churnShowCurrentTimeFlash,
                {
                    {timeout, showCurrentTime, tranSetCurrentTime},
                    {rotation, setCurrentTime, tranCacheCurrentTime}
                }
            }
        }
    };


// Setup code

void setup()
{ 
    matrix.setup(DIN, CLK, CS, 3, 0, 3E5);
    ls.initialise(BEEP);

    clk.readTime(&clkState.currentTime);
    clk.readAlarmTime(&clkState.alarmTime);
    clkState.alarmIsActive = false;

    scheduler.setup(fsm, showCurrentTime);

}

// Program loop
void loop()
{
    scheduler.run();
}