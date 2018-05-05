#pragma once
#include "./System/Window.h"

class Environment;
class Character;
class TestCube;
class RenderingManager;

class GameMain : public Window
{
public:
	void Initialize();
	void Destroy();
	void Update();
	void Render();

private:
	RenderingManager * renderManager;
	void ControlCamera();


	Environment* environment;



	TestCube* testcube;
	
	Character* player;

	
	int testvalue = 0;
};