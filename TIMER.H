/****************************************************************************
        A very simple timing unit by amit moscovich.

        Use StartTimer near the start of the main loop and DisplayTime after
        the loop's end.
*****************************************************************************/

#ifndef _TIMER_H_
#define _TIMER_H_

extern unsigned long* const CurrentTime;
extern const float TimerResolution;

extern void StartTimer();
extern void DisplayTime(unsigned long LoopCycles);

#endif
