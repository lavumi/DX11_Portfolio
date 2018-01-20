#pragma once

void TW_CALL Notified(void* value);

class TweakBar
{
public:
	static TweakBar* Get();
	static void Delete();

	void Start();
	void InputProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);
	void Render();

	static void Add(void* value, function<void(void *)> func);
	static void Remove(void* value);
	static void CallFunc(void* value);

	void GetDraw(bool* iconified)
	{
		*iconified = this->iconified;
	}

	void SetDraw(bool iconified)
	{
		this->iconified = iconified;

		if(iconified == true)
			TwDefine("TweakBar iconified=true");
		else
			TwDefine("TweakBar iconified=false");
	}

	void ChangeDraw()
	{
		SetDraw(!iconified);
	}

	TwBar* GetBar()
	{
		return bar;
	}

private:
	TweakBar();
	~TweakBar();

	static TweakBar* instance;
	static map<void *, function<void(void *)>> funcMap;

	bool iconified;
	UINT width;
	TwBar* bar;
};