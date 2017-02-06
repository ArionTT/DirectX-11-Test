#include "Input.h"


Input::Input()
{
	m_directInput = NULL;
	m_keyboard = NULL;
	m_mouse = NULL;

	Xlen = 0.1f;
	Ylen = 0.1f;
}

Input::~Input()
{
}

bool Input::Initialize(HINSTANCE hInstance, HWND hWnd, int width, int height)
{
	HRESULT hr;

	m_screenHeight = height;
	m_screenWidth = width;
	m_mouseX = 0;
	m_mouseY = 0;

	hr = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);
	if (FAILED(hr))
	{
		MessageBox(hWnd, L"Input Create Failed ", L"Error", MB_OK);
		return false;
	}

	//----KeyBoard----
	hr = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
	if (FAILED(hr))
	{
		MessageBox(hWnd, L"KeyBoard Create Failed ", L"Error", MB_OK);
		return false;
	}

	hr = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
	{
		MessageBox(hWnd, L"Keyboard SetFormat Failed ", L"Error", MB_OK);
		return false;
	}

	// Set the cooperative level of the keyboard to not share with other programs.
	hr = m_keyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(hr))
	{
		MessageBox(hWnd, L"Keyboard Set Cooperative Failed", L"Error", MB_OK);
		return false;
	}
	// Now acquire the keyboard.
	hr = m_keyboard->Acquire();
	if (FAILED(hr))
	{
		MessageBox(hWnd, L"Keyboard Acquire Failed", L"Error", MB_OK);
		return false;
	}

	//----Mouse----
	hr = m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
	if (FAILED(hr))
	{
		MessageBox(hWnd, L"Mouse Create Failed", L"Error", MB_OK);
		return false;
	}

	hr = m_mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(hr))
	{
		MessageBox(hWnd, L"Mouse Set Format Failed", L"Error", MB_OK);
		return false;
	}

	hr = m_mouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(hr))
	{
		MessageBox(hWnd, L"Mouse Set Cooperative Failed", L"Error", MB_OK);
		return false;
	}

	hr = m_mouse->Acquire();
	if (FAILED(hr))
	{
		MessageBox(hWnd, L"Mouse Acquire Failed", L"Error", MB_OK);
		return false;
	}
	return true;
}

bool Input::ReadKeyboard()
{
	HRESULT hr;

	hr = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
	if (FAILED(hr))
	{
		// If the keyboard lost focus or was not acquired then try to get control back.
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			m_keyboard->Acquire();
		}
		else
		{
			return false;
		}
	}
	return true;
}

bool Input::ReadMouse()
{
	HRESULT result;

	// Read the mouse device.
	result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
	if (FAILED(result))
	{
		// If the mouse lost focus or was not acquired then try to get control back.
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_mouse->Acquire();
		}
		else
		{
			return false;
		}
	}
	return true;
}

bool Input::Process()
{
	m_mouseX += m_mouseState.lX;
	m_mouseY += m_mouseState.lY;

	//// Ensure the mouse location doesn't exceed the screen width or height.
	//if (m_mouseX < 0) { m_mouseX = 0; }
	//if (m_mouseY < 0) { m_mouseY = 0; }

	//if (m_mouseX > m_screenWidth) { m_mouseX = m_screenWidth; }
	//if (m_mouseY > m_screenHeight) { m_mouseY = m_screenHeight; }

	return true;
}

bool Input::Frame()
{
	//Keyboard Key
	HRESULT hr = S_OK;
	hr=m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
	if (FAILED(hr))
	{
		// If the keyboard lost focus or was not acquired then try to get control back.
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			m_keyboard->Acquire();
		}
		else
		{
			MessageBox(NULL, L"Get Mouse Device Failed ", L"Error", MB_OK);
			return false;
		}
	}
	if (KEYDOWN(m_keyboardState, DIK_A))
		this->AKeyDown = true;
	else
		this->AKeyDown = false;
	if (KEYDOWN(m_keyboardState, DIK_D))
		this->DKeyDown = true;
	else
		this->DKeyDown = false;
	if (KEYDOWN(m_keyboardState, DIK_W))
		this->WKeyDown = true;
	else
		this->WKeyDown = false;
	if (KEYDOWN(m_keyboardState, DIK_S))
		this->SKeyDown = true;
	else
		this->SKeyDown = false;
	if (KEYDOWN(m_keyboardState, DIK_SPACE))
		this->SpaceDown = true;
	else
		this->SpaceDown = false;

	//Mouse Button 
	hr=m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
	if (FAILED(hr))
	{
		// If the mouse lost focus or was not acquired then try to get control back.
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			m_mouse->Acquire();
		}
		else
		{
			MessageBox(NULL, L"Get Mouse Device Failed ", L"Error", MB_OK);
			return false;
		}
	}
	if (BUTTONDOWN(m_mouseState, MB_LEFT))
		mouseDown[0] = true;
	else
		mouseDown[0] = false;
	if (BUTTONDOWN(m_mouseState, MB_RIGHT))
		mouseDown[1] = true;
	else
		mouseDown[1] = false;
	m_mouseX = m_mouseState.lX;
	m_mouseY = m_mouseState.lY;

	return true;
}

float Input::GetMoveX()
{
	if (m_keyboardState[DIK_A] & 0x80)
		return -1.0f;
	else if (m_keyboardState[DIK_D] & 0x80)
		return 1.0f;
	else
		return 0.0f;
}

float Input::GetMoveY()
{
	if (m_keyboardState[DIK_W] & 0x80)
		return 1.0f;
	else if (m_keyboardState[DIK_S] & 0x80)
		return -1.0f;
	else
		return 0.0f;
}

int Input::GetAxisX()
{
	return m_mouseX;
}

int Input::GetAxisY()
{
	return m_mouseY;
}

bool Input::isAKeyDown()
{
	return AKeyDown;
}

bool Input::isSKeyDown()
{
	return SKeyDown;
}

bool Input::isWKeyDown()
{
	return WKeyDown;
}

bool Input::isDKeyDown()
{
	return DKeyDown;
}

bool Input::isMouseLeftDown()
{
	return mouseDown[0];
}

bool Input::isMouseRightDown()
{
	return mouseDown[1];
}

bool Input::isSpaceDown()
{
	return SpaceDown;
}
