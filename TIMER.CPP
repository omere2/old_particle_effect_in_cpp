#include "timer"
#include <iostream>

unsigned long* const CurrentTime=(unsigned long*)0x46C;
const float TimerResolution=18.2;
unsigned long StartTime;

void StartTimer()
{
        StartTime=*CurrentTime;
}

void DisplayTime(unsigned long LoopCycles)
{
        cout << "Seconds passed since timer start: " << (*CurrentTime-StartTime)/TimerResolution << ".\n";
        cout << "Frames per second: " << LoopCycles/((*CurrentTime-StartTime)/TimerResolution) << ".\n";
}