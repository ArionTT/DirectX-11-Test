#include "Time.h"



Time::Time()
{
	currentTime = timeGetTime()*0.001f;
	lastTime = currentTime;
	deltaTime = 0.0f;
	Fps = 0.0f;
}

Time::~Time()
{
}

void Time::Frame()
{
	currentTime = timeGetTime()*0.001f;
	deltaTime = currentTime - lastTime;
	lastTime = currentTime;
	Fps = 1 / deltaTime;
}

float Time::GetDeltaTime()
{
	return deltaTime;
}

float Time::GetFrame()
{
	float frame = 0.0f;

	return frame;
}

int Time::GetFps()
{
	return Fps;
}

