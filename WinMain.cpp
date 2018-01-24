#include "stdafx.h"
#include "GameMain.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	srand((UINT)time(NULL));

	D3DInfo info;
	info.appName = L"DirectX Game";
	info.instance = hInstance;
	info.isFullScreen = true;
	info.isVsync = true;
	info.mainHandle = NULL;
	info.screenWidth = 1920;
	info.screenHeight = 1080;
	info.clearColor = D3DXCOLOR(34.0f / 255.0f, 161.0f / 255.0f, 193.0f / 255.0f, 1.0f);
	D3D::SetInfo(info);


	GameMain* gameMain = new GameMain();
	gameMain->Run();
	delete gameMain;

	return 0;
}