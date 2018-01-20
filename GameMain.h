#pragma once
#include "./System/Window.h"


class RenderTexture;

class Skydome;
class Landscape;
class Water;

class Grass;
class MosaicTile;


class TestCube;
class Mirror;

class NormalMapShader;
class DepthShadowShader;
class SimpleShader;
class SkydomeShader;
class ShadowShader;
class BlurShader;
class MirrorShader;
class TerrianShader;

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


	Frustum* frustum;

	Grass* grass;
	//MosaicTile* mosaicTile;
	//RNDNoise* noise;

	Skydome* skydome;
	Landscape* landscape;
	Water* lake;


	TestCube* testcube;
	//Mirror* testplane;



	OrthoWindowPlane* shadowtestPlane;



	SkydomeShader* skydomeShader;
	NormalMapShader* normalMapShader;
	DepthShadowShader* depthShadowShader;
	SimpleShader* simpleShader;
	ShadowShader* shadowShader;
	BlurShader* blurShader;
	MirrorShader* mirrorShader;
	TerrianShader* terrianShader;
};