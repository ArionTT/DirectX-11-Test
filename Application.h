#pragma once

#include<Windows.h>
#include<string>
#include"Device.h"



class Application
{
public:
	Application();
	~Application();
	bool Initialize(HINSTANCE hInstance,int nCmdShow, int width, int height);
	bool Run();
	bool SetWindowTitle(std::wstring title);

public:
	virtual bool InitializeDevice(HINSTANCE hinstance,HWND hwnd,int width,int height);

public:
	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

protected:
	HWND hwnd;
	HINSTANCE hinstance;

	Device*device;
};

static Application*ApplicationHandle = NULL;
static LRESULT CALLBACK Process(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

