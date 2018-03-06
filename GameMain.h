#pragma once
#include "./System/Window.h"


class RenderTexture;

class Skydome;
class Landscape;
class Water;
class Skyplane;
class TerrainGrass;
class Cloud;
class RainCone;


class GrassTexture;
class MosaicTile;
class PerlinNoise;


class TestCube;
class Mirror;

class NormalMapShader;
class DepthShadowShader;
class TextureShader;
class SkydomeShader;
class ShadowShader;
class BlurShader;
class TerrainShader;
class ColorShader;
class SkyplaneShader;
class WaterShader;
class InstanceTextureShader;
class RainShader;
class GrassShader;

class FBXModelShader;

class Frustum;

class OrthoWindowPlane;

class VPBuffer;


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

	static bool landscapeWireFrame;
	bool LodOff;


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

	//MosaicTile* mosaicTile;
	PerlinNoise* noise;

	Skydome* skydome;
	Skyplane* cloud;
	Landscape* landscape;
	Water* lake;
	TerrainGrass* grass;
	//Cloud* volumeCloud;
	RainCone* rainCone;


	TestCube* testcube;
	Mirror* testplane;



	OrthoWindowPlane* orthoWindow;



	SkydomeShader* skydomeShader;
	NormalMapShader* normalMapShader;
	DepthShadowShader* depthShadowShader;
	TextureShader* textureShader;
	ShadowShader* shadowShader;
	BlurShader* blurShader;
	TerrainShader* terrainShader;
	ColorShader* colorShader;
	SkyplaneShader* skyplaneShader;
	WaterShader* waterShader;
	InstanceTextureShader* instanceShader;
	RainShader* rainShader;
	GrassShader* grassShader;

	FBXModelShader* fbxShader;

	VPBuffer* vpBuffer;
	Character* player;

	int testvalue = 0;
};