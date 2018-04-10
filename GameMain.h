#pragma once
#include "./System/Window.h"


class RenderTexture;

class Skydome;
class Landscape;
class Water;
class Skyplane;
class TerrainGrass;
class RainCone;
class TestCube;

class GrassTexture;
class PerlinNoise;


class ShaderManager;
class CascadeShadowBuffer;



class Frustum;

class OrthoWindowPlane;

class VPBuffer;
class LightBuffer;


class Character;
class GameMain : public Window
{
public:
	void Initialize();
	void Destroy();
	void Update();
	void PreRender();
	void Render();

private:

	void ControlCamera();


	RenderTexture* depthShadowTexture;
	RenderTexture* shadowTexture;
	RenderTexture* blurShadowTexture;

	RenderTexture* lakeReflectionTexture;
	RenderTexture* lakeRefractionTexture;

	RenderTexture* rainTexture;
	RenderTexture* mainRendering;
	RenderTexture* postRendering;

	Frustum* frustum;

	GrassTexture* grassTexture;
	PerlinNoise* noise;

	Skydome* skydome;
	Skyplane* cloud;
	Landscape* landscape;
	Water* lake;
	TerrainGrass* grass;
	RainCone* rainCone;


	TestCube* testcube;
	



	OrthoWindowPlane* orthoWindow;

	ShaderManager* shaderManager;
	CascadeShadowBuffer* shadowBuffer;

	VPBuffer* vpBuffer;
	LightBuffer* lightBuffer;



	Character* player;







	int testvalue = 0;
};