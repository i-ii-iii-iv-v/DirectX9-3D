#pragma once
#include <windows.h>
namespace gameA {

	class FPS
	{
	public:
		void start();
		void countFrames();
		INT64 getFrameRate();
	private:
		INT64 frequency = 0;//ticks per second
		INT64 frameRate = 0;//frame rate
		INT64 frameCount = 0;
	};
}