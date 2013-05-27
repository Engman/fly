#ifndef TIMER_H
#define TIMER_H

#include <windows.h>


class Timer
{
	private:
		INT64 frequency;
		INT64 ticksPerFrame;
		INT64 startTime;
		INT64 ticksLastFrame;

		float ticksPerMs;
		float frameTime;
		float fps;

	public:
		Timer();
		Timer(const Timer& other);
		~Timer();

		bool Initialize();
		void Frame();

		/** Returns time between two frames in seconds */
		float GetDeltaTime();
		float GetFPS();
		float GetTime();
};

#endif