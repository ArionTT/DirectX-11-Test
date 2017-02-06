#include "Application.h"



Application::Application()
{
	hwnd = NULL;
	hinstance = NULL;
	device = new Device();
}

Application::~Application()
{
}

bool Application::Initialize(HINSTANCE hInstance, int nCmdShow, int width, int height)
{
	bool hr = true;

	ApplicationHandle = this;

	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpfnWndProc = Process;
	wc.lpszMenuName = NULL;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.lpszClassName = L"MyDirectX";
	wc.cbClsExtra = NULL;
	wc.cbWndExtra = NULL;

	if (!RegisterClass(&wc))
		return false;
	hinstance = hInstance;
	hwnd = CreateWindow(L"MyDirectX", L"DirectX Demo", WS_OVERLAPPEDWINDOW,
		50, 50, width, height, NULL, NULL, hInstance, NULL);

	if (!hwnd)
		return false;

	ShowWindow(hwnd, nCmdShow);

	hr=InitializeDevice(hInstance, hwnd, width, height);
	if (!hr)
		return false;

	return true;
}

bool Application::SetWindowTitle(std::wstring title)
{
	SetWindowText(hwnd, title.c_str());
	return true;
}

bool Application::InitializeDevice(HINSTANCE hinstance, HWND hwnd, int width, int height)
{
	HRESULT hr=S_OK;
	hr=device->Initialize(hinstance,hwnd, width, height);
	if (FAILED(hr))
	{
		MessageBox(hwnd, L"Initialize D3D Device Failed!", L"Error", MB_OK);
		return false;
	}
	return true;
}

bool Application::Run()
{
	bool done = false;
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (WM_QUIT == msg.message)
			done = true;
		else
		{
			device->Update();
			device->Render();
		}
	}
	return true;
}

LRESULT CALLBACK Application::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}

LRESULT CALLBACK Process(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_KEYDOWN:
		break;
	case WM_KEYUP:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	default:
		//return ApplicationHandle->WndProc(hWnd, message, wParam, lParam);
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}
