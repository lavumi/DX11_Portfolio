#pragma once
#include "./System/Window.h"


class RenderTexture;

class Skydome;
class Landscape;
class Water;
class Skyplane;
	

class Grass;
class MosaicTile;


class TestCube;
class Mirror;

class NormalMapShader;
class DepthShadowShader;
class TextureShader;
class SkydomeShader;
class ShadowShader;
class BlurShader;
class MirrorShader;
class TerrianShader;
class ColorShader;
class SkyplaneShader;
class WaterShader;

class Frustum;

class OrthoWindowPlane;
class GameMain : public Window
{   
public:
	void Initialize();
	void Destroy();
	void Update();
	void PreRender();
	void Render();

private:

	static bool landscapeWireFrame;


	void ControlCamera();


	RenderTexture* depthShadowTexture;
	RenderTexture* shadowTexture;
	RenderTexture* blurShadowTexture;

	RenderTexture* lakeReflectionTexture;
	RenderTexture* lakeRefractionTexture;

	Frustum* frustum;

	Grass* grass;
	//MosaicTile* mosaicTile;
	//RNDNoise* noise;

	Skydome* skydome;
	Skyplane* cloud;
	Landscape* landscape;
	Water* lake;


	TestCube* testcube;
	Mirror* testplane;



	OrthoWindowPlane* shadowtestPlane;



	SkydomeShader* skydomeShader;
	NormalMapShader* normalMapShader;
	DepthShadowShader* depthShadowShader;
	TextureShader* textureShader;
	ShadowShader* shadowShader;
	BlurShader* blurShader;
	MirrorShader* mirrorShader;
	TerrianShader* terrianShader;
	ColorShader* colorShader;
	SkyplaneShader* skyplaneShader;
	WaterShader* waterShader;
};