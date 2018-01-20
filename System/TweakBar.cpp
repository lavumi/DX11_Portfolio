#include "../stdafx.h"
#include "TweakBar.h"

void TW_CALL Notified(void* value)
{
	TweakBar::CallFunc(value);
}

TweakBar* TweakBar::instance = NULL;
map<void *, function<void(void *)>> TweakBar::funcMap;

TweakBar * TweakBar::Get()
{
	if (instance == NULL)
		instance = new TweakBar();

	return instance;
}

void TweakBar::Delete()
{
	SAFE_DELETE(instance);
}

void TweakBar::Start()
{
	BOOL result = TwInit(TW_DIRECT3D11, D3D::GetDevice());
	assert(result == TRUE);

	bar = TwNewBar("TweakBar");
	TwDefine("TweakBar color='0 128 255' alpha=128");
	TwDefine("TweakBar valueswidth=120");
	TwDefine("TweakBar movable=true");
	TwDefine("TweakBar iconpos=topleft");
	

	width = 250;
	
	D3DInfo info;
	D3D::GetInfo(&info);

	string option = "TweakBar position='";
	option += to_string(info.screenWidth - width - 5) + " ";
	option += to_string(5) + "'";
	TwDefine(option.c_str());

	option = "TweakBar size='";
	option += to_string(width) + " ";
	option += to_string(info.screenHeight - 10) + "'";
	TwDefine(option.c_str());

	iconified = false;
	SetDraw(iconified);
}

void TweakBar::InputProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	TwEventWin(wnd, msg, wParam, lParam);
}

void TweakBar::Render()
{
	TwDraw();
}

void TweakBar::Add(void * value, function<void(void*)> func)
{
	if (funcMap.count(value) < 1)
		funcMap[value] = func;	
}

void TweakBar::Remove(void * value)
{
	if (funcMap.count(value) > 0)
		funcMap.erase(value);
}

void TweakBar::CallFunc(void * value)
{
	funcMap[value](value);
}

TweakBar::TweakBar()
{
	
}

TweakBar::~TweakBar()
{
	TwTerminate();
}
