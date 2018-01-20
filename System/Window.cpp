#include "../stdafx.h"
#include "Window.h"
#include "../GameMain.h"

LRESULT CALLBACK WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	Mouse::Get()->InputProc(message, wParam, lParam);

	if (TweakBar::Get() != NULL)
		TweakBar::Get()->InputProc(handle, message, wParam, lParam);

	if (message == WM_CLOSE || message == WM_DESTROY)
	{
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(handle, message, wParam, lParam);
}

Window::Window()
{
	D3DInfo info;
	D3D::GetInfo(&info);

	WNDCLASSEX wndClass;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wndClass.hIconSm = wndClass.hIcon;
	wndClass.hInstance = info.instance;
	wndClass.lpfnWndProc = (WNDPROC)WndProc;
	wndClass.lpszClassName = info.appName.c_str();
	wndClass.lpszMenuName = NULL;
	wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndClass.cbSize = sizeof(WNDCLASSEX);

	WORD wHr = RegisterClassEx(&wndClass);
	assert(wHr != 0);

	if (info.isFullScreen == true)
	{
		DEVMODE devMode = { 0 };
		devMode.dmSize = sizeof(DEVMODE);
		devMode.dmPelsWidth = info.screenWidth;
		devMode.dmPelsHeight = info.screenHeight;
		devMode.dmBitsPerPel = 32;
		devMode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&devMode, CDS_FULLSCREEN);
	}


	info.mainHandle = CreateWindowEx
	(
		WS_EX_APPWINDOW
		, info.appName.c_str()
		, info.appName.c_str()
		, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW
		, CW_USEDEFAULT
		, CW_USEDEFAULT
		, CW_USEDEFAULT
		, CW_USEDEFAULT
		, NULL
		, (HMENU)NULL
		, info.instance
		, NULL
	);
	assert(info.mainHandle != NULL);
	D3D::SetInfo(info);

	
	RECT rect = { 0, 0, (LONG)info.screenWidth, (LONG)info.screenHeight };

	UINT centerX = (GetSystemMetrics(SM_CXSCREEN) - info.screenWidth) / 2;
	UINT centerY = (GetSystemMetrics(SM_CYSCREEN) - info.screenHeight) / 2;
	
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
	MoveWindow
	(
		info.mainHandle
		, centerX, centerY
		, rect.right - rect.left, rect.bottom - rect.top
		, TRUE
	);
	ShowWindow(info.mainHandle, SW_SHOWNORMAL);
	SetForegroundWindow(info.mainHandle);
	SetFocus(info.mainHandle);

	ShowCursor(true);
}

Window::~Window()
{
	D3DInfo info;
	D3D::GetInfo(&info);

	if (info.isFullScreen == true)
		ChangeDisplaySettings(NULL, 0);

	DestroyWindow(info.mainHandle);

	UnregisterClass(info.appName.c_str(), info.instance);
}

WPARAM Window::Run()
{
	MSG msg = { 0 };

	D3DInfo info;
	D3D::GetInfo(&info);

	D3D::Get();
	Mouse::Get()->SetHandle(info.mainHandle);
	TweakBar::Get()->Start();
	Frames::Get()->Start();
	//DepthStencil::Get();
	Rasterizer::Get();
	Sampler::Get();
	
	Initialize();

	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Keyboard::Get()->Update();
			Mouse::Get()->Update();
			Frames::Get()->Update();
			
			Update();

			PreRender();
			D3D::Get()->BeginScene();
			{
				Render();

				if (Keyboard::Get()->KeyDown(VK_F11))
					TweakBar::Get()->ChangeDraw();
				TweakBar::Get()->Render();
			}
			D3D::Get()->EndScene();
		}
	}
	Destroy();
	
	Keyboard::Delete();
	Mouse::Delete();
	Rasterizer::Delete();
	Sampler::Delete();
	//DepthStencil::Delete();
	TweakBar::Delete();

	D3D::Delete();

	return msg.wParam;
}