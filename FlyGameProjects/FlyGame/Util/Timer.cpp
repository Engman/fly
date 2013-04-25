#include "Timer.h"
#include <conio.h>
#include <windows.h>

Timer::Timer()
{

}


Timer::Timer(const Timer& other)
{

}


Timer::~Timer()
{

}

bool Timer::Initialize()
{
	QueryPerformanceFrequency((LARGE_INTEGER*)&this->frequency);
	if(this->frequency == 0)
	{
		return false;
	}

	this->ticksPerMs = (float)(this->frequency / 1000);

	QueryPerformanceCounter((LARGE_INTEGER*)&this->startTime);

	this->ticksPerFrame = (INT64)(this->ticksPerMs*1000)/60;

	return true;
}

void Timer::Frame()
{
	INT64 currentTime;
	float timeDifference;


	QueryPerformanceCounter((LARGE_INTEGER*)& currentTime);

	timeDifference = (float)(currentTime - this->startTime);

	if(this->ticksPerFrame - timeDifference > 0)
		Sleep((DWORD)((this->ticksPerFrame - timeDifference)/this->ticksPerMs));

	this->startTime = currentTime;

	return;
}

