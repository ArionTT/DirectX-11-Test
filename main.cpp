
#include"Application.h"
#include<Windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	bool hr;
	Application*app = new Application();

	hr=app->Initialize(hInstance,nCmdShow,800,600);
	if (!hr)
		return hr;

	app->Run();

	return 0;
}