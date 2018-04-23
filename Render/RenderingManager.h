#pragma once

class ShaderManager;
class Sampler;
class Rasterizer;
class LightManager;

class RenderTexture;


class Environment;

class TestCube;

class GrassTexture;
class PerlinNoise;


class OrthoWindowPlane;
class CascadeShadowBuffer;
class VPBuffer;
class LightBuffer;

class Frustum;


class Character;

class RenderingManager {
public:
	RenderingManager();
	~RenderingManager();

	void Initianlize(Environment*);

	void AddCharacter(Character* );
	void Test(TestCube* testcube);

	void Update();

	void Render();
private:

	void LightView();
	void RenderShadow();
	void RenderReflection();
	void RenderRefraction();
	void RenderMain();

	void FinalRender();


	ShaderManager * shaderManager;
	Sampler* sampler;
	Rasterizer* rasterizer;
	LightManager* light;

	//RT
	RenderTexture * depthShadowTexture;
	RenderTexture* shadowTexture;
	RenderTexture* lakeReflectionTexture;
	RenderTexture* lakeRefractionTexture;
	RenderTexture* rainTexture;
	RenderTexture* mainRendering;
	RenderTexture* postRendering;



	GrassTexture* grassTexture;
	PerlinNoise* noise;

	//OrthoWindow
	OrthoWindowPlane* orthoWindow;

	//통합버퍼들
	CascadeShadowBuffer* shadowBuffer; //10번 슬롯
	VPBuffer* vpBuffer;       //12번 슬롯
	LightBuffer* lightBuffer;//11번 슬롯

	D3DXMATRIX view, projection;








	//여기서부턴 외부에서 받아오는 것들
	Frustum* frustum;
	D3DXMATRIX cropMatrix[4];

	//Landscape
	Environment* environment;


	TestCube* testcube;

	vector<Character*> characters;
};