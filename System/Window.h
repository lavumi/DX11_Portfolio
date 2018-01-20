#pragma once
class Window
{
public:
	Window();
	virtual ~Window();

	WPARAM Run();

protected:
	virtual void Initialize() = 0;
	virtual void Destroy() = 0;
	virtual void Update() = 0;
	virtual void PreRender() = 0;
	virtual void Render() = 0;
};