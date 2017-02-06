#pragma once

#pragma comment( lib,"winmm.lib" )

#include<Windows.h>

class Time
{
public:
	Time();
	~Time();
	void Frame();

public:
	float GetDeltaTime();
	float GetFrame();
	int GetFps();

private:
	float deltaTime;
	float currentTime;
	float lastTime;
	float Fps;
};

