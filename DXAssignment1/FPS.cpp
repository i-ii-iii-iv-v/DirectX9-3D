#include "FPS.h"

void gameA::FPS::start()
{
	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
}

void gameA::FPS::countFrames()
{
	INT64 newCount = 0;
	static INT64 lastCount = 0;
	INT64 difference = 0;

	QueryPerformanceCounter((LARGE_INTEGER*)&newCount);

	frameCount++;

	difference = newCount - lastCount;
	if (difference >= frequency)
	{
		frameRate = frameCount;
		frameCount = 0;

		lastCount = newCount;
	}
	

}

INT64 gameA::FPS::getFrameRate()
{
	return frameRate;
}
