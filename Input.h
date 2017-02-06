#pragma once

#pragma comment(lib,"dinput8.lib")
#pragma comment (lib,"dxguid.lib")

#include<dinput.h>

#define KEYDOWN(name,key)  (name[key]&0x80)
#define BUTTONDOWN(name, key)  (name.rgbButtons[key]&0x80)
#define MB_LEFT 0
#define MB_RIGHT 1


class Input
{
public:
	Input();
	~Input();
	bool Initialize(HINSTANCE hInstance,HWND hwnd,int width,int height);
	bool Frame();

public:
	float GetMoveX();
	float GetMoveY();
	int GetAxisX();
	int GetAxisY();

	bool isAKeyDown();
	bool isSKeyDown();
	bool isWKeyDown();
	bool isDKeyDown();
	bool isMouseLeftDown();
	bool isMouseRightDown();
	bool isSpaceDown();

private:
	bool ReadKeyboard();
	bool ReadMouse();
	bool Process();

private:
	float Xlen;
	float Ylen;

	float AxisX;
	float AxisY;
	bool mouseDown[2];
	bool WKeyDown;
	bool SKeyDown;
	bool AKeyDown;
	bool DKeyDown;
	bool SpaceDown;

private:
	IDirectInput8* m_directInput;
	IDirectInputDevice8* m_keyboard;
	IDirectInputDevice8* m_mouse;
	DIMOUSESTATE m_mouseState;
	
	int m_screenWidth, m_screenHeight;
	int m_mouseX;
	int m_mouseY;
	unsigned char m_keyboardState[256];

};

