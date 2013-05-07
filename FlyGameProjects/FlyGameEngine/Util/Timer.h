#ifndef TIMER_H
#define TIMER_H

#include <windows.h>


class Timer
{
	private:
		INT64 frequency;
		INT64 ticksPerFrame;
		INT64 startTime;

		float ticksPerMs;
		float frameTime;

	public:
		Timer();
		Timer(const Timer& other);
		~Timer();

		bool Initialize();
		void Frame();	
};

#endif